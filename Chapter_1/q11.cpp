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
	inline bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
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

struct Triangle {
	Vector2 a, b, c;
	Triangle(const Vector2& a = Vector2(), const Vector2& b = Vector2(), const Vector2& c = Vector2()){
		// Nếu hai trong 3 điểm bằng nhau thì kết thúc
		if (a == b || b == c || c == a) GENERATE_TRAP();
		this->a = a; this->b = b; this->c = c;
	}
	Vector2 centroid() const {
		// Trọng tâm
		return Vector2((a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f);
	}
	float perimeter() const {
		return a.distance_to(b) + b.distance_to(c) + c.distance_to(a);
	}
	float area(/*const bool& use_heron = true*/) const {
		/*if (use_heron){*/
			// Tính diện tích bằng công thức Hê-rông
			float la = a.distance_to(b);
			float lb = b.distance_to(c);
			float lc = c.distance_to(a);
			float p = (la + lb + lc) / 2.0f;
			return std::sqrt(p * (p - la) * (p - lb) * (p - lc));
		//}
		// return (b - a).cross(c - a);
	}
	std::string stringify() const {
		return std::string("Triangle(") + a.stringify() + ", " + b.stringify() + ", " + c.stringify() + ")";
	}
};

int main() {
	std::cout << Triangle(Vector2(0.0, 0.0), Vector2(1.3, -2.4), Vector2(4.6, 0.2)).area();
	return 0;
}
