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
	inline float angle_to(const Vector2& normalized_to) const {
		// Góc đến một véc tơ đơn vị
		return std::atan2f(cross(normalized_to), dot(normalized_to));
	}
	std::string stringify() const {
		return std::string("(") + std::to_string(x) + ", " + std::to_string(y) + ")";
	}
};

#define Math_PI 3.14159265358979323846264338
#define Math_HalfPI 0.5 * Math_PI
#define Math_2PI 2.0 * Math_PI
#define CMP_EPSILON 0.00001

#define MAX_VEC2(axis, v1, v2) (v1.axis > v2.axis ? v1 : v2)
#define PYTHAGORAS(v0, v1, v2) is_equal_approx((v0 * v0) + (v1 * v1), (v2 * v2))

static inline bool is_equal_approx(double a, double b) {
	// Do việc sử dụng atan2f và macro Math_PI, không thể nào đo đạc chính xác được 2 số thực có bằng nhau hay không
	// bởi vậy nên ta ước lượng
	if (a == b) {
		return true;
	}
	double tolerance = CMP_EPSILON * std::abs(a);
	if (tolerance < CMP_EPSILON) {
		tolerance = CMP_EPSILON;
	}
	return std::abs(a - b) < tolerance;
}

struct Triangle {
	enum Type : unsigned int {
		INVALID = 0U,		// Không hợp lệ
		REGULAR = 1U,		// Thường
		ISOSCELES = 2U,		// Cân
		PERPENDICULAR = 4U,	// Vuông
		EQUILATERAL = 8U,	// Đều
	};
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

	Vector2 biggest_x() const {
		// Hoành độ lớn nhất
		return MAX_VEC2(x, MAX_VEC2(x, b, c), a);
	}
	Vector2 biggest_y() const {
		// Tung độ lớn nhất
		return MAX_VEC2(y, MAX_VEC2(y, b, c), a);
	}
	float total_length_from(const Vector2& against) const {
		// Tổng khoảng cách từ "against" đến 3 đỉnh tam giác
		return against.distance_to(a) + against.distance_to(b) + against.distance_to(c);
	}
	bool is_triangle_valid() const {
		// Bất đẳng thức tam giác
		return a.distance_to(b) + b.distance_to(c) > c.distance_to(a);
	}
	uint32_t type() const {
		if (!is_triangle_valid()) return uint32_t(INVALID);
		uint32_t re = REGULAR;
		float l_a = a.distance_to(b);
		float l_b = b.distance_to(c);
		float l_c = c.distance_to(a);
		if (is_equal_approx(l_a, l_b) && is_equal_approx(l_b, l_c)){
			re += EQUILATERAL;
			return re;
		}
		double corner = 0.0;
		if (is_equal_approx(l_a, l_b)){
			re += ISOSCELES;
		} else if (is_equal_approx(l_b, l_c)){
			re += ISOSCELES;
		} else if (is_equal_approx(l_c, l_a)){
			re += ISOSCELES;
		}
		if (PYTHAGORAS(l_a, l_b, l_c) || PYTHAGORAS(l_b, l_c, l_a) || PYTHAGORAS(l_c, l_a, l_b)) re += PERPENDICULAR;
		return re;
	}
	static inline void translate(const uint32_t& types){
		if (types == INVALID){
			// Không hợp lệ
			std::cout << "This triangle is invalid\n"; return;
		}
		// Hợp lệ
		std::cout << "This triangle is valid";
		// Đều
		if (types & EQUILATERAL) std::cout << ", equilateral";
		// Cân
		if (types & ISOSCELES) std::cout << ", isosceles";
		// Vuông
		if (types & PERPENDICULAR) std::cout << ", perpendicular";
		// Vuông và Cân có thể xảy ra cùng lúc
		std::cout << "\n";
	}
	float perimeter() const {
		return a.distance_to(b) + b.distance_to(c) + c.distance_to(a);
	}
	float area() const {
		float la = a.distance_to(b);
		float lb = b.distance_to(c);
		float lc = c.distance_to(a);
		float p = (la + lb + lc) / 2.0f;
		return std::sqrt(p * (p - la) * (p - lb) * (p - lc));
	}
	std::string stringify() const {
		return std::string("Triangle(") + a.stringify() + ", " + b.stringify() + ", " + c.stringify() + ")";
	}
};

#define SQRT_0_75 0.866025403784
#define GENERAL_LENGTH 2.0

int main() {
	// Tam giác cân
	Triangle::translate(Triangle(Vector2(-1.0, 0.0), Vector2(1.0, 0.0), Vector2(0.0, 2.0)).type());
	// Tam giác đều
	Triangle::translate(Triangle(Vector2(0.0, 0.0), Vector2(GENERAL_LENGTH, 0.0), Vector2(GENERAL_LENGTH / 2.0, GENERAL_LENGTH * SQRT_0_75)).type());
	// Tam giác vuông
	Triangle::translate(Triangle(Vector2(0.0, 0.0), Vector2(1.0, 0.0), Vector2(0.0, 2.0)).type());
	// Tam giác vuông cân
	Triangle::translate(Triangle(Vector2(0.0, 0.0), Vector2(1.0, 0.0), Vector2(0.0, 1.0)).type());
	return 0;
}
