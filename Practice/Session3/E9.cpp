#include <iostream>
#include <initializer_list>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif

template <class T, unsigned int SIZE = 16>
class QueueArray {
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
		T re = array[0];
		const size_t mem_size = (SIZE - 1) * sizeof(T);
		T* buffer = (T*)malloc(mem_size);
		memcpy(buffer, &array[1], mem_size);
		memcpy(&array[0], buffer, mem_size);
		free(buffer);
		size_cache--;
		return re;
	}
	unsigned int size() const { return size_cache; }
	bool empty() const { return !size(); }
	const T* ptr() const { return array; }
	QueueArray(){
		
	}
	QueueArray(const std::initializer_list<T>& i_list){
		for (const auto& v : i_list)
			push(v);
	}
};

template <class T>
void output_stack(const QueueArray<T>& queue){
	for (unsigned int i = 0; i < queue.size(); i++){
		std::cout << queue.ptr()[i] << " ";
	}
	std::cout << "\n";
}

int main() {
	QueueArray<long> queue = { 41, 23, 4, 14, 56, 1 };
	output_stack<long>(queue);
	queue.push(55);
	output_stack<long>(queue);
	while (queue.pop() != 23) {}
	output_stack<long>(queue);
	return 0;
}
