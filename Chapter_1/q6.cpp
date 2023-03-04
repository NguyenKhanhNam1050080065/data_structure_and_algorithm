#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include <vector>

class Accumulator {
private:
	mutable std::vector<uint32_t> cached_products;
	mutable std::vector<uint32_t> cached_exponents;
	mutable uint32_t last_x;
	uint32_t a, n;

	void build_products() const {
		cached_products.clear();
		cached_products.resize(n + 1);
		for (uint32_t i = 0; i <= n; i++){
			cached_products[i] = i * a;
		}
	}
	void build_exponent(const uint32_t& x) const {
		cached_exponents.clear();
		cached_exponents.resize(n + 1);
		cached_exponents[0] = 1;
		last_x = x;
		if (!n) return;
		cached_exponents[1] = x;
		for (uint32_t i = 2; i <= n; i++){
			cached_exponents[i] = cached_exponents[i - 1] * x;
		}
	}
public:
	Accumulator(const uint32_t& a = 0, const uint32_t& n = 0){
		this->a = a; this->n = n; last_x = 0;
		build_products();
	}
	uint32_t get_a() const { return a; }
	void set_a(const uint32_t& a){
		if (this->a == a) return;
		this->a = a;
		build_products();
	}
	uint32_t get_n() const { return n; }
	void set_n(const uint32_t& n){
		if (this->n == n) return;
		this->n = n;
		build_products();
	}
	uint64_t calculate(const uint32_t& x) const {
		// Quy hoạch động và caching
		if (x != last_x) build_exponent(x);
		uint64_t re = 0;
		if (!n) return re;
		for (uint32_t i = 0; i <= n; i++){
			re += cached_exponents[i] * cached_products[i];
		}
		return re;
	}
};

int main() {
	std::cout << Accumulator(1, 3).calculate(2);
	return 0;
}
