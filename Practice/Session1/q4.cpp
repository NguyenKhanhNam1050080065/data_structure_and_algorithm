#include <iostream>
#include <cstdlib>
unsigned long long calculate_sum(const unsigned int& n){
	if (n == 0) return 1;
	if (n == 1) return n * n;
	unsigned long long *x_arr = (unsigned long long*)malloc(sizeof(unsigned long long) * (n + 1));

	x_arr[0] = 1;
	x_arr[1] = n * n;
	unsigned long long total = x_arr[1];
	for (unsigned int i = 2; i <= n; i++){
		x_arr[i] = (n - 1) * (n - 1) * x_arr[i - 1];
		total += x_arr[i];
	}
	free(x_arr);
	return total;
}

int main(){
	std::cout << calculate_sum(12);
}
