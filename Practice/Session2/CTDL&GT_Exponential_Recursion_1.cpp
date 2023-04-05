// Source: https://www.geeksforgeeks.org/program-to-calculate-ex-by-recursion/
#include <iostream>


double e(unsigned int x, unsigned int n){
	static double p = 1, f = 1;
	double r;
	if (n == 0)
		return 1;
	r = e(x, n - 1);
	p = p * x;
	f = f * n;

	return (r + p / f);
}
int main(){
	std::cout << e(2, 3);
}