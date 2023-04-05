#include <iostream>


long calc(long n, int i = 0){
	if (i == n) return n * n;
	return calc(n, i + 1) + (n * i);
}

int main(){
	std::cout << calc(15);
}
