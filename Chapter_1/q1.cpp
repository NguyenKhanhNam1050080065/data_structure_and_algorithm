#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>

class Fraction {
	uint32_t num, den, in;
public:
	static uint32_t gcd(const uint32_t& a, const uint32_t& b) {
		if (a == 0)
			return b;
		else if (b == 0)
			return a;
		if (a < b)
			return gcd(a, b % a);
		else
			return gcd(b, a % b);
	}
	uint32_t& numerator() { return num; }
	const uint32_t& numerator() const { return num; }
	uint32_t& denumerator() { return den; }
	const uint32_t& denumerator() const { return den; }
	uint32_t& integral() { return in; }
	const uint32_t& integral() const { return in; }
	Fraction(const uint32_t& numerator = 0, const uint32_t& denumerator = 0, const uint32_t& integral = 0){
		num = numerator; den = denumerator; in = integral;
	}
	~Fraction(){}
	Fraction& convert(const double& input) {
		constexpr uint32_t precision = 1'000'000'000; // Độ chính xác
		in = uint32_t(std::floor(input));
		double frac = input - in;
	
	
		uint32_t gcd_ = gcd(std::round(frac * precision), precision);
	
		den = precision / gcd_;
		num = std::round(frac * precision) / gcd_;
		return *this;
	}
	std::string stringify() const {
		return std::to_string(in) + " + (" + std::to_string(num) + "/" + std::to_string(den) + ")";
	}
};

int main() {
	std::cout << Fraction().convert(3.1415926535897932).stringify();

	return 0;
}