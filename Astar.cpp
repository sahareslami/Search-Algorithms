#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include "node.h"



using namespace std;

const int n = 3;
const int hash_base = 37;
const long long hash_mod = 1e9 + 7;
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

bool goal_test(node* v){
	if(v->state[n * n - 1] != 0) return 0;
	for(int i = 0 ; i < (n * n - 1) ; i++){
		if(v->state[i] != i + 1)
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

int manhattanDistance(int* )


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
		down.heuristic = 0;

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
		up.heuristic = 0;

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
		right.heuristic = 0;

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
		left.heuristic = 0;

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
	if(goal_test(initNode)){
		return solution(initNode);
	}
	frontier.insert(make_pair(initNode->cost,initNode));
	explored.insert(initNode->hash);
	while(!frontier.empty()){
		node* v = (*frontier.begin()).second;
		if(goal_test(v)) return solution(v);
		frontier.erase(frontier.begin());
		vector<node> childs = successor(v);
		for(node child: childs){
			if(explored.find(child.hash) == explored.end()){
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
	init.heuristic = 0;




	vector <node> ans = Astar(&init);
	for(int i = 0 ; i < ans.size() ; i++){
		for(int j = 0 ; j < n * n ; j++)
			cout << ans[i].state[j] << " ";
		cout << endl;
	}


}