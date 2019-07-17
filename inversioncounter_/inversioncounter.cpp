/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Joshua Pirog
 * Version     : 1.0
 * Date        : 11/4/18
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in theta(n^2) time.
 */
long count_inversions_slow(int array[], int length) {
    // TODO
	long inversions = 0;
	for (int i = 0; i < length - 1; i++){
		for (int j = i + 1; j < length; j++){
			if (array[i] > array[j]){
				inversions++; // if higher value is to the left of lower value, increment inversions
			}
		}
	}
	return inversions;
}

/**
 * Counts the number of inversions in an array in theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    // TODO
    // Hint: Use mergesort!
	int *scratch = new int[length];
	long inversions = mergesort(array, scratch, 0, length - 1);

	delete [] scratch;
	return inversions;
}

static long mergesort(int array[], int scratch[], int low, int high) {
    // TODO
	long inversions = 0;
	if (low < high){
		int mid = low + (high - low)/2;
		inversions = mergesort(array, scratch, low, mid);
		inversions += mergesort(array, scratch, mid + 1, high);

		int L = low;
		int H = mid + 1;
		for (int k = low; k <= high; k++){
			if (L <= mid && ((H > high) || array[L] <= array[H])){
				scratch[k] = array[L];
				L = L + 1;
			}
			else {
				scratch[k] = array[H];
				inversions = inversions + (mid - L) + 1;
				H = H + 1;
			}
		}
		for (int k = low; k <= high; k++){
			array[k] = scratch[k];
		}

	}
	return inversions;
}

int main(int argc, char *argv[]) {
    // TODO: parse command-line argument

	if (argc > 2) { //too many paramenters
		cout << "Usage: " << argv[0] << " [slow]" << endl;
		return -1;
	}

	string speed = "fast"; //default
	if (argc == 2) {
		speed = argv[1];
		if (argc > 1 && (speed != "slow")) {
			cout << "Error: Unrecognized option '" << speed << "'." << endl;
			return -1;
		}
	}


    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    // TODO: produce output
    if (values.size() == 0) {
    	cerr << "Error: Sequence of integers not received." << endl;
    	return -1;
    }
    if (speed == "slow") { //slow function
    	cout << "Number of inversions: " << count_inversions_slow(&values[0], values.size()) << endl;
    	return -1;
    }

    else {//default to fast function
    	cout << "Number of inversions: " << count_inversions_fast(&values[0], values.size()) << endl;
    	return -1;
    }


    return 0;
}
