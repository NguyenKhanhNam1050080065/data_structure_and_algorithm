#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline
#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

static _ALWAYS_INLINE_ void vec_input(std::vector<long>& vec){
	unsigned int n;
	do {
		std::cout << "N = ";
		std::cin >> n;
	} while (n == 0);
	vec.resize(n);
	for (uint32_t i = 0; i < n; i++){
		std::cout << "vec[" << i << "] =";
		std:: cin >> vec[i];
	}
	return;
}

int main() {
	std::vector<long> vec1, vec2;
	std::cout << "First vector:\n";
	vec_input(vec1);
	std::cout << "Second vector:\n";
	vec_input(vec2);
	if (vec1.size() != vec2.size()) {
		std::cout << "vec1 is not a reflection of vec2\n"; return 0;
	}
	for (int64_t i = 0; i < vec1.size(); i++){
		for (int64_t j = vec1.size() - 1; j >= 0; j++){
			if (vec1[i] != vec2[j]) { std::cout << "vec1 is not a reflection of vec2\n"; return; }
		}
	}
	std::cout << "vec1 is a reflection of vec2\n";
	return 0;
}
