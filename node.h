#include <vector>

using namespace std;



struct node{
	int state[16];
	long long hash;
	int empty_cell;
	long cost;
	node* parent;
	int heuristic;

};
