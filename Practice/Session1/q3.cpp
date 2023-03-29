#include <iostream>
#include <cstdlib>


void calculate_sum(const unsigned int& n, unsigned long long& result_x, unsigned long long& result_y){
	unsigned long long *x_arr = (unsigned long long*)malloc(sizeof(unsigned long long) * (n + 1));
	unsigned long long *y_arr = (unsigned long long*)malloc(sizeof(unsigned long long) * (n + 1));

	x_arr[0] = 1;
	y_arr[0] = 0;
	for (unsigned int i = 1; i <= n; i++){
		x_arr[i] = x_arr[i - 1] + y_arr[i - 1];
		y_arr[i] = (3 * x_arr[i - 1]) + (2 * y_arr[i - 1]);
	}
	result_x = x_arr[n];
	result_y = y_arr[n];
	free(x_arr);
	free(y_arr);
}

int main(){
	unsigned long long x, y;
	calculate_sum(10, x, y);
	std::cout << x << " " << y;
}
