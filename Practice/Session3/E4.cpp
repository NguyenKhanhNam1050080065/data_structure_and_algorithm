//Source: https://www.geeksforgeeks.org/selection-sort/
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
	static void selection_sort(std::vector<long>& v){
		int i, j, min_idx;
		for (i = 0; i < v.size() - 1; i++)
		{
			min_idx = i;
			for (j = i + 1; j < v.size(); j++)
			{
				if (v[j] < v[min_idx])
					min_idx = j;
			}
			if (min_idx!=i)
				SWAP(v[min_idx], v[i]);
		}
	}
public:
	NumberVector() : std::vector<long>() {}
	NumberVector(const std::initializer_list<long>& list) : std::vector<long>(list) {}
	inline void s_sort() {
		selection_sort(*this);
	}
};

int main() {
	NumberVector vec= { 41, 23, 4, 14, 56, 1 };
	vec.s_sort();
	for (const auto& e : vec) {
		std::cout << e << " ";
	}
	return 0;
}
