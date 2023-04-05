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
	Vector<long> vec= { 1, -3, 2, 72, 16, 5 };
	std::cout << vec.find(3) << " " << vec.find(72) << " " << vec.find(-15) << "\n";
	return 0;
}
