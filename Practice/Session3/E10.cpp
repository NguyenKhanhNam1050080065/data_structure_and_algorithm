#include <iostream>
#include <cstdlib>

long sum_even(const int* arr, const unsigned int& count_down){
	if (!count_down) return 0;
	const int curr = *arr;
	return (curr % 2 == 0 ? curr : 0) + sum_even(&arr[1], count_down - 1);
}

int main(){
	unsigned int n;
	std::cout << "N = "; std::cin >> n;
	int *arr = n == 0 ? nullptr : (int*)malloc(sizeof(int) * n);
	for (unsigned int i = 0; i < n; i++){
		std::cout << "arr[" << i << "] = "; std::cin >> arr[i];
	}
	std::cout << "Even numbers\' sum: " << sum_even(arr, n) << "\n";

	free(arr);
	return 0;
}
