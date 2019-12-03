#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <string>
#include "dpdb.h"


using namespace std;
const int n = 4;
const int hash_base = 37;
const long long hash_mod = 9827870924701019;
const int max_part3 = 43680;
const int max_part6 = 57657600;
map<string , int> database;
int number_of_node = 0;
set<long long> closed;
queue<dpdb*> open;

void check(int* state){
	for(int j = 0 ; j < n ; j++){
		for(int i = 0 ; i < n ; i++)
			cerr << state[j * n + i] << " ";
			cerr << endl;
	}
	cerr << endl;
}

dpdb* copydpdb(dpdb child){
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
		if(v[i] == 0){
			hash += (power * 16 * 1LL) % hash_mod;
			hash %= hash_mod;	
		}
		power = (hash_base * power * 1LL) % hash_mod;
	}
	return hash;
}

vector<dpdb> successor(dpdb* parent){
	//row and column of empty cell
	int r = (parent->empty_cell) / n , c = (parent->empty_cell) % n;
	int parent_empty = parent->empty_cell;
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
		down.cost = (down.state[parent_empty] == -1)? parent->cost : (parent->cost + 1);

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
		struct dpdb left;

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
string make_pattern(int* state){
	// cerr << "Check how pattern work!" << endl;
	// check(state);
	string pattern = "";
	set<pair<int , int> > situation;
	for(int i = 0 ; i < n * n ; i++){
		if(state[i] != -1 && state[i] != 0)
			situation.insert(make_pair(state[i] , i));
	}
	// cerr << "check the size" << situation.size() << endl;
	for(auto it = situation.begin() ; it != situation.end() ; it++){
		int place = (*it).second;
		if(place < 10) pattern += '0';
		pattern +=  to_string(place);
	}
	// cerr << pattern << endl;
	return pattern;
}
void update_database(string pattern, int cost){
	if(database.find(pattern) == database.end())
		database[pattern] = cost;
	else if(database[pattern] > cost)
		database[pattern] = cost;
}

void bfs(dpdb* goalNode){//,&ofstream file){
	check(goalNode->state);
	 cerr << goalNode->hash << endl;
	open.push(goalNode);
	closed.insert(goalNode->hash);
	int cnt = 1;
	while(!open.empty()){
		dpdb* node = open.front();
		open.pop();
		vector<dpdb> childs = successor(node);
		for(dpdb child : childs){
			if(closed.find(child.hash) == closed.end()){
				dpdb* tmp = copydpdb(child);
				// check(tmp->state);
				// cerr << "BAM" << tmp->cost << endl;
				update_database(make_pattern(tmp->state) , tmp->cost);
				open.push(tmp);
				closed.insert(tmp->hash);
				if(closed.size() > cnt * max_part3 * 12){
					cnt++;
					cerr << "capture" << " " << cnt <<  endl;
				}
			}
		}
	}
	cerr << closed.size() << endl;
}

void save(long long hash,int cost,ofstream& fout){
	//fout << hash << " " << cost

}
int main(){
	struct dpdb goal;
	for(int i = 0 ; i < n * n ; i++)
		goal.state[i] = -1;
	/*goal.state[1] = 2;
	goal.state[2] = 3;
	goal.state[3] = 4;
	goal.state[15] = 0;*/
	goal.state[0] = 1;
	goal.state[4] = 5;
	goal.state[5] = 6;
	goal.state[8] = 9;
	goal.state[9] = 10;
	goal.state[12] = 13;
	goal.state[15] = 0;
	/*goal.state[2] = 3;
	goal.state[3] = 4;
	goal.state[6] = 7;
	goal.state[7] = 8;
	goal.state[11] = 12;*/
	goal.cost = 0;
	goal.hash = make_hash(goal.state);
	goal.empty_cell = 15;
 	bfs(&goal);
 	// for(int i = 0 ;)
	/*ofstream database1;
	database1.open("database1.txt");*/
	/*
	do something
	*/



	
}