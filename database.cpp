#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <time.h>
#include <string>
#include "dpdb.h"


using namespace std;
const int n = 4;
int cnt[100];
const int hash_base = 37;
const long long hash_mod = 9827870924701019; //15 digit prime number
map<string , int> database;
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

		if(closed.find(down.hash) == closed.end()) child.push_back(down);
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

		if(closed.find(up.hash) == closed.end()) child.push_back(up);
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

		if(closed.find(right.hash) == closed.end()) child.push_back(right);
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

		if(closed.find(left.hash) == closed.end()) child.push_back(left);
	}

	return child;

}
string make_pattern(int* state){
	string pattern = "";
	set<pair<int , int> > situation;
	for(int i = 0 ; i < n * n ; i++){
		if(state[i] != -1 && state[i] != 0)
			situation.insert(make_pair(state[i] , i));
	}
	for(auto it = situation.begin() ; it != situation.end() ; it++){
		int place = (*it).second;
		if(place < 10) pattern += '0';
		pattern +=  to_string(place);
	}

	return pattern;
}
void update_database(string pattern, int cost){
	if(database.find(pattern) == database.end())
		database[pattern] = cost;
	else if(database[pattern] > cost)
		database[pattern] = cost;
}

void bfs(dpdb* goalNode){

	open.push(goalNode);
	closed.insert(goalNode->hash);

	while(!open.empty()){
		dpdb* node = open.front();
		open.pop();
		vector<dpdb> childs = successor(node);
		for(dpdb child : childs){
			if(closed.find(child.hash) == closed.end()){
				dpdb* tmp = copydpdb(child);
				update_database(make_pattern(tmp->state) , tmp->cost);
						if(tmp->cost == 1)
			check(tmp->state);
				open.push(tmp);
				closed.insert(tmp->hash);
			}
		}
	}
}

void save_database(string path){
	ofstream fout;
	fout.open(path);
	cout << database.size() << endl;
	for(auto it = database.begin() ; it!= database.end() ; it++){
		fout << it->first << " " << it->second << endl;
		cnt[it->second]++;

	}
	for(int i = 0 ; i < 25 ; i++)
		cout << i << " " << cnt[i] << endl;
	fout.close();

}

void database_5_5_5(){
	//pattern 1 : 1 2 3 4 x x 7 x x x x x x x x 0
	/*struct dpdb goal_1;
	for(int i = 0 ; i < n * n ; i++)
		goal_1.state[i] = -1;
	goal_1.state[0] = 1;
	goal_1.state[1] = 2;
	goal_1.state[2] = 3;
	goal_1.state[3] = 4;
	goal_1.state[6] = 7;
	goal_1.state[15] = 0;
	goal_1.cost = 0;
	goal_1.hash = make_hash(goal_1.state);
	goal_1.empty_cell = 15;
	bfs(&goal_1);
	cerr << "done" << endl;
	save_database("DPdatabase/DB5_5_5/DB1.txt");*/

	//pattern 2 : x x x x 5 6 x x 9 10 x x 13 x x 0
	/*struct dpdb goal_2;
	for(int i = 0 ; i < n * n ; i++)
		goal_2.state[i] = -1;
	goal_2.state[0] = 1;
	goal_2.state[1] = 2;
	goal_2.state[4] = 5;
	goal_2.state[8] = 9;
	goal_2.state[12] = 13;
	goal_2.state[15] = 0;
	goal_2.cost = 0;
	goal_2.hash = make_hash(goal_2.state);
	goal_2.empty_cell = 15;
	bfs(&goal_2);

	save_database("DPdatabase/DB5_5_5/DB2.txt");*/

	//pattern 3: x x x x x x x 8 x x 11 12 x 14 15 0
	struct dpdb goal_3;
	for(int i = 0 ; i < n * n ; i++)
		goal_3.state[i] = -1;
	goal_3.state[2] = 3;
	goal_3.state[3] = 4;
	goal_3.state[6] = 7;
	goal_3.state[7] = 8;
	goal_3.state[11] = 12;
	goal_3.state[15] = 0;
	goal_3.cost = 0;
	goal_3.hash = make_hash(goal_3.state);
	goal_3.empty_cell = 15;
	bfs(&goal_3);

	save_database("DPdatabase/DB5_5_5/DB5.txt");

}


int main(){
	clock_t tStart = clock();
	database_5_5_5();
	// database_6_6_3();
	// database_7_8();
	struct dpdb goal;
	for(int i = 0 ; i < n * n ; i++)
		goal.state[i] = -1;

	goal.state[0] = 1;
	goal.state[4] = 5;
	goal.state[8] = 9;
	goal.state[9] = 10;
	goal.state[12] = 13;
	goal.state[15] = 0;
	goal.cost = 0;
	goal.hash = make_hash(goal.state);
	goal.empty_cell = 15;
 	//bfs(&goal);
 	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
 	// for(int i = 0 ;)
	/*ofstream fout;
	fout.open("DPdatabase/DB5_5_5/DB1.txt");
	fout << "trugnjkvf njfk njfn fjkgnfjkdfjkfd l;f fjk fj ld" << endl;
	fout.close();*/
	/*
	do something
	*/


}
