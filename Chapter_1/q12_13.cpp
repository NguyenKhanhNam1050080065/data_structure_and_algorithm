#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include <vector>

#ifdef _MSC_VER
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif

struct Monomial {
	float real_number;
	int32_t exponent;

	inline Monomial& copy(const Monomial& other) {
		real_number = other.real_number;
		exponent = other.exponent;
		return *this;
	}
	Monomial(const float& r = 0.0f, const int32_t& e = 0.0f) {
		real_number = r;
		exponent = e;
	}
	Monomial(const Monomial& other) {
		copy(other);
	}
	inline Monomial duplicate() const {
		return Monomial(*this);
	}
	inline Monomial& inverse() {
		if (real_number == 0.0f) GENERATE_TRAP();
		real_number = 1.0f / real_number;
		exponent = -exponent;
		return *this;
	}
	inline Monomial inversed() const {
		return duplicate().inverse();
	}
	inline Monomial& reverse() {
		real_number = -real_number;
		return *this;
	}
	inline Monomial reversed() const {
		return duplicate().reverse();
	}
	inline Monomial& operator=(const Monomial& other) {
		return copy(other);
	}
	inline void operator+=(const Monomial& other) {
		if (exponent != other.exponent) GENERATE_TRAP();
		real_number += other.real_number;
	}
	inline void operator-=(const Monomial& other) {
		*this += other.reversed();
	}
	inline void operator*=(const Monomial& other) {
		real_number *= other.real_number;
		exponent += other.exponent;
	}
	inline void operator/=(const Monomial& other) {
		*this *= other.inversed();
	}
	inline Monomial operator+(const Monomial& other) const {
		Monomial dup = duplicate();
		dup += other;
		return dup;
	}
	inline Monomial operator-(const Monomial& other) const {
		Monomial dup = duplicate();
		dup -= other;
		return dup;
	}
	inline Monomial operator*(const Monomial& other) const {
		Monomial dup = duplicate();
		dup *= other;
		return dup;
	}
	inline Monomial operator/(const Monomial& other) const {
		Monomial dup = duplicate();
		dup /= other;
		return dup;
	}
	float solve(const float& x) const {
		// Giải đơn thức
		return std::pow(x, float(exponent)) * real_number;
	}
	static float custom_derivation(const float& x, const float& a, float n, uint8_t iteration) {
		float re;
		if (x != 0.0f) {
			re = n;
		}
		else return 0.0f;
		n -= 1.0f;
		n = n > 0.0f ? n : 0.0f;
		if (n == 0.0f) {
			iteration = 0;
		}
		re *= std::pow(x, n) * a;
		if (!iteration) return re;
		return custom_derivation(re, a, n, iteration - 1);
	}
	float derive(const float& x, const uint8_t& iteration = 0) const {
		// Đạo hàm
		return custom_derivation(x, real_number, float(exponent), iteration);
	}
	std::string stringify() const {
		return std::string("Monomial(") + std::to_string(real_number) + " * x^(" + std::to_string(exponent) + "))";
	}
};

class Polymial {
private:
	std::vector<Monomial> monomials;
public:
	Polymial() {
		monomials.push_back(Monomial(0.0, 0));
	}
	Polymial(const Polymial& other) {
		monomials = other.monomials;
	}
	inline uint32_t length() const { return monomials.size(); }
	Polymial& add_monomial(const Monomial& mono) {
		// Thêm đơn thức
		// Do không thể nào chính xác chuyển số nguyên có dấu
		// => số nguyên không dấu (trần khác nhau) nên
		// sử dụng type punning để lấy chỉ mục
		uint32_t index = *(uint32_t*)(&mono.exponent);
		if (index >= monomials.size()) {
			monomials.resize(index + 1);
		}
		// Nếu là phần tử mới hoặc phần mũ bằng 0
		// thì cho phép set giá trị trực tiếp
		if (monomials[index].exponent == 0) {
			if (monomials[index].real_number != 0.0f) monomials[index] += mono;
			else monomials[index] = mono;
		}
		else {
			monomials[index] += mono;
		}
		return *this;
	}
	Polymial& remove_monomial(const Monomial& mono) {
		uint32_t index = *(uint32_t*)(&mono.exponent);
		if (index >= monomials.size()) {
			monomials.resize(index + 1);
		}
		if (monomials[index].exponent == 0) {
			if (monomials[index].real_number != 0.0f) monomials[index] -= mono;
			else monomials[index] = mono.inversed();
		}
		else {
			monomials[index] -= mono;
		}
		return *this;
	}
	Polymial duplicate() const {
		return Polymial(*this);
	}
	Polymial operator+(const Polymial& other) const {
		Polymial dup = length() > other.length() ? duplicate() : other.duplicate();
		const Polymial& copy_from = length() <= other.length() ? *this : other;
		for (const Monomial& mono : copy_from.monomials) {
			dup.add_monomial(mono);
		}
		return dup;
	}
	Polymial operator-(const Polymial& other) const {
		Polymial dup = length() > other.length() ? duplicate() : other.duplicate();
		const Polymial& copy_from = length() <= other.length() ? *this : other;
		for (const Monomial& mono : copy_from.monomials) {
			dup.remove_monomial(mono);
		}
		return dup;
	}
	float derive(const float& x, const uint8_t& iteration = 0) const {
		float re = 0.0f;
		Polymial dup = duplicate();
		for (Monomial& mono : dup.monomials) {
			if (mono.real_number == 0.0f) continue;
			re += mono.derive(x, iteration);
		}
		return re;
	}
	float solve(const float& x) const {
		float re = 0.0f;
		const Polymial& dup = *this;
		for (const Monomial& mono : dup.monomials) {
			if (mono.real_number == 0.0f) continue;
			re += mono.solve(x);
		}
		return re;
	}
	std::string stringify() const {
		std::string re = "Polynomial(";
		const Monomial& mono = monomials[length() - 1];
		re += "(" + std::to_string(mono.real_number) + " * x^(" + std::to_string(mono.exponent) + "))";
		for (int32_t i = length() - 2; i >= 0; i--) {
			const Monomial& mono = monomials[i];
			re += " + (" + std::to_string(mono.real_number) + " * x^(" + std::to_string(mono.exponent) + "))";
		}
		return re + ")";
	}
};

int main() {
	std::cout << (Monomial(2.0, 3.0) * Monomial(3.0, 2.0)).solve(1.0) << "\n";
	std::cout << Polymial().add_monomial(Monomial(3.0, 1)).add_monomial(Monomial(-2.0, 2)).solve(2.0) << "\n";
	std::cout << Polymial().add_monomial(Monomial(3.0, 1)).add_monomial(Monomial(-2.0, 2)).derive(2.0, 2) << "\n";

	Polymial left; left.add_monomial(Monomial(-1.0, 2));
	Polymial right; right.add_monomial(Monomial(3.0, 1)).add_monomial(Monomial(-1.0, 2));
	std::cout << (left + right).solve(2.0) << "\n";
	std::cout << (left + right).stringify() << "\n";
	return 0;
}
