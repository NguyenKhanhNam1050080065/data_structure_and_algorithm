#include <vector>
#include <iostream>
#include <initializer_list>

class NumberVector : public std::vector<long> {
public:
	NumberVector() : std::vector<long>() {}
	NumberVector(const std::initializer_list<long>& list) : std::vector<long>(list) {}
	inline void i_sort() {
		for (int i = 0; i < size() - 1; i++) {
			for (int j = i + 1; j < size(); j++) {
				auto& left = this->operator[](i);
				auto& right = this->operator[](j);
				if (left > right) {
					auto temp = left;
					left = right;
					right = temp;
				}
			}
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
