#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>

#ifdef _MSC_VER
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif

struct Vector2 {
	float x, y;

	Vector2(const float& x = 0.0f, const float& y = 0.0f) {
		this->x = x; this->y = y;
	}
	Vector2(const Vector2& other) {
		x = other.x; y = other.y;
	}
	inline Vector2 reversed() const {
		return Vector2(-x, -y);
	}
	inline Vector2 inversed() const {
		return Vector2(1.0f / x, 1.0f / y);
	}
	inline float& operator[](const int32_t& idx){
		// Nếu tràn chỉ mục thì dừng chương trình đột ngột
		if (idx < 0 || idx > 1) GENERATE_TRAP();
		// Type punning: mọi đối tượng trong C++ là một khối dữ liệu có tuần tự có con trỏ bắt đầu từ property đầu tiên
		// Bởi vậy nên ta có thể làm được trò này lololol
		return ((float*)((void*)this))[idx];
	}
	inline const float& operator[](const int32_t& idx) const {
		return const_cast<Vector2*>(this)->operator[](idx);
	}
	inline void operator*=(const float& real_number){
		x *= real_number; y *= real_number;
	}
	inline void operator*=(const Vector2& other){
		x *= other.x; y *= other.y;
	}
	inline void operator/=(const float& real_number){
		// Chia cho số thực
		// Nếu bằng 0 => dừng chương trình đột ngột
		if (real_number == 0.0f) GENERATE_TRAP();
		operator*=(1.0f / real_number);
	}
	inline void operator/=(const Vector2& other){
		// Chia cho véc tơ khác
		if (other.x == 0.0f || other.y == 0.0f) GENERATE_TRAP();
		operator*=(other.inversed());
	}
	inline void operator+=(const float& real_number){
		x += real_number; y += real_number;
	}
	inline void operator+=(const Vector2& other){
		x += other.x; y += other.y;
	}
	inline void operator-=(const float& real_number){
		operator+=(-real_number);
	}
	inline void operator-=(const Vector2& other){
		operator+=(other.reversed());
	}
	inline Vector2 operator*(const Vector2& other) const {
		Vector2 re = duplicate();
		re.operator*=(other);
		return re;
	}
	inline Vector2 operator/(const Vector2& other) const {
		Vector2 re = duplicate();
		re.operator/=(other);
		return re;
	}
	inline Vector2 operator+(const Vector2& other) const {
		Vector2 re = duplicate();
		re.operator+=(other);
		return re;
	}
	inline Vector2 operator-(const Vector2& other) const {
		Vector2 re = duplicate();
		re.operator-=(other);
		return re;
	}
	inline float length_squared() const {
		// Độ dài nhưng bình phương
		// Các thủ tục so sánh d8o65 dài véc tơ xài phương thức này sẽ có hiệu năng toy61 hơn
		// do không phải dùng hàm căn bậc
		return (x * x) + (y * y);
	}
	inline float length() const {
		// Độ dài
		return std::sqrt(length_squared());
	}
	inline Vector2 duplicate() const {
		// Sao chép
		return Vector2(*this);
	}
	inline Vector2& normalize() {
		// Đơn vị hóa
		*this /= length();
		return *this;
	}
	inline Vector2 normalized() const {
		// Đơn vị hóa nhưng là phương thức hằng
		return duplicate().normalize();
	}
	inline float dot(const Vector2& against) const {
		// Tích vô hướng
		return (x * against.x) + (y * against.y);
	}
	inline float cross(const Vector2& against) const {
		// Tích có hướng
		return (x * against.x) - (y * against.y);
	}
	inline float distance_squared_to(const Vector2& to) const {
		// Khoảng cách bình phương đến
		// Giải thích ở trên
		return (to - *this).length_squared();
	}
	inline float distance_to(const Vector2& to) const {
		// Khoảng cách đến
		return (to - *this).length();
	}
	inline Vector2 direction_to(const Vector2& to) const {
		// Hướng đến
		return (to - *this).normalize();
	}
	std::string stringify() const {
		return std::string("Vector2(") + std::to_string(x) + ", " + std::to_string(y) + ")";
	}
};

struct Plane2D {
	float a = 0.0, b = 0.0, c = 0.0;
	Plane2D(const float& a = 0.0, const float& b = 0.0, const float& c = 0.0) {
		this->a = a; this->b = b; this->c = c;
	}
	float solve(const Vector2& vec) const {
		return (a * vec.x) + (b * vec.y) + c;
	}
};

int main() {
	std::cout << Plane2D(2.0, 1.0, 0.0).solve(Vector2(1.0, -2.0));
	return 0;
}
