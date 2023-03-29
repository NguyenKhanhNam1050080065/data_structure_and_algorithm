#include <iostream>
#include <cstdlib>

long count_positive(const float* arr, const unsigned int& count_down){
	if (!count_down) return 0;
	return (*arr > 0.0F ? 1 : 0) + count_positive(&arr[1], count_down - 1);
}

int main(){
	unsigned int n;
	std::cout << "N = "; std::cin >> n;
	float *arr = n == 0 ? nullptr : (float*)malloc(sizeof(float) * n);
	for (unsigned int i = 0; i < n; i++){
		std::cout << "arr[" << i << "] = "; std::cin >> arr[i];
	}
	std::cout << "Positive number count: " << count_positive(arr, n) << "\n";

	free(arr);
	return 0;
}
