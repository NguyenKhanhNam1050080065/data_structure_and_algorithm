#include <iostream>


unsigned long perm(unsigned long n){
	if (n <= 1) return 1;
	return n * perm(n - 1);
}

int main(){
	std::cout << perm(3);
}