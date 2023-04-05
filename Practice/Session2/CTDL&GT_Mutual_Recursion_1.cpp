//Source: https://en.wikipedia.org/wiki/Mutual_recursion
#include <iostream>

bool is_even(unsigned int n);
bool is_odd(unsigned int n);

bool is_even(unsigned int n) {
	if (n == 0)
		return true;
	else
		return is_odd(n - 1);
}

bool is_odd(unsigned int n) {
	if (n == 0)
		return false;
	else
		return is_even(n - 1);
}

int main(){
	std::cout << is_even(15) << "\n" << is_odd(15);
}