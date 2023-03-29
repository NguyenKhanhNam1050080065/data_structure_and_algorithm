#include <iostream>

#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline
#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

#ifdef _MSC_VER
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif

template <class T>
class Queue {
public:
	struct Node {
		T value;
		Node* next;
		Queue* host;
	};
private:
	Node* first;
	Node* rear;
	unsigned int cached_size;
public:
	_ALWAYS_INLINE_ void push(const T& value) {
		Node* next = new Node();
		next->next = nullptr;
		next->value = value;
		next->host = this;
		if (first == nullptr) {
			first = next;
			rear = next;
		}
		else {
			rear->next = next;
			rear = next;
		}
		cached_size++;
	}
	_ALWAYS_INLINE_ T pop() {
		if (first == nullptr) GENERATE_TRAP();
		Node* iter = first;
		T retval = iter->value;
		first = first->next;
		if (first == nullptr) rear = nullptr;
		delete iter;
		cached_size--;
		return retval;
	}
private:
	_ALWAYS_INLINE_ const Node* ptr() const noexcept { return first; }
	_ALWAYS_INLINE_ void append(const Node* from) {
		if (from->host == this) return;
		Node* iter = const_cast<Node*>(from);
		while (iter) {
			push(iter->value);
			iter = iter->next;
		}
	}
	_ALWAYS_INLINE_ void copy(const Node* from){
		clear();
		append(from);
	}
public:
	_ALWAYS_INLINE_ unsigned int size() const noexcept { return cached_size; }
	_ALWAYS_INLINE_ bool empty() const noexcept { return !size(); }
	_ALWAYS_INLINE_ void clear() {
		while (!empty()) pop();
	}
	_ALWAYS_INLINE_ Queue& operator=(const Queue& other) {
		copy(other.ptr());
	}
	_ALWAYS_INLINE_ void append(const Queue& other){
		append(other.ptr());
	}
	_ALWAYS_INLINE_ void copy(const Queue& other){
		copy(other.ptr());
	}
	Queue() {
		first = nullptr;
		rear = nullptr;
		cached_size = 0;
	}
	~Queue() {
		clear();
	}
	Queue(const Queue& other) {
		append(other.ptr());
	}
};

int main() {
	long n;
	Queue<long> queue;
	do {
		std::cout << "N = ";
		std::cin >> n;
		queue.push(n);
	} while (n != 0);
	Queue<long> copy_queue = queue;
	while (!copy_queue.empty()) {
		std::cout << copy_queue.pop() << " ";
	}
	return 0;
}
