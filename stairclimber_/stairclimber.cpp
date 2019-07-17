/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Joshua Pirog
 * Date        : 10/03/18
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System." Joshua Pirog
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vectors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.

	vector<vector<int>> ways;
	vector<vector<int>> result;

	if(num_stairs <= 0){ //checks if there are stairs
		vector<int> way;
		ways.push_back(way);
	}
	else{

		for(int i = 1; i < 4; i++){
			if(num_stairs >= i){
				result = get_ways(num_stairs - i); //recursion for previous possible ways to climb stairs
				for(vector<vector<int>>::iterator it = result.begin(); it < result.end(); it++){
					it->insert(it->begin(), i); //add current number of stairs to the list
				}
				ways.insert(ways.end(), result.begin(), result.end());
			}
		}
	}
	return ways;
}

void display_ways(const vector< vector<int> > &ways) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.

	for(unsigned int i = 0; i < ways.size(); i++){ // for all "ways" vectors
		if(i <= 8 && ways.size() > 9){
			cout << " " << i+1 << ". " << "[";
		}
		else{
			cout << i+1 << ". " << "[";
		}

		for(unsigned int j = 0; j < ways[i].size(); j++){ //for all elements in "ways"
			if(j != ways[i].size() - 1)
				cout << ways[i][j] << ", ";
			else
				cout << ways[i][j];
		}
		cout << "]" << endl;
	}
}

bool number(const string &s){ //checks for positive integer
	for(unsigned int count = 0; count < s.size(); count++){
		char c = s[count];

		if(c > 57 || c < 48){
			cerr << "Error: Number of stairs must be a positive integer." << endl;
			return false;
		}
	}
	return true;

}

int main(int argc, char * const argv[]) {
	// prints the different ways one can climb given stairs in the requested format
	if(argc != 2){
		cerr << "Usage: " << argv[0] << " <number of stairs>" << endl;
		return 1;
	}
	string stairs(argv[1]);


	if(number(stairs)){
		istringstream buffer(stairs);

		int val;
		buffer >> val;

		vector<vector<int>> result = get_ways(val);

		if(result.size() == 1){
			cout << "1 way to climb 1 stair." << endl;
		}
		else{
			cout << result.size() << " ways to climb " << argv[1] << " stairs." << endl;
		}

		display_ways(get_ways(val));
	}

}
