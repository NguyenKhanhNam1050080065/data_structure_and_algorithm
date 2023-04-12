// Source: https://www.geeksforgeeks.org/cpp-program-for-binary-search/


#include <vector>
#include <iostream>
#include <initializer_list>


class NumberVector : public std::vector<long> {
	static int b_search_internal(const std::vector<long>& arr, const int& l_bound, const int& r_bound, const long& what) {
		if (r_bound >= 1 && l_bound != r_bound && l_bound < arr.size() && r_bound < arr.size()) {
			int idx = l_bound + (r_bound - 1) / 2;
			if (arr[idx] == what) return idx;
			if (arr[idx] > what) return b_search_internal(arr, l_bound, idx - 1, what);
			return b_search_internal(arr, idx + 1, r_bound, what);
		}
		return -1;
	}
public:
	NumberVector() : std::vector<long>() {}
	NumberVector(const std::initializer_list<long>& list) : std::vector<long>(list) {}
	inline void bubble_sort() {
		for (int i = 0; i < size() - 1; i++) {
			for (int j = 0; j < size() - i - 1; j++) {
				auto& left = this->operator[](j);
				auto& right = this->operator[](j + 1);
				if (left > right) {
					auto temp = left;
					left = right;
					right = temp;
				}
			}
		}
	}
	inline int b_search(const long& what) const {
		return b_search_internal(*this, 0, size() - 1, what);
	}
};

int main() {
	NumberVector vec = { 2, 3, 4, 10, 40 };
	vec.bubble_sort();
	for (const auto& e : vec) {
		std::cout << e << " ";
	}
	std::cout << "\n" << vec.b_search(10) << "\n";
	return 0;
}
