#include <iostream>
#include <cstdlib>

void output(const int* arr, const unsigned int& count_down){
	if (!count_down) return;
	std::cout << *arr << " ";
	output(&arr[1], count_down - 1);
}

int main(){
	unsigned int n;
	std::cout << "N = "; std::cin >> n;
	int *arr = n == 0 ? nullptr : (int*)malloc(sizeof(int) * n);
	for (unsigned int i = 0; i < n; i++){
		std::cout << "arr[" << i << "] = "; std::cin >> arr[i];
	}
	std::cout << "Even numbers\' sum: "; output(arr, n); std:: cout << "\n";

	free(arr);
	return 0;
}
