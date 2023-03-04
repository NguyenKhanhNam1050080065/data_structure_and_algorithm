#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>

class Exponent {
private:
	uint64_t cached;
	uint32_t a, n;
public:
	static uint64_t exp(const uint32_t& a, const uint32_t& n){
		uint64_t accumulate = 1;
		if (!n) return accumulate;
		accumulate = a;
		for (uint32_t i = 1; i < n; i++){
			accumulate *= a;
		}
		return accumulate;
	}
	Exponent(const uint32_t& a = 0, const uint32_t& n = 1){
		this->a = a; this->n = n; cached = exp(a, n);
	}
	inline uint64_t get_exp() const { return cached; }
	inline void raise() {
		n += 1;
		cached *= a;
	}
	inline void lower() {
		if (n == 0) return;
		n -= 1;
		cached /= a;
	}
	void set_a(const uint32_t& a){
		this->a = a; cached = exp(a, n);
	}
	void set_n(const uint32_t& n){
		this->n = n; cached = exp(a, n);
	}
};

int main() {
	std::cout << Exponent(3, 4).get_exp();
	return 0;
}
