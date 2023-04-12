#include <vector>
#include <iostream>
#include <initializer_list>

class NumberVector : public std::vector<long> {
public:
	NumberVector() : std::vector<long>() {}
	NumberVector(const std::initializer_list<long>& list) : std::vector<long>(list) {}
	inline void i_sort() {
		for (int i = 1; i < size(); i++) {
			long key = this->operator[](i);
			int j = i - 1;
			while (j >= 0 && this->operator[](j) > key){
				this->operator[](j + 1) = this->operator[](j);
				j -= 1;
			}
			this->operator[](j + 1) = key;
		}
	}
};

int main() {
	NumberVector vec= { 41, 23, 4, 14, 56, 1 };
	vec.i_sort();
	for (const auto& e : vec) {
		std::cout << e << " ";
	}
	return 0;
}
