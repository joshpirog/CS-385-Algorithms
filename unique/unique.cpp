/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Joshua Pirog
 * Date        : 9/25/18
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System."
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    // TODO: returns true if all characters in string are lowercase
    // letters in the English alphabet; false otherwise.
	if (s.empty()){
		return false;
	}
	for (unsigned int count = 0; count < s.size(); count++){
		char c = s[count];
		if(!(c >= 'a' && c <= 'z')){ //makes sure the string is a letter
			return false;
		}
	}
	return true;
}

bool all_unique_letters(const string &s) {
    // TODO: returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.
    // You may use only a single int for storage and work with bitwise
    // and bitshifting operators.
    // No credit will be given for other solutions.

	int check = 0;
	for (unsigned int i = 0; i < s.size(); i++){
		int ans = s[i] - 97;
		if ((check & (1 << ans)) > 0) { //checking if the letters are unique
			return false;
		}
		check |= (1 << ans);
	}
	return true;
}

int main(int argc, char * const argv[]) {
    // TODO: reads and parses command line arguments.
    // Calls other functions to produce correct output.
	if(argc != 2) {
		cerr << "Usage: " << argv[0] << " <string>" << endl;
		return 1;
	}
	string uniqueStr(argv[1]);

	if (!is_all_lowercase(uniqueStr)) {
		cerr << "Error: String must contain only lowercase letters." << endl;
		return 1;
	}
	else if (all_unique_letters(uniqueStr) == true) {
		cout << "All letters are unique." << endl;
		return 1;
	}
	else {
		cout << "Duplicate letters found." << endl;
		return 1;
	}
	return 0;
}
