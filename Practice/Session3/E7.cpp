// Source: https://www.geeksforgeeks.org/heap-sort/

#include <vector>
#include <iostream>
#include <initializer_list>
#include <algorithm>

#define SWAP(m_x, m_y) __swap_tmpl((m_x), (m_y))
template <class T>
inline void __swap_tmpl(T &x, T &y) {
	T aux = x;
	x = y;
	y = aux;
}

class NumberVector : public std::vector<long> {
	static void heapify(std::vector<long>& v, const unsigned int& limit, const unsigned int& i){
		int largest = i;
		int l = 2 * i + 1;
		int r = 2 * i + 2;
		if (l < limit && v[l] > v[largest]) largest = l;
		if (r < limit && v[r] > v[largest]) largest = r;
		if (largest != i){
			SWAP(v[i], v[largest]);
			heapify(v, limit, largest);
		}
	}
	static void heap_sort(std::vector<long>& v){
		for (int i = v.size() / 2 - 1; i >= 0; i--) heapify(v, v.size(), i);
		for (int i = v.size() - 1; i > 0; i--){
			SWAP(v[0], v[i]);
			heapify(v, i, 0);
		}
	}
public:
	NumberVector() : std::vector<long>() {}
	NumberVector(const std::initializer_list<long>& list) : std::vector<long>(list) {}
	inline void q_sort(){
		
	}
	inline void h_sort(){
		heap_sort(*this);
	}
};

int main() {
	NumberVector vec= { 41, 23, 4, 14, 56, 1 };
	vec.h_sort();
	for (const auto& e : vec) {
		std::cout << e << " ";
	}
	return 0;
}
