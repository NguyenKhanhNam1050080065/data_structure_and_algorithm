#include <vector>
#include <iostream>
#include <initializer_list>

template <class T>
class Vector : public std::vector<T> {
public:
	Vector() : std::vector<T>() {}
	Vector(std::initializer_list<T> l) : std::vector<T>(l) {}
	inline int find(const T& what) const {
		for (int i = 0; i < this->size(); i++){
			if (this->operator[](i) == what) return i;
		}
		return -1;
	}
};

int main() {
	Vector<long> vec= { 2, 3, 4, 10, 40 };
	for (const auto& e : vec) {
		std::cout << e << " ";
	}
	std::cout << "\n" << vec.find(10) << "\n";
	return 0;
}
