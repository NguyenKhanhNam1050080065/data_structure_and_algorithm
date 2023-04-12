#include <iostream>
#include <initializer_list>
#include <cstdlib>

#ifdef _MSC_VER
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif

template <class T, unsigned int SIZE = 16>
class StackArray {
private:
	unsigned int size_cache = 0;
	T array[SIZE];
public:
	void push(const T& val){
		if (size_cache >= SIZE) GENERATE_TRAP();
		array[size_cache++] = val;
	}
	T pop(){
		if (size_cache == 0) GENERATE_TRAP();
		return array[--size_cache];
	}
	unsigned int size() const { return size_cache; }
	bool empty() const { return !size(); }
	StackArray(){
		
	}
	StackArray(const std::initializer_list<T>& i_list){
		for (const auto& v : i_list)
			push(v);
	}
};

int main() {
	StackArray<long> stack = { 41, 23, 4, 14, 56, 1 };
	while (!stack.empty()) std::cout << stack.pop() << " ";
	return 0;
}
