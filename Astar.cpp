#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include "node.h"




using namespace std;

const int n = 4;
const int hash_base = 37;
const long long hash_mod = 1e10 + 19;
set<long long> explored;
set<pair<int , node*> > frontier;

void check(node* a){
	for(int j = 0 ; j < n ; j++){
	for(int i = 0 ; i < n ; i++)
		cerr << a->state[j * n + i] << " ";
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
			if(state[la] == 0 || state[la] / n != i)
				continue;
			for(int k = j + 1 ; k < n ; k++){
				//kth tile in ith row = (i * n + k)th in state
				int lb = i * n + k;
				if(state[lb] == 0 || state[lb] / n != i)
					continue;
				if(state[la] > state[lb])
						lc++;
			}
		}
	}
	return lc;
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
		down.heuristic = manhattanDistance(down.state);
		//second heuristic -> manhattan distance + linear conflict
		down.heuristic += linearConflict(down.state);
		//third heuristic -> disjoint pattern database
		//down.heuristic = 


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
		up.heuristic = manhattanDistance(up.state);
		//second heuristic -> manhattan distance + linear conflict
		up.heuristic += linearConflict(up.state);
		//third heuristic -> disjoint pattern database
		//up.heuristic += 

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
		right.heuristic = manhattanDistance(right.state);
		//second heuristic -> manhattan distance + linear conflict
		right.heuristic += linearConflict(right.state);
		//third heuristic -> disjoint pattern database
		//right.heuristic += 


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
		left.heuristic = manhattanDistance(left.state);
		//second heuristic -> manhattan distance + linear conflict
		left.heuristic += linearConflict(left.state);
		//third heuristic -> disjoint pattern database
		//left.heuritic = 

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


vector<node> Astar(node* initNode){
	if(goal_test(initNode->state)){
		return solution(initNode);
	}
	frontier.insert(make_pair(initNode->cost + initNode-> heuristic ,initNode));
	explored.insert(initNode->hash);
	while(!frontier.empty()){
		node* v = (*frontier.begin()).second;
		if(goal_test(v->state)) return solution(v);
		frontier.erase(frontier.begin());
		vector<node> childs = successor(v);
		for(node child: childs){
			if(explored.find(child.hash) == explored.end()){
				 /*cerr << "checking" << endl;
				 check(&child);
				 cerr << child.heuristic << " " << linearConflict(child.state) <<  endl;*/
				node* tmp = nodeCopy(child);
				frontier.insert(make_pair(tmp->cost + tmp->heuristic , tmp));
				explored.insert(child.hash);
			}
		}
	}
	return solution(initNode);
}






int main(){
	struct node init;// {{1 , 2 , 3 , 4 , 5, 6, 7, 8, 0} , 1 , 9 , 0 , &init , 0};
	for(int i = 0 ; i < (n * n) ; i++){
		cin >> init.state[i];
		if(init.state[i] == 0)
			init.empty_cell = i;
	}
	init.hash = make_hash(init.state);
	init.cost = 0;
	init.parent = &init;
	init.heuristic = manhattanDistance(init.state);




	vector <node> ans = Astar(&init);
	//cout << 1 << "   ";
	for(int j = 0 ; j < n * n ; j++){
			if(j == n * n - 1) cout << init.state[j];
			else cout << init.state[j] << ",";
	}
	cout << endl;
	for(int i = (ans.size() - 1) ; i >= 0  ; i--){
		//cout << (ans.size() - i + 1) << "   ";
		for(int j = 0 ; j < n * n ; j++){
			if(j == n * n - 1) cout << ans[i].state[j];
			else cout << ans[i].state[j] << ",";
		}
		cout << endl;
	}


}