#include <iostream>
#include <cstdlib>
#include <limits>

float max_real_number(const float* arr, const unsigned int& count_down, const float& curr_max = std::numeric_limits<float>::min()){
	if (!count_down) return curr_max;
	return max_real_number(&arr[1], count_down - 1, *arr > curr_max ? *arr : curr_max);
}

int main(){
	unsigned int n;
	std::cout << "N = "; std::cin >> n;
	float *arr = n == 0 ? nullptr : (float*)malloc(sizeof(float) * n);
	for (unsigned int i = 0; i < n; i++){
		std::cout << "arr[" << i << "] = "; std::cin >> arr[i];
	}
	std::cout << "Positive number count: " << max_real_number(arr, n) << "\n";

	free(arr);
	return 0;
}
