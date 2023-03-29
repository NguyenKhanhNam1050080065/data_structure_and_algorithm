#include <iostream>
#include <cmath>

double perm(const unsigned int& n){
	double re = 1U;
	for (unsigned int i = 2; i <= n; i++){
		re *= i;
	}
	return re;
}

double stuff(const int& x, const unsigned int& n){
	if (n == 1U || n == 0U) return x;
	return (std::pow(x, n) / perm(n)) + stuff(x, n - 1);
}

int main(){
	std::cout << stuff(2, 2);
}
