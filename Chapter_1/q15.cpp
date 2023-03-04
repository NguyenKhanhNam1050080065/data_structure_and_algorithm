#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>

class MixedNumber {
public:
	static int32_t gcd(const int32_t& a, const int32_t& b) {
		if (a == 0)
			return b;
		else if (b == 0)
			return a;
		if (a < b)
			return gcd(a, b % a);
		else
			return gcd(b, a % b);
	}
private:
	int32_t num, den, in;
	void internal_shorten() {
		float real = float(num) / den;
		int32_t dir = int32_t(std::abs(real) / real);
		in = int32_t(std::floor(std::abs(real))) * dir;
		num -= den * in;
		int32_t _gcd = gcd(std::abs(num), std::abs(den));
		num /= _gcd;
		//num *= dir;
		den /= _gcd;
	}
	void unpack_numerator() {
		num += in * den;
	}
public:
	MixedNumber(const int32_t& numerator = 0, const int32_t& denumerator = 0, const int32_t& integral = 0) {
		num = numerator; den = denumerator; in = integral;
		unpack_numerator();
		in = 0;
	}
	MixedNumber(const MixedNumber& other) : MixedNumber(other.num, other.den, other.in) {}
	~MixedNumber() {}
	inline MixedNumber duplicate() const {
		return MixedNumber(*this);
	}
	inline MixedNumber& reverse() {
		// unpack_numerator();
		num = -num;
		// shorten();
		return *this;
	}
	inline MixedNumber reversed() const {
		return duplicate().reverse();
	}
	inline MixedNumber& inverse() {
		unpack_numerator();
		int32_t temp = num;
		num = den;
		den = temp;
		in = 0;
	}
	inline MixedNumber inversed() const {
		return duplicate().inverse();
	}
	inline int32_t numerator() const { return num; }
	inline void set_numerator(const int32_t& n) {
		num = n;
		unpack_numerator();
		in = 0;
	}
	inline int32_t denumerator() { return den; }
	inline void set_denumerator(const int32_t& d) {
		den = d;
		unpack_numerator();
		in = 0;
	}
	inline int32_t& integral() { return in; }
	inline const int32_t& integral() const { return in; }

	inline MixedNumber& convert(const double& input) {
		constexpr int32_t precision = 1'000'000'000; // Độ chính xác
		in = int32_t(std::floor(input));
		double frac = input - in;


		int32_t gcd_ = gcd(std::round(frac * precision), precision);

		den = precision / gcd_;
		num = std::round(frac * precision) / gcd_;
		return *this;
	}
	inline MixedNumber& shorten() {
		internal_shorten();
		return *this;
	}
	inline MixedNumber shortened() const {
		return duplicate().shorten();
	}
	inline bool is_negative() const { return float(num + (den * in)) / den < 0.0f; }
	inline bool is_positive() const { return float(num + (den * in)) / den > 0.0f; }
	inline bool is_zero() const { return !is_negative() && !is_positive(); }
	inline MixedNumber abs() const {
		MixedNumber dup = duplicate();
		dup.num = std::abs(num);
		dup.den = std::abs(den);
		dup.in = std::abs(in);
		return dup;
	}
	inline char compare(const MixedNumber& other) const {
		MixedNumber sub = other - *this;
		if (sub.is_negative()) return 1;
		else if (sub.is_positive()) return -1;
		return 0;
	}
	inline bool operator==(const MixedNumber& other) const {
		return num == other.num && den == other.den && in == other.in;
	}
	inline bool operator!=(const MixedNumber& other) const {
		return !(*this == other);
	}
	inline bool operator>(const MixedNumber& other) const {
		return compare(other) == 1;
	}
	inline bool operator>=(const MixedNumber& other) const {
		return compare(other) == 1 || compare(other) == 0;
	}
	inline bool operator<(const MixedNumber& other) const {
		return compare(other) == -1;
	}
	inline bool operator<=(const MixedNumber& other) const {
		return compare(other) == -1 || compare(other) == 0;
	}
	inline void operator*=(const MixedNumber& other) {
		unpack_numerator();
		MixedNumber right = other.duplicate();
		right.num += right.in * right.den;
		num *= right.num;
		den *= right.den;
		in = 0;
	}
	inline void operator*=(const int32_t& other) {
		*this *= MixedNumber(other, other);
	}
	inline void operator/=(const MixedNumber& other) {
		*this *= MixedNumber(other.den, other.num);
	}
	inline void operator/=(const int32_t& other) {
		*this *= MixedNumber(1, other);
	}
	inline void operator+=(const MixedNumber& other) {
		int32_t left_den  = other.den;
		int32_t right_den = this->den;
		*this *= left_den;
		MixedNumber right = other.duplicate();
		right *= right_den;
		in += right.in;
		num += right.num;
		unpack_numerator();
		in = 0;
	}
	inline void operator+=(const int32_t& other) {
		*this += MixedNumber(other, 1);
	}
	inline void operator-=(const MixedNumber& other) {
		*this += other.reversed();
	}
	inline void operator-=(const int32_t& other) {
		*this += (-other);
	}
	inline MixedNumber& operator++() {
		*this += MixedNumber(1, den);
	}
	inline MixedNumber& operator--() {
		*this -= MixedNumber(1, den);
	}
	inline MixedNumber operator*(const MixedNumber& other) const {
		MixedNumber dup = duplicate();
		dup *= other;
		return dup;
	}
	inline MixedNumber operator*(const int32_t& other) const {
		MixedNumber dup = duplicate();
		dup *= other;
		return dup;
	}
	inline MixedNumber operator/(const MixedNumber& other) const {
		MixedNumber dup = duplicate();
		dup /= other;
		return dup;
	}
	inline MixedNumber operator/(const int32_t& other) const {
		MixedNumber dup = duplicate();
		dup /= other;
		return dup;
	}
	inline MixedNumber operator+(const MixedNumber& other) const {
		MixedNumber dup = duplicate();
		dup += other;
		return dup;
	}
	inline MixedNumber operator+(const int32_t& other) const {
		MixedNumber dup = duplicate();
		dup += other;
		return dup;
	}
	inline MixedNumber operator-(const MixedNumber& other) const {
		MixedNumber dup = duplicate();
		dup -= other;
		return dup;
	}
	inline MixedNumber operator-(const int32_t& other) const {
		MixedNumber dup = duplicate();
		dup -= other;
		return dup;
	}
	inline std::string stringify() const {
		return std::to_string(in) + " + (" + std::to_string(num) + "/" + std::to_string(den) + ")";
	}
};

int main() {
	MixedNumber a(1, 2), b(4, 5), c(1, 3), d(9, 7);
	std::cout << (a + d).shorten().stringify() << "\n";
	std::cout << (a - b).shorten().stringify() << "\n";
	std::cout << (a * c).shorten().stringify() << "\n";
	std::cout << (b / d).shorten().stringify() << "\n";
	std::cout << (a + a).shorten().stringify() << "\n";
	std::cout << (a / a).shorten().stringify() << "\n";
	std::cout << (a == a) << "\n";
	std::cout << (a >  a) << "\n";
	std::cout << (a >= a) << "\n";
	std::cout << (b >= a) << "\n";
	std::cout << (b <  a) << "\n";
	std::cout << (b >  a) << "\n";
	std::cout << (b < a.reversed()) << "\n";
	return 0;
}
