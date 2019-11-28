#include <iostream>
#include <set>
#include "node.h"


using namespace std;
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
	p.parent = &p;
	p.cost = 5;
	p.hash = 9;
	p.state[1] = 1;
	p.state[1] = 2;
	cout <<"ziad shod"<< p.state[1] << endl;

	struct node n = {{0 , 167 , 3 , 2} , 10*1LL , 10  , 10 , &p};
	Ytry(n.state);

	set<node> frontier;
	vector<node> tt = Jtry(&n);
	cout << (tt).size() << endl;
	frontier.insert(p);
}
