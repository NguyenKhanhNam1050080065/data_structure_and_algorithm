#include <iostream>
#include <cstdlib>

float sum(const float* arr, const unsigned int& count_down){
	if (!count_down) return 0.0F;
	return *arr + sum(&arr[1], count_down - 1);
}

int main(){
	unsigned int n;
	std::cout << "N = "; std::cin >> n;
	float *arr = n == 0 ? nullptr : (float*)malloc(sizeof(float) * n);
	for (unsigned int i = 0; i < n; i++){
		std::cout << "arr[" << i << "] = "; std::cin >> arr[i];
	}
	std::cout << "Positive number count: " << sum(arr, n) << "\n";

	free(arr);
	return 0;
}
