#include <vector>

using namespace std;


enum actions{Up , Down , Left , Right};

struct node{
	vector<int> state;
	long long hash_number;
	actions action;
	long cost;
	node* parent;

};

bool operator <(const node& a,const node& b){
	return a.hash_number < b.hash_number;
}