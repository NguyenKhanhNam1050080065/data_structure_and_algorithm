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

#define Math_PI 3.14159265358979323846264338

struct Circle {
	Vector2 origin;
	float radius = 1.0f;

	Circle(const Vector2& origin = Vector2(), const float& radius = 1.0f){
		this->origin = origin; this->radius = radius;
	}
	std::string stringify() const {
		return std::string("(x - (") + std::to_string(origin.x) + "))^2 + (y - (" + std::to_string(origin.y) + "))^2 = " + std::to_string(radius * radius);
	}
	bool is_part_of(const Vector2& point) const {
		float l = point.x - origin.x;
		float r = point.y - origin.y;
		return (l * l) + (r * r) - (radius * radius) == 0.0f;
	}
	float perimeter() const { return 2.0 * radius * Math_PI; }
	float area() const { return radius * radius * Math_PI; }
	bool touch(const Circle& other) const {
		// Chạm nhau
		return origin.distance_to(other.origin) == radius + other.radius;
	}
	bool intersects(const Circle& other) const {
		// Giao nhau
		return origin.distance_to(other.origin) < radius + other.radius;
	}
	bool collide(const Circle& other) const {
		// Một trong hai
		return touch(other) || intersects(other);
	}
};

int main() {
	std::cout << Circle(Vector2(0.0f, 0.0f), 2.0f).intersects(Circle(Vector2(-1.0f, 1.0f), 1.0f));
	return 0;
}
