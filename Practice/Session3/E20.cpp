#include <vector>
#include <iostream>
#include <initializer_list>

#define SWAP(m_x, m_y) __swap_tmpl((m_x), (m_y))
template <class T>
inline void __swap_tmpl(T &x, T &y) {
	T aux = x;
	x = y;
	y = aux;
}

class NumberVector : public std::vector<long> {
	static void quicksort(std::vector<long>& v, const int& low, const int& high){
		if (low < high){
			long pivot = v[high];
			int i = low - 1;
			for (int j = low; j < high; j++){
				if (v[j] < pivot){
					i += 1;

					SWAP(v[i], v[j]);
				}
			}
			SWAP(v[i + 1], v[high]);
			int pivot_idx = i + 1;
			quicksort(v, low, pivot_idx - 1);
			quicksort(v, pivot_idx + 1, high);
		}
	}
public:
	NumberVector() : std::vector<long>() {}
	NumberVector(const std::initializer_list<long>& list) : std::vector<long>(list) {}
	inline void q_sort(){
		quicksort(*this, 0, size() - 1);
	}
};

int main() {
	NumberVector vec= { 1, -3, 2, 72, 16, 5 };
	vec.q_sort();
	for (const auto& e : vec) {
		std::cout << e << " ";
	}
	return 0;
}
