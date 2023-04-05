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
class Stack {
public:
	struct Node {
		T value;
		Node* next;
		Node* prev;
		Stack* host;
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
			next->prev = nullptr;
		}
		else {
			rear->next = next;
			next->prev = rear;
		}
		rear = next;
		cached_size++;
	}
	_ALWAYS_INLINE_ T pop() {
		if (first == nullptr) GENERATE_TRAP();
		Node* iter = rear;
		T retval = iter->value;
		rear = rear->prev;
		if (rear == nullptr) first = nullptr;
		else rear->next = nullptr;
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
	_ALWAYS_INLINE_ void copy(const Node* from) {
		clear();
		append(from);
	}
public:
	_ALWAYS_INLINE_ unsigned int size() const noexcept { return cached_size; }
	_ALWAYS_INLINE_ bool empty() const noexcept { return !size(); }
	_ALWAYS_INLINE_ void clear() {
		while (!empty()) pop();
	}
	_ALWAYS_INLINE_ Stack& operator=(const Stack& other) {
		copy(other.ptr());
	}
	_ALWAYS_INLINE_ void append(const Stack& other) {
		append(other.ptr());
	}
	_ALWAYS_INLINE_ void copy(const Stack& other) {
		copy(other.ptr());
	}
	Stack() {
		first = nullptr;
		rear = nullptr;
		cached_size = 0;
	}
	~Stack() {
		clear();
	}
	Stack(const Stack& other) {
		append(other.ptr());
	}
};

int main() {
	long n;
	Stack<long> queue;
	do {
		std::cout << "N = ";
		std::cin >> n;
		queue.push(n);
	} while (n != 0);
	Stack<long> copy_queue = queue;
	while (!copy_queue.empty()) {
		std::cout << copy_queue.pop() << " ";
	}
	return 0;
}
