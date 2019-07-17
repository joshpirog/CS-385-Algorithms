/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Joshua Pirog
 * Date        : 9/17/18
 * Description : Sieve of Eratosthenes
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System."
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit) : is_prime_{new bool[limit + 1]}, limit_{limit} {
    	for ( int i = 2; i <= limit; i++){
    		is_prime_[i] = true;
    	}
        sieve();

        num_primes_ = count_num_primes();
        max_prime_ = max_prime();
    }

    ~PrimesSieve() {
        delete [] is_prime_;
    }
    // returns number of primes
    inline int num_primes() const {
        return num_primes_;
    }

    //displays primes in the required format
	void display_primes() const {
		const int max_prime_width = num_digits(max_prime_),
				primes_per_row = 80 / (max_prime_width + 1);
		int lines = 0;

		for (int i = 2; i < limit_ + 1; i++) {
			if (is_prime_[i] == true) {
				if (num_primes_ > primes_per_row) {
					if (lines % primes_per_row == 0)
						cout << endl << setw(max_prime_width) << i;
					else
						cout << setw(max_prime_width + 1) << i;
					lines++;
				} else {
					if (lines % primes_per_row == 0)
						cout << endl << i;
					else
						cout << " " << i;
					lines++;

				}
			}
		}
	}

private:
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    //finds the max prime
    int max_prime() {
       	for(int i = limit_; i>=2; i--){
       		if(is_prime_[i] == true){

       			return i;

       		}
       	}
       	return 2;
       }
    // counts the number of primes
    int count_num_primes() const {
    	int counter = 0;
    	for (int i = 2; i <= limit_; i++){
    		if(is_prime_[i]){
    			counter++;
    		}
    	}
    	return counter;

    }

    //determines the amount of digits in an integer
    int num_digits(int num) const {

    	int digits = 0;
    	if (num < 0){
    		digits = 1;
    	}
    	while(num){
    		num /= 10;
    		digits++;
    	}
    	return digits;
    }
    //sieve algorithm
    void sieve() {

    	for (int i = 2; i <= sqrt(limit_); i++){
    		if (is_prime_[i] == true){
    			for (int j = i * i; j <= limit_; j += i){
    				is_prime_[j] = false;
    			}
    		}

    	}

    }
};

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    // TODO: write code that uses your class to produce the desired output.
    PrimesSieve pr(limit);
    cout << endl << "Number of primes found: " << pr.num_primes() << endl;
    cout << "Primes up to " << limit << ":";
    pr.display_primes();

    return 0;
}
