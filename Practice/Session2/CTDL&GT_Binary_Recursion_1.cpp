#include <iostream>


unsigned long fibonacci(unsigned long n){
	if (n <= 2) return n;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(){
	std::cout << fibonacci(30);
}