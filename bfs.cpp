#include <iostream>
#include <explored>
#include <queue>
#include "node.h"



using namespace std;
const int n = 9;
set<long long> explored;
queue<node> frontier;


vector<node> bfs(node *initNode){
	if(goal_test(*initNode)) return solution(&initNode);
	frontier.push(*initNode);
	explored.insert(*initNode);
	while(!frontier.empty()){
		node node = frontier.front();
		vector<node> childs = successor(&node);
		for(node child: childs){
			if(explored.find(child.hash) == explored.end()){
				if(goal_test(child)) return solution(child);
				frontier.push(child);
				explored.insert(child);
			}
		}
	}st
}

bool goal_test(node *node){
	if(node->state[n - 1] != 0) return 0;
	for(int i = 0 ; i < (n - 1) ; i++){
		if(node->state[i] != i + 1)
			return 0;
	}
	return 1;
}

vector<node> solution(node *node){
	vector<node> ans;
	while((node->parent)->hash != node->hash){
		ans.push_back(*node);
		node = node->parent;
	}
	return ans
}

vector<node> successor(node* node){
	vector<node> child;
	int parent_empty = *node->empty_cell;
	//the row and column of empty cell
	int r = parent_empty / n ,  c = parent_empty % n;

	//empty cell go down
	if(r + 1 < n){
		struct node down;

		for(int i = 0 ; i < (n*n) ; i++)
			down.state[i] = node->state[i];

		down.state[parent_empty] = node->state[parent_empty + n];
		down.state[parent_empty + n] = 0;

		down.hash = make_hash(down.state);
		down.empty_cell = parent_empty + n;
		down.cost = node->cost + 1;
		down.parent = node;

		child.push_back(down);
	}

	//empty cell go up
	if(r - 1 >= 0){
		struct node up;

		for(int i = 0 ; i < n * n ; i++)
			up.state[ii] = node->state[ii];
		up.state[parent_empty] = node->state[parent_empty - n];
		up.state[parent_empty - n] = 0;

		up.empty_cell = parent_empty - n;
		up.hash = make_hash(up.state);
		up.cost = node->cost + 1;
		up.parent = node;

		child.push_back(up);
	}

	//empty cell going right
	if(c + 1 < n){
		struct node right;
		for(int i = 0 ; i < (n * n) ; i++)
			right.state[i] = node->state[i];
		right.state[parent_empty] = node->state[parent_empty + 1];
		right.state[parent_empty + 1] = 0;

		right.empty_cell = parent_empty + 1;
		right.hash = make_hash(right.state);
		right.cost = node->cost + 1;
		right.parent = node;

		child.push_back(right);
	}
	//empty cell going left
	if(c - 1 >= 0){
		struct node left;

		for (int i = 0; i < (n * n) ; i++)
			left.state[i] = node->state[i];

		left.state[empty_cell] = node->state[parent_empty - 1];
		left.state[empty_cell - 1] = 0;

		left.empty_cell = parent_empty - 1;
		left.hash = make_hash(left.state);
		left.cost = node->cost + 1;
		left.parent = node;

		child.push_back(left);
	}

	return child;
} 


int main(){

}