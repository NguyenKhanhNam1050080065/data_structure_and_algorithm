template <class T>
struct LinkedList {
	T value;
	LinkedList<T>* prev = nullptr;
	LinkedList<T>* next = nullptr;
};

typedef LinkedList<long> IntegerLinkedList;
