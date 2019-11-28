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
	int parent_empty = *node->empty_cell;
	int i = parent_empty / n , j = parent_empty % n;
	//empty cell go down
	if(i + 1 < n){
		struct node down;
		for(int i = 0 ; i < (n*n) ; i++)
			down.state[i] = node->state[i];
		down.state[parent_empty] = node->state[parent_empty + n];
		down.state[parent_empty + n] = 0;

		down.hash = make_hash(down.state);
		down.empty_cell = parent_empty + n;
		down.cost = node.cost + 1;
		down.parent = node;
	}


} 


int main(){

}