#include <iostream>
#include <cstdlib>

bool all_negative(const float* arr, const unsigned int& count_down){
	if (!count_down) return true;
	if (*arr >= 0.0F) return false;
	return all_negative(&arr[1], count_down - 1);
}

int main(){
	unsigned int n;
	std::cout << "N = "; std::cin >> n;
	float *arr = n == 0 ? nullptr : (float*)malloc(sizeof(float) * n);
	for (unsigned int i = 0; i < n; i++){
		std::cout << "arr[" << i << "] = "; std::cin >> arr[i];
	}
	std::cout << "Positive number count: " << all_negative(arr, n) << "\n";

	free(arr);
	return 0;
}
