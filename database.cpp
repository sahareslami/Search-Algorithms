#include "dpdb.h"


using namespace std;

const int hash_base = 37;
const int hash_mode = 1e9 + 7;



node* copydpdb(dpdb child){
	dpdb* tmp = new dpdb;
	for(int i = 0 ; i < n * n; i++)
		tmp->state[i] = child.state[i];
	tmp->hash =  child.hash;
	tmp->empty_cell = child.empty_cell;
	tmp->cost = child.cost;
	return tmp;
}

long long make_hash(int* v){
	long long power = 1LL;
	long long hash = 0 * 1LL;
	for(int i = 0 ; i < (n * n) ; i++){
		if(v[i] != -1){
			hash += (power * v[i] * 1LL) % hash_mod;
			hash %= hash_mod;
		}
		power = (hash_base * power * 1LL) % hash_mod;
	}
	return hash;
}

vector<dpdb> successor(dpdb* parent){
	//row and column of empty cell
	int r = (parent->empty_cell) / n , c = (parent->empty_cell) % n;
	int parent_empty = parent->parent;
	vector<dpdb> child;

	//empty cell go down
	if(r + 1 < n){
		struct dpdb down;

		for(int i = 0 ; i < (n*n) ; i++)
			down.state[i] = parent->state[i];

		down.state[parent_empty] = parent->state[parent_empty + n];
		down.state[parent_empty + n] = 0;

		down.hash = make_hash(down.state);
		down.empty_cell = parent_empty + n;
		down.cost = (down.state[parent_empty] == -1)? parent->cost : (parnet->cost + 1);

		child.push_back(down);
	}

	//empty cell go up
	if(r - 1 >= 0){
		struct dpdb up;

		for(int i = 0 ; i < n * n ; i++)		
			up.state[i] = parent->state[i];
		up.state[parent_empty] = parent->state[parent_empty - n];
		up.state[parent_empty - n] = 0;

		up.empty_cell = parent_empty - n;
		up.hash = make_hash(up.state);
		up.cost = (up.state[parent_empty] == -1)? parent->cost : (parent->cost + 1);

		child.push_back(up);
	}

	//empty cell going right
	if(c + 1 < n){
		struct dpdb right;

		for(int i = 0 ; i < (n * n) ; i++)
			right.state[i] = parent->state[i];
		right.state[parent_empty] = parent->state[parent_empty + 1];
		right.state[parent_empty + 1] = 0;

		right.empty_cell = parent_empty + 1;
		right.hash = make_hash(right.state);
		right.cost = (right.state[parent_empty] == -1)? parent->cost : (parent->cost + 1);

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
		left.cost = (left.state[parent_empty] == -1)? parent->cost : (parent->cost + 1);

		child.push_back(left);
	}

	return child;

}
void bfs(dpdb* goalNode,&ofstream file){
	open.push(goalNode);
	closed.push(goalNode->hash);
	while(!open.empty()){
		dpdb* node = open.front();
		open.pop();
		vector<dpdb> childs = successor(node);
		for(dpdb child : childs){
			if(closed.find(child.hash) != closed.end()){
				dpdb* tmp = copydpdb(child);
				// save(tmp->hash , tmp->cost , mod);
				fout << hash << " " << cost << endl;
				open.push(tmp);
				closed.insert(tmp->hash);
			}
		}
	}
}

void save(long long hash,int cost,ofstream& fout){
	fout << hash << " " << cost

}
int main(){
	ofstream database1;
	database1.open("database1.txt");
	/*
	do something
	*/



	
}