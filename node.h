#include <vector>

using namespace std;


//enum actions{Up , Down , Left , Right};

struct node{
	int state[9];
	long long hash;
	int empty_cell;
	long cost;
	node* parent;
	int heuristic;

	/*bool operator <(const node& a)const{
		return a.hash < hash;
	}*/

};
