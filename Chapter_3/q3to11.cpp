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

template <class T>
_ALWAYS_INLINE_ void swap_tmpl(T& x, T& y) {
	T aux = x;
	x = y;
	y = aux;
}

struct ConditionAND {
	static _ALWAYS_INLINE_ bool combine(const bool& left, const bool& right) {
		return left && right;
	}
};

struct ConditionOR {
	static _ALWAYS_INLINE_ bool combine(const bool& left, const bool& right) {
		return left || right;
	}
};

template <class T>
struct CoConditionTRUE {
	static _ALWAYS_INLINE_ bool check(const T& left, const T& right) {
		return true;
	}
};

template <class T>
struct CoConditionFALSE {
	static _ALWAYS_INLINE_ bool check(const T& left, const T& right) {
		return false;
	}
};

template <class T>
struct OddNumberCondition {
	static _ALWAYS_INLINE_ bool check(const T& left, const T& right) {
		if (!left || !right) return false;
		return std::abs(left - right) % 2 == 0 && left % 2 == 1;
	}
};

template <class T>
struct EvenNumberCondition {
	static _ALWAYS_INLINE_ bool check(const T& left, const T& right) {
		if (!left || !right) return false;
		return std::abs(left - right) % 2 == 0 && left % 2 == 0;
	}
};

template <class T>
struct PrimeNumberCondition {
	static _ALWAYS_INLINE_ bool check_single(const T& against) {
		if (against <= 0) return false;
		if (against <= 3) return true;
		double threshold = std::sqrt(against);
		for (uint32_t i = 2; i <= threshold; i++) {
			if (against % i == 0) return false;
		}
		return true;
	}
	static _ALWAYS_INLINE_ bool check(const T& left, const T& right) {
		return check_single(left) && check_single(right);
	}
};

template <class T>
struct PerfectNumberCondition {
	static _ALWAYS_INLINE_ bool check_single(const T& against) {
		static std::vector<T> prime_sum_cache{ 1, 3, 6 };
		static std::vector<T>     prime_cache{ 1, 2, 3 };
		while (against > prime_sum_cache[prime_sum_cache.size() - 1]) {
			T next_prime = prime_cache[prime_cache.size() - 1] + 1;
			while (!PrimeNumberCondition<T>::check_single(next_prime)) {
				next_prime += 1;
			}
			prime_cache.push_back(next_prime);
			prime_sum_cache.push_back(prime_sum_cache[prime_sum_cache.size() - 1] + next_prime);
			if (against == next_prime) return true;
		}
		for (const T& value : prime_sum_cache) {
			if (against == value) return true;
		}
		return false;
	}
	static _ALWAYS_INLINE_ bool check(const T& left, const T& right) {
		return check_single(left) && check_single(right);
	}
};

template <class T>
struct PositiveNumberCondition {
	static _ALWAYS_INLINE_ bool check_single(const T& against) {
		return against > 0;
	}
	static _ALWAYS_INLINE_ bool check(const T& left, const T& right) {
		return check_single(left) && check_single(right);
	}
};
template <class T>
struct NegativeNumberCondition {
	static _ALWAYS_INLINE_ bool check_single(const T& against) {
		return against < 0;
	}
	static _ALWAYS_INLINE_ bool check(const T& left, const T& right) {
		return check_single(left) && check_single(right);
	}
};


template <class T>
struct IntegerSwapperAscend {
	static _ALWAYS_INLINE_ void swap(T& left, T& right) {
		if (left > right) {
			swap_tmpl<T>(left, right);
		}
	}
};

template <class T>
struct IntegerSwapperDescend {
	static _ALWAYS_INLINE_ void swap(T& left, T& right) {
		if (left < right) {
			swap_tmpl<T>(left, right);
		}
	}
};

template <class T>
struct IntegerStringifier {
	static _ALWAYS_INLINE_ std::string stringify(const T& val) {
		return std::to_string(val);
	}
};

class ConditionalSort {
public:
	ConditionalSort() = delete;
	template <class T, class Condition, class Swapper, class ConditionCombine = ConditionAND, class CoCondition = CoConditionTRUE<T>>
	static _ALWAYS_INLINE_ void sort(T* array, uint32_t size) {
		for (uint32_t i = 0; i < size; i++) {
			for (uint32_t j = size - 1; j >= i + 1; j--) {
				T& left = array[i];
				T& right = array[j];
				if (ConditionCombine::combine(Condition::check(left, right), CoCondition::check(left, right)))
					Swapper::swap(left, right);
			}
		}
	}
	template <class T, class Stringifier = IntegerStringifier<T>>
	static _ALWAYS_INLINE_ std::string output(const T* array, const uint32_t& size) {
		std::string re = "";
		for (uint32_t i = 0; i < size; i++) {
			re += Stringifier::stringify(array[i]) + " ";
		}
		re += "\n";
		return re;
	}
};

template <class T>
void input_vec(std::vector<T>& vec){
	uint32_t n;
	do {
		std::cout << "N = ";
		std::cin >> n;
	} while (n == 0);
	vec.resize(n);
	for (uint32_t i = 0; i < n; i++) {
		std::cout << "A[" << i << "] = ";
		std::cin >> vec[i];
	}
}

int main() {
	std::vector<long> vec, copy;
	int n = vec.size();
	input_vec<long>(vec);

	//2---------------------------------------------------------------------------------------------
	copy = vec;
	std::cout << "Ascending odd numbers: ";
	ConditionalSort::sort<long, OddNumberCondition<long>, IntegerSwapperAscend<long>>(&copy[0], n);
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//4---------------------------------------------------------------------------------------------
	copy = vec;
	std::cout << "Ascending prime numbers: ";
	ConditionalSort::sort<long, PrimeNumberCondition<long>, IntegerSwapperAscend<long>>(&copy[0], n);
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//5---------------------------------------------------------------------------------------------
	copy = vec;
	std::cout << "Descending perfect numbers: ";
	ConditionalSort::sort<long, PerfectNumberCondition<long>, IntegerSwapperDescend<long>>(&copy[0], n);
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//7---------------------------------------------------------------------------------------------
	copy = vec;
	std::cout << "Ascending positive numbers: ";
	ConditionalSort::sort<long, PositiveNumberCondition<long>, IntegerSwapperAscend<long>>(&copy[0], n);
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//8---------------------------------------------------------------------------------------------
	copy = vec;
	std::cout << "Ascending odd/even numbers: ";
	ConditionalSort::sort<long, OddNumberCondition<long>, IntegerSwapperAscend<long>, ConditionOR, EvenNumberCondition<long>>(&copy[0], n);
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//9---------------------------------------------------------------------------------------------
	copy = vec;
	std::cout << "Ascending positive numbers & Descending negative number: ";
	ConditionalSort::sort<long, PositiveNumberCondition<long>, IntegerSwapperAscend<long>>(&copy[0], n);
	ConditionalSort::sort<long, NegativeNumberCondition<long>, IntegerSwapperDescend<long>>(&copy[0], n);
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//----------------------------------------------------------------------------------------------
	std::cout << "--------------------------------------------------------------------------------------------\n";
	std::vector<long> slice1, slice2;
	std::vector<long> combined;
	input_vec<long>(slice1); input_vec<long>(slice2);
	n = slice1.size() + slice2.size();
	combined.clear();
	combined.insert(combined.end(), slice1.begin(), slice1.end());
	combined.insert(combined.end(), slice2.begin(), slice2.end());

	copy = vec;
	ConditionalSort::sort<long, CoConditionTRUE<long>, IntegerSwapperAscend<long>>(&copy[0], n);
	std::cout << "Combined and sorted (ascending): ";
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//10--------------------------------------------------------------------------------------------
	copy = vec;
	ConditionalSort::sort<long, CoConditionTRUE<long>, IntegerSwapperDescend<long>>(&copy[0], n);
	std::cout << "Combined and sorted (ascending): ";
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//11--------------------------------------------------------------------------------------------
	copy = vec;
	ConditionalSort::sort<long, CoConditionTRUE<long>, IntegerSwapperDescend<long>>(&copy[0], n);
	std::cout << "Combined and sorted (descending): ";
	std::cout << ConditionalSort::output<long, IntegerStringifier<long>>(copy.data(), n);
	//----------------------------------------------------------------------------------------------
	
	return 0;
}
