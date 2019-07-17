/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Michael Novak & Joshua Pirog
 * Date        : 10/19/2018
 * Description : Water Jug Puzzle
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <deque>
#include <list>
#include <queue>

using namespace std;

//Steps
//1) Pour from C to A
//2) Pour from B to A
//3) Pour from C to B
//4) Pour from A to B
//5) Pour from B to C
//6) Pour from A to C

//Pours water from the 'from' jug to the 'to' jug. Returns the 'curr' vector after the pour is completed.
//If pouring all water from the 'from' jug would cause over flow in the 'to' jug, then only pour as much as will fit.
vector<int> pour(vector<int> curr,vector<int>size, int from, int to, int step) {
	if(curr[from] > (size[to] - curr[to])) {
		curr[4] = (size[to] - curr[to]);
		curr[from] = curr[from] - (size[to] - curr[to]);
		curr[to] = size[to];
		curr[3] = step;
	} else {
		curr[4] = curr[from];
		curr[to] = curr[to] + curr[from];
		curr[from] = 0;
		curr[3] = step;
	}
	return curr;
}

//Input is two integer vectors of size 3
//Returns true if the vectors match, and false if they don't
bool compare_vec(vector<int> &curr,vector<int>&target) {
	int i = 0;
	for(i = 0; i < 3; i++) {
		if(!(curr[i] == target[i])){
			return false;
		}
	}
	return true;
}

//Finds all possible pours that can be made from the current state
//If a particular state has previously been visited, then it is not visited again
//If a new state has been found, then it is pushed to 'search'
bool find_pours(vector<int> &curr, int cap_a, int cap_b, int cap_c,
		vector<int> &target, vector<vector<int>> &visited, queue<vector<int>> &search) {
	vector<int> next;
	vector<int> cap_vec;
	bool can_pour = false;

	cap_vec.push_back(cap_a);
	cap_vec.push_back(cap_b);
	cap_vec.push_back(cap_c);

	if(curr[2] > 0 && curr[0] < cap_a) {
		//Pour C into A
		next = pour(curr, cap_vec, 2, 0, 1);
		if(compare_vec(next, target)) {
			search.push(next);
			return true;
		}
		if(visited[next[0]][next[1]] == 0) {
			can_pour = true;
			visited[next[0]][next[1]] = 1;
		}
		if(can_pour) {
			search.push(next);
			can_pour = false;
		}
		next.clear();
	}
	if(curr[1] > 0 && curr[0] < cap_a) {
		//Pour B into A
		next = pour(curr, cap_vec, 1, 0, 2);
		if(compare_vec(next, target)) {
			search.push(next);
			return true;
		}
		if(visited[next[0]][next[1]] == 0) {
			can_pour = true;
			visited[next[0]][next[1]] = 1;
		}
		if(can_pour) {
			search.push(next);
			can_pour = false;
		}
		next.clear();
	}
	if(curr[2] > 0 && curr[1] < cap_b) {
		//Pour C into B
		next = pour(curr, cap_vec, 2, 1, 3);
		if(compare_vec(next, target)) {
			search.push(next);
			return true;
		}
		if(visited[next[0]][next[1]] == 0) {
			can_pour = true;
			visited[next[0]][next[1]] = 1;
		}
		if(can_pour) {
			search.push(next);
			can_pour = false;
		}
		next.clear();
	}
	if(curr[0] > 0 && curr[1] < cap_b) {
		//Pour A into B
		next = pour(curr, cap_vec, 0, 1, 4);
		if(compare_vec(next, target)) {
			search.push(next);
			return true;
		}
		if(visited[next[0]][next[1]] == 0) {
			can_pour = true;
			visited[next[0]][next[1]] = 1;
		}
		if(can_pour) {
			search.push(next);
			can_pour = false;
		}
		next.clear();
	}
	if(curr[1] > 0 && curr[2] < cap_c) {
		//Pour B into C
		next = pour(curr, cap_vec, 1, 2, 5);
		if(compare_vec(next, target)) {
			search.push(next);
			return true;
		}
		if(visited[next[0]][next[1]] == 0) {
			can_pour = true;
			visited[next[0]][next[1]] = 1;
		}
		if(can_pour) {
			search.push(next);
			can_pour = false;
		}
		next.clear();
	}
	if(curr[0] > 0 && curr[2] < cap_c) {
		//Pour A into C
		next = pour(curr, cap_vec, 0, 2, 6);
		if(compare_vec(next, target)) {
			search.push(next);
			return true;
		}
		if(visited[next[0]][next[1]] == 0) {
			can_pour = true;
			visited[next[0]][next[1]] = 1;
		}
		if(can_pour) {
			search.push(next);
			can_pour = false;
		}
		next.clear();
	}

	return false;
}

//Returns a 2D vector of the graph up to the target
vector<vector<int>> breadthFirstSearch(vector<int> &curr,int cap_a, int cap_b, int cap_c,
		vector<int> &target, vector<vector<int>> &visited, queue<vector<int>> &search) {
	vector<vector<int>> bfs;
	search.push(curr);
	if(compare_vec(curr, target)) {
		bfs.push_back(search.front());
		search.pop();
		return bfs;
	} else {
		while(!search.empty()) {
			bfs.push_back(search.front());
			find_pours(search.front(), cap_a, cap_b, cap_c, target, visited, search);
			if(compare_vec(target, search.back())) {
				search.pop();
				while(!search.empty()) {
					bfs.push_back(search.front());
					search.pop();
				}
				return bfs;
			} else {
				search.pop();
			}
		}
		return bfs;
	}
}

//Recursively pops the end of the vector until the last element is the target
void previous(vector<vector<int>> &bfs,vector<int>&target) {
	if(compare_vec(bfs[bfs.size()-1], target)) {
		return;
	}
	bfs.pop_back();
	previous(bfs, target);
	return;
}

//Returns the path from start to target of a BFS by backtracking, using the path taken and amount poured to trace
//Reverses the backtracked trace to return the forwards trace
vector<vector<int>> find_path(vector<vector<int>> &bfs,vector<int>&target,vector<int>&start) {
	vector<int> curr;
	vector<int> next;
	vector<vector<int>> backwards;
	vector<vector<int>> forwards;
	if(compare_vec(bfs[bfs.size()-1], target)) {
		curr = bfs[bfs.size()-1];
		while(!compare_vec(curr, start)) {
			next.clear();
			backwards.push_back(bfs[bfs.size()-1]);
			if(curr[3] == 1) {
				next.push_back(curr[0]-curr[4]);
				next.push_back(curr[1]);
				next.push_back(curr[2]+curr[4]);
			}
			if(curr[3] == 2) {
				next.push_back(curr[0]-curr[4]);
				next.push_back(curr[1]+curr[4]);
				next.push_back(curr[2]);
			}
			if(curr[3] == 3) {
				next.push_back(curr[0]);
				next.push_back(curr[1]-curr[4]);
				next.push_back(curr[2]+curr[4]);
			}
			if(curr[3] == 4) {
				next.push_back(curr[0]+curr[4]);
				next.push_back(curr[1]-curr[4]);
				next.push_back(curr[2]);
			}
			if(curr[3] == 5) {
				next.push_back(curr[0]);
				next.push_back(curr[1]+curr[4]);
				next.push_back(curr[2]-curr[4]);
			}
			if(curr[3] == 6) {
				next.push_back(curr[0]+curr[4]);
				next.push_back(curr[1]);
				next.push_back(curr[2]-curr[4]);
			}
			previous(bfs, next);
			curr = bfs[bfs.size()-1];
		}
		backwards.push_back(bfs[0]);
		while(!backwards.empty()) {
			forwards.push_back(backwards.back());
			backwards.pop_back();
		}
		return forwards;
	} else {
		next.push_back(-1);
		forwards.push_back(next);
		return forwards;
	}
}

//Prints each pour step
void print_step(vector<vector<int>> &forwards) {
	unsigned int i;

	if(forwards[0][0] == -1) {
		cout << "No solution.";
		return;
	} else {
		cout << "Initial state. (" << forwards[0][0] << ", " << forwards[0][1] << ", " << forwards[0][2] << ")";
		for(i = 1; i < forwards.size(); i++) {
			cout << endl;
			if(forwards[i][4] == 1) {
				cout << "Pour 1 gallon from ";
			} else {
				cout << "Pour " << forwards[i][4] << " gallons from ";
			}
			if(forwards[i][3] == 1) {
				cout << "C to A. (";
			}
			if(forwards[i][3] == 2) {
				cout << "B to A. (";
			}
			if(forwards[i][3] == 3) {
				cout << "C to B. (";
			}
			if(forwards[i][3] == 4) {
				cout << "A to B. (";
			}
			if(forwards[i][3] == 5) {
				cout << "B to C. (";
			}
			if(forwards[i][3] == 6) {
				cout << "A to C. (";
			}
			cout << forwards[i][0] << ", " << forwards[i][1] << ", " << forwards[i][2] << ")";
		}
	}
}

//Capacities and goals must be positive, non-zero integers
//Proper variables must be input
//Goal of a particular jug cannot exceed the maximum capacity of the jug
//The goals of the jugs must sum to the capacity of jug C
int main(int argc, char * const argv[]) {
	int a, b, c, cap_a, cap_b, cap_c;

	if(argc != 7) {
		cerr << "Usage: " << argv[0] << " <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>";
		return 1;
	}
	istringstream iss;
	iss.str(argv[1]);
	if((!(iss >> cap_a))||(cap_a <= 0)) {
		cerr << "Error: Invalid capacity '" << argv[1] << "' for jug A.";
		return 1;
	}
	iss.clear();
	iss.str(argv[2]);
	if((!(iss >> cap_b))||(cap_b <= 0)) {
		cerr << "Error: Invalid capacity '" << argv[2] << "' for jug B.";
		return 1;
	}
	iss.clear();
	iss.str(argv[3]);
	if((!(iss >> cap_c))||(cap_c <= 0)) {
		cerr << "Error: Invalid capacity '" << argv[3] << "' for jug C.";
		return 1;
	}
	iss.clear();
	iss.str(argv[4]);
	if((!(iss >> a))||(a < 0)) {
		cerr << "Error: Invalid goal '" << argv[4] << "' for jug A.";
		return 1;
	}
	iss.clear();
	iss.str(argv[5]);
	if((!(iss >> b))||(b < 0)) {
		cerr << "Error: Invalid goal '" << argv[5] << "' for jug B.";
		return 1;
	}
	iss.clear();
	iss.str(argv[6]);
	if((!(iss >> c))||(c < 0)) {
		cerr << "Error: Invalid goal '" << argv[6] << "' for jug C.";
		return 1;
	}
	if(a > cap_a) {
		cerr << "Error: Goal cannot exceed capacity of jug A.";
		return 1;
	}
	if(b > cap_b) {
		cerr << "Error: Goal cannot exceed capacity of jug B.";
		return 1;
	}
	if(c > cap_c) {
		cerr << "Error: Goal cannot exceed capacity of jug C.";
		return 1;
	}
	if((a + b + c) != cap_c) {
		cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C.";
		return 1;
	}

	vector<int> start;
	start.push_back(0);
	start.push_back(0);
	start.push_back(cap_c);
	start.push_back(0);
	start.push_back(0);

	vector<int> target;
	target.push_back(a);
	target.push_back(b);
	target.push_back(c);

	vector<vector<int>> visited;
	vector<int> fill;
	for(int i = 0; i <= cap_b; i++) {
		fill.push_back(0);
	}
	for(int j = 0; j <= cap_c; j++) {
		visited.push_back(fill);
	}

	visited[0][0] = 1;

	queue<vector<int>> search;
	vector<vector<int>> bfs;
	vector<vector<int>> forwards;

	bfs = breadthFirstSearch(start, cap_a, cap_b, cap_c, target, visited, search);
	forwards = find_path(bfs, target, start);
	print_step(forwards);

	return 0;
}
