#include <iostream>
#include <set>
#include <time.h>
#include <vector>
#include <queue>
#include "node.h"



using namespace std;

const int n = 4;
const int hash_base = 37;
const long long hash_mod = 9827870924701019;
set<long long> forward_e,backward_e;
set<node*> forwardq,  backwardq;

void check(node* a){
	for(int j = 0 ; j < n ; j++){
	for(int i = 0 ; i < n ; i++)
		cerr << a->state[j * n + i] << " ";
		cerr << endl;
	}
	cerr << endl;
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
pair<node*,node*> find_middle(long long fhash){
	node* first;
	node* second;
	for(auto it = forwardq.begin() ; it != forwardq.end() ; it++)
		if((*it)->hash == fhash)
			first = *it;
	for(auto it = backwardq.begin() ; it!= backwardq.end() ; it++)
		if((*it)->hash == fhash)
			second = *it;
	return make_pair(first , second);
}

long long intersection(){
	if(forwardq.size() < backwardq.size()){
		for(auto it = forwardq.begin() ; it != forwardq.end() ; it++)
			if(backward_e.find((*it)->hash) != backward_e.end()){
				return (*it)->hash;
			}
		return -1;
	}
	else{
		for(auto it = backwardq.begin() ; it != backwardq.end() ; it++)
			if(forward_e.find((*it)->hash) != forward_e.end())
				return (*it)->hash;
		return -1;
	}
}
set<node*> update_forward(){
	set<node*> ans;
	for(auto it = forwardq.begin() ; it != forwardq.end() ; it++){
		// cerr << "forward time" << endl;
		// check(*it);
		vector<node> childs = successor(*it);
		for(node child: childs){
			if(forward_e.find(child.hash) == forward_e.end()){
				node* tmp = nodeCopy(child);
				ans.insert(tmp);
				forward_e.insert(tmp->hash);
			}
		}
	}

	return ans;
}

set<node*> update_backward(){
	set<node*> ans;
	for(auto it = backwardq.begin() ; it != backwardq.end() ; it++){
		vector<node> childs = successor(*it);
		for(node child : childs){
			if(backward_e.find(child.hash) == backward_e.end()){
				node* tmp = nodeCopy(child);
				ans.insert(tmp);
				backward_e.insert(tmp->hash);
			}
		}
	}	
	return ans;
}
pair<node*,node*> bds(node* init,node* goal){
	if(init->hash == goal->hash) return make_pair(init , goal);
	forwardq.insert(init);
	backwardq.insert(goal);
	forward_e.insert(init->hash);
	backward_e.insert(goal->hash);
	while(!forwardq.empty() && !backwardq.empty()){
		forwardq = update_forward();
		long long term = intersection();
		if(term != -1) return find_middle(term);
		backwardq = update_backward();
		term = intersection();
		if(term != -1) return find_middle(term);
	}
}


int main(){
	clock_t tStart = clock();
	struct node init;
	for(int i = 0 ; i < n * n ; i++){
		cin >> init.state[i];
		if(init.state[i] == 0)
			init.empty_cell = i;
	}
	init.hash = make_hash(init.state);
	init.cost = 0;
	init.parent = &init;
	init.heuristic = 0;

	struct node goal;
	for(int i = 0 ; i < (n * n) ; i++){
		goal.state[i] = i + 1;
	}
	goal.state[15] = 0;
	goal.empty_cell = 15;
	goal.hash = make_hash(goal.state);
	goal.cost = 0;
	goal.parent = &goal;
	init.heuristic = 0;


	pair<node*, node*> middle = bds(&init , &goal);
	vector <node> forward = solution(middle.first);
	vector <node> backward = solution(middle.second);
	for(int i = 0 ; i < n * n ; i++)
		if(i < 15) cout << init.state[i] << ",";
		else cout << init.state[i] << endl;

	for(int i = forward.size() - 1 ; i >= 0 ; i--){
		for(int j = 0 ; j < n * n ; j++)
			 cout << forward[i].state[j] << " ";
		cout << endl;
	}

	for(int i = 0 ; i < backward.size() ; i++){
		for(int j = 0 ; j < n * n ; j++)
			cout << backward[i].state[j] << " ";
		cout << endl;
	}
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

}