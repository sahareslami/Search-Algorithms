#include <iostream>
#include <time.h>
#include <fstream>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include "node.h"




using namespace std;

const int n = 4;
const int hash_base = 23;
const long long hash_mod = 9827870924701019;
map<long long , node*> explored;
set<pair<int , node*> > frontier;
map<string,int> database[3];
map<long long,node*> frontier_map;

void check(int* state){
	for(int j = 0 ; j < n ; j++){
	for(int i = 0 ; i < n ; i++)
		cerr << state[j * n + i] << " ";
		cerr << endl;
	}
	cerr << endl;
}

bool goal_test(int* state){
	if(state[n * n - 1] != 0) return 0;
	for(int i = 0 ; i < (n * n - 1) ; i++){
		if(state[i] != i + 1)
			return 0;
	}
	return 1;
}

vector<node> solution(node* v){
	vector<node> ans;
	while((v->parent)->hash != v->hash){
		ans.push_back(*v);
		v = v->parent;

	}
	return ans;
}
//first heuristic
int manhattanDistance(int* state){
	int md = 0;
	for(int i = 0 ; i < (n * n) ; i++){
		if(state[i] == 0) continue;
		//what is the goal row and column of this tile
		int gr = (state[i] - 1) / n , gc = (state[i] - 1) % n;
		//what is the row and column of this tile
		int r = i / n  , c = i % n;
		md += (max(gr - r , r - gr) + max(gc - c , c - gc));
	}
	return md;

}

//second heuristic
int linearConflict(int* state){
	int lc = 0;
	for(int i = 0 ; i < n ; i++){
		for(int j = 0 ; j < n ; j++){
			//jth tile in ith row = (i * n + j)th in state
			int la = i * n + j;
			if(state[la] == 0 || ((state[la] - 1) / n) != i)
				continue;
			for(int k = j + 1 ; k < n ; k++){
				//kth tile in ith row = (i * n + k)th in state
				int lb = i * n + k;
				if(state[lb] == 0 || ((state[lb] - 1) / n) != i)
					continue;
				if(state[la] > state[lb])
						lc++;
			}
		}
	}

	for(int i = 0 ; i < n ; i++){
		for(int j = 0 ; j < n ; j++){
			//j the tile of i th column
			int la = j * 4 + i;
			if(state[la] == 0 || ((state[la] - 1) % n) != i)
				continue;
			for(int k = j + 1 ; k < n ; k++){
				int lb = k * 4 + i;
				if(state[lb] == 0 || ((state[lb] - 1) % n) != i)
					continue;
				if(state[la] > state[lb])
					lc++;
			}
		}
	}
	
	return lc * 2;
}

int DisjointPatternDB(int* state){
	string p1 = "",p2 = "" ,p3 = "";
	string location[16];

	for(int i = 0 ; i < n * n ; i++){
		if(i < 10){
			string tmp = "0";
			tmp += to_string(i);
			location[state[i]] = tmp;
		}
		else
			location[state[i]] = to_string(i);
	}

	p1 = location[1] + location[2] + location[3] + location[4] + location[7];
	p2 = location[5] + location[6] + location[9] + location[10] + location[13];
	p3 = location[8] + location[11] + location[12] + location[14] + location[15];
	// check(state);
	// cerr << "check patterns " << p1 << " " << p2 << " " << p3 << endl;

	int ans = database[0][p1] + database[1][p2] + database[2][p3];
	/*if(ans < manhattanDistance(state)){
		check(state);
		cerr << "manhattanDistance" << manhattanDistance(state) << endl;
		cerr  << "DPDB" << database[0][p1] << " " << database[1][p2] << " " << database[2][p3] << endl;
	}*/
	return ans;

}

long long make_hash(int* v){
	long long power = 1LL;
	long long hash = 0 * 1LL;
	for(int i = 0 ; i < (n * n) ; i++){
		hash += (power * v[i] * 1LL) % hash_mod;
		hash %= hash_mod;
		power = (hash_base * power * 1LL) % hash_mod;
	}
	return hash;
}


vector<node> successor(node* parent){
	vector<node> child;
	int parent_empty = parent->empty_cell;
	//the row and column of empty cell
	int r = parent_empty / n ,  c = parent_empty % n;

	//empty cell go down
	if(r + 1 < n){
		struct node down;

		for(int i = 0 ; i < (n*n) ; i++)
			down.state[i] = parent->state[i];

		down.state[parent_empty] = parent->state[parent_empty + n];
		down.state[parent_empty + n] = 0;

		down.hash = make_hash(down.state);
		down.empty_cell = parent_empty + n;
		down.cost = parent->cost + 1;
		down.parent = parent;
		//first heuristic -> manhattan Distance
		// down.heuristic = manhattanDistance(down.state);
		//second heuristic -> manhattan distance + linear conflict
		// down.heuristic = linearConflict(down.state) + manhattanDistance(down.state);
		//third heuristic -> disjoint pattern database
		down.heuristic = DisjointPatternDB(down.state);

		child.push_back(down);
	}

	//empty cell go up
	if(r - 1 >= 0){
		struct node up;

		for(int i = 0 ; i < n * n ; i++)		
			up.state[i] = parent->state[i];
		up.state[parent_empty] = parent->state[parent_empty - n];
		up.state[parent_empty - n] = 0;

		up.empty_cell = parent_empty - n;
		up.hash = make_hash(up.state);
		up.cost = parent->cost + 1;
		up.parent = parent;

		//first heuristic -> manhattan Distance
		 // up.heuristic = manhattanDistance(up.state);
		//second heuristic -> manhattan distance + linear conflict
		// up.heuristic = linearConflict(up.state) + manhattanDistance(up.state);
		//third heuristic -> disjoint pattern database
		up.heuristic = DisjointPatternDB(up.state);

		child.push_back(up);
	}

	//empty cell going right
	if(c + 1 < n){
		struct node right;
		for(int i = 0 ; i < (n * n) ; i++)
			right.state[i] = parent->state[i];
		right.state[parent_empty] = parent->state[parent_empty + 1];
		right.state[parent_empty + 1] = 0;

		right.empty_cell = parent_empty + 1;
		right.hash = make_hash(right.state);
		right.cost = parent->cost + 1;
		right.parent = parent;

		//first heuristic -> manhattan Distance
		 // right.heuristic = manhattanDistance(right.state);
		//second heuristic -> manhattan distance + linear conflict
		// right.heuristic = linearConflict(right.state) + manhattanDistance(right.state);
		//third heuristic -> disjoint pattern database
		right.heuristic = DisjointPatternDB(right.state);


		child.push_back(right);
	}
	//empty cell going left
	if(c - 1 >= 0){
		struct node left;

		for (int i = 0; i < (n * n) ; i++)
			left.state[i] = parent->state[i];

		left.state[parent_empty] = parent->state[parent_empty - 1];
		left.state[parent_empty - 1] = 0;

		left.empty_cell = parent_empty - 1;
		left.hash = make_hash(left.state);
		left.cost = parent->cost + 1;
		left.parent = parent;
		//first heuristic -> manhattan Distance
		 // left.heuristic = manhattanDistance(left.state);
		//second heuristic -> manhattan distance + linear conflict
		// left.heuristic = linearConflict(left.state) + manhattanDistance(left.state);
		//third heuristic -> disjoint pattern database
		left.heuristic = DisjointPatternDB(left.state);

		child.push_back(left);
	}

	return child;
} 
node* nodeCopy(node child){
	node* tmp = new node;
	for(int i = 0 ; i < n * n; i++)
		tmp->state[i] = child.state[i];
	tmp->hash =  child.hash;
	tmp->empty_cell = child.empty_cell;
	tmp->cost = child.cost;
	tmp->parent =  child.parent;
	tmp->heuristic = child.heuristic;
	return tmp;
}

//this is a Astar algorithm for consistent heuristic
//manhattan distance is the only heuristic that is consistent
/*vector<node> AstarC(node* initNode){
	if(goal_test(initNode->state)) return solution(initNode);
	frontier.insert(make_pair(initNode->cost + initNode-> heuristic ,initNode));
	explored.insert(initNode->hash);
	while(!frontier.empty()){
		node* v = (*frontier.begin()).second;
		if(goal_test(v->state)) return solution(v);
		frontier.erase(frontier.begin());
		//if .find(explore)
		//continue;
		//add explore v
		vector<node> childs = successor(v);
		for(node child: childs){
			if(explored.find(child.hash) == explored.end()){
				node* tmp = nodeCopy(child);
				frontier.insert(make_pair((child.cost + child.heuristic) , tmp));
				//
				explored.insert(child.hash);
			}
		}
	}
	return solution(initNode);
}*/

//Astar for non-consistent heuristic
vector<node> Astar(node* initNode){
	if(goal_test(initNode->state)) return solution(initNode);
	frontier.insert(make_pair(initNode->cost + initNode->heuristic , initNode));
	frontier_map[initNode->hash] = initNode;
	while(!frontier.empty()){
		node* v = (*frontier.begin()).second;
		frontier.erase(frontier.begin());
		frontier_map.erase(v->hash);
		explored[v->hash] = v;
		if(goal_test(v->state)) return solution(v);
		vector<node> childs = successor(v);
		for(node child: childs){
			node* tmp = nodeCopy(child);
			//if child node are not in frontier
			if(frontier_map.find(tmp->hash) != frontier_map.end()){
				//if child node are in the frontier but need update
				auto it = frontier_map.find(tmp->hash);
				node* updated = it->second;
				int guess = updated->cost + updated->heuristic;
				if(guess > tmp->heuristic + tmp->cost){
					// cerr << "NON" << endl;
					frontier_map.erase(updated->hash);
					frontier_map[tmp->hash] = tmp;
					frontier.erase(make_pair(guess , updated));
					frontier.insert(make_pair(tmp->heuristic + tmp->cost , tmp));
				}
			} 
			if(explored.find(tmp->hash) != explored.end()){
				auto it = explored.find(tmp->hash);
				node* updated = it->second;
				int guess = updated->heuristic + updated->cost;
				if(guess > tmp->heuristic + tmp->cost){
					 // cerr << "NON consistent" << endl;
					explored.erase(updated->hash);
					frontier.insert(make_pair(tmp->heuristic + tmp->cost , tmp));
					frontier_map.insert(make_pair(tmp->hash , tmp));
				}
			}
			else{
				frontier.insert(make_pair(child.cost + child.heuristic , tmp));
				frontier_map[child.hash] = tmp;
			}
		}
	}
}


// vector<node*> Astar()

void upload_database(string path,int mode){
	ifstream fin;
	fin.open(path);
	while(!fin.eof()){
		string str;
		int cnt;
		fin >> str >> cnt;
		database[mode][str] = cnt;
	}
}




int main(){
	clock_t tStart = clock();
	upload_database("DPdatabase/DB5_5_5/DB1.txt" , 0);
	upload_database("DPdatabase/DB5_5_5/DB2.txt" , 1);
	upload_database("DPdatabase/DB5_5_5/DB3.txt" , 2);
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	struct node init;// {{1 , 2 , 3 , 4 , 5, 6, 7, 8, 0} , 1 , 9 , 0 , &init , 0};
	for(int i = 0 ; i < (n * n) ; i++){
		cin >> init.state[i];
		if(init.state[i] == 0)
			init.empty_cell = i;
	}
	init.hash = make_hash(init.state);
	init.cost = 0;
	init.parent = &init;
	//init.heuristic = manhattanDistance(init.state) + linearConflict(init.state);
	init.heuristic = DisjointPatternDB(init.state);



	vector <node> ans = Astar(&init);
	//cout << 1 << "   ";
	for(int j = 0 ; j < n * n ; j++){
			if(j == n * n - 1) cout << init.state[j];
			else cout << init.state[j] << ",";
	}
	cout << endl;
	for(int i = (ans.size() - 1) ; i >= 0  ; i--){
		//cout << (ans.size() - i + 1) << "   ";
		cerr << linearConflict(ans[i].state) << endl;
		for(int j = 0 ; j < n * n ; j++){
			if(j == n * n - 1) cout << ans[i].state[j];
			else cout << ans[i].state[j] << ",";
		}
		cout << endl;
	}
	cout << "path size : " << ans.size() << endl;
	cout << "number of node expanded : " << explored.size() << endl; 
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


}