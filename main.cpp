#include <iostream>
#include <set>
#include <functional>
#include "node.h"


using namespace std;
const int k = 6 * 1e8;
int l[k];
void Ytry(int* a){
	cout << "ahay" << a[1] << endl;

}
vector<node> Jtry(node* n){
	vector<node> a;
	cout << (n)->state[1] << endl;
	a.push_back(*n);
	cout << a[0].hash << endl;
	node node = a[0];
	struct node *l = (n->parent);
	int j = l->hash;
	cout << j << endl;
	struct node pl;
	pl.parent = n;
	a.push_back(*l);
	cout << a.size() << endl;
	return a;

}


int main(){
	struct node p;
	for(int i = 0 ; i < 9 ; i++)
		p.state[i] = i;
	p.parent = &p;
	p.cost = 5;
	p.hash = 9;

	struct node n;
	for(int i = 0 ; i < 9 ; i++)
		n.state[i] = i;
	n.parent = &n;
	n.cost = 9;
	n.hash = 2;
	//Ytry(n.state);

	set<int> fring;
	fring.insert(1);
	fring.insert(0);
	// node* v = (*fring.begin());
	int a = *fring.begin();
	cout << a;

	// for(auto it = fring.begin() ; it!= fring.end() ; it++)
		// cout << (*it)->cost << " " << (*it)->hash << endl;
}
