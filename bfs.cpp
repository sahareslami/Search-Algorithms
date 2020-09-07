#include <iostream>
#include <set>
#include <time.h>
#include <vector>
#include <queue>
#include "node.h"



using namespace std;

int n;
const int hash_base = 37;
const long long hash_mod = 9827870924701019;
set<long long> explored;
queue<node*> frontier;

/*void check(node* a){
	for(int j = 0 ; j < n ; j++){
	for(int i = 0 ; i < n ; i++)
		cerr << a->state[j * n + i] << " ";
		cerr << endl;
	}
	cerr << endl;
}*/

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


vector<node> bfs(node* initNode){
	if(goal_test(initNode->state)){
		return solution(initNode);
	}
	frontier.push(initNode);
	explored.insert(initNode->hash);
	while(!frontier.empty()){
		node* v = frontier.front();
		frontier.pop();
		vector<node> childs = successor(v);
		for(node child: childs){
			if(explored.find(child.hash) == explored.end()){
				if(goal_test(child.state))	return solution(&child);
				node* tmp = nodeCopy(child);
				frontier.push(tmp);
				explored.insert(child.hash);
			}
		}
	}
	return solution(initNode);
}

int main(){
	cin >> n;
	clock_t tStart = clock();
	struct node init;
	for(int i = 0 ; i < (n * n) ; i++){
		char t;
		cin >> init.state[i];
		if(i < 15) cin >> t;

		if(init.state[i] == 0)
			init.empty_cell = i;
	}
	init.hash = make_hash(init.state);
	init.cost = 0;
	init.parent = &init;
	init.heuristic = 0;




	vector <node> ans = bfs(&init);
	cout << ans.size() << endl;
	for(int i = ans.size() - 1 ; i >= 0 ; i--){
		for(int j = 0 ; j < n * n ; j++){
			cout << ans[i].state[j];
			if(j< 15) cout  << ",";
		}
		cout << endl;
	}
	cout << "REAL BUg" << explored.size() << endl;
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


}