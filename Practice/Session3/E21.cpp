#include <iostream>
int main(){
	unsigned int n;
	std::cout << "N = "; std::cin >> n;
	double val = 1.0F;
	for (unsigned int i = 1; i <= n; i++){
		val *= i;
	}
	std::cout << val << "\n";
	return 0;
}
