#include <iostream>


long nested(long n){
	if (n <= 0) return 1;
	return nested(nested(n - 1) - 1);
}

int main(){
	std::cout << nested(30);
}