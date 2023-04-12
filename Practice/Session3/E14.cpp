template <class T>
struct BinaryTree {
	T value;
	BinaryTree<T>* left   = nullptr;
	BinaryTree<T>* right  = nullptr;
	BinaryTree<T>* parent = nullptr;
};

typedef BinaryTree<long> IntegerBinaryTree;
