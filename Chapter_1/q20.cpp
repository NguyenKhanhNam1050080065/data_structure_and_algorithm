#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>

#ifdef _MSC_VER
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif

struct Vector3 {
	float x, y, z;

	Vector3(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f) {
		this->x = x; this->y = y; this->z = z;
	}
	Vector3(const Vector3& other) {
		x = other.x; y = other.y; z = other.z;
	}
	inline Vector3 reversed() const {
		return Vector3(-x, -y, -z);
	}
	inline Vector3 inversed() const {
		return Vector3(1.0f / x, 1.0f / y, 1.0f / z);
	}
	inline float& operator[](const int32_t& idx){
		// Nếu tràn chỉ mục thì dừng chương trình đột ngột
		if (idx < 0 || idx > 2) GENERATE_TRAP();
		// Type punning: mọi đối tượng trong C++ là một khối dữ liệu có tuần tự có con trỏ bắt đầu từ property đầu tiên
		// Bởi vậy nên ta có thể làm được trò này lololol
		return ((float*)((void*)this))[idx];
	}
	inline const float& operator[](const int32_t& idx) const {
		return const_cast<Vector3*>(this)->operator[](idx);
	}
	inline void operator*=(const float& real_number){
		x *= real_number; y *= real_number;; z *= real_number;
	}
	inline void operator*=(const Vector3& other){
		x *= other.x; y *= other.y; z *= other.z;
	}
	inline void operator/=(const float& real_number){
		// Chia cho số thực
		// Nếu bằng 0 => dừng chương trình đột ngột
		if (real_number == 0.0f) GENERATE_TRAP();
		operator*=(1.0f / real_number);
	}
	inline void operator/=(const Vector3& other){
		// Chia cho véc tơ khác
		if (other.x == 0.0f || other.y == 0.0f) GENERATE_TRAP();
		operator*=(other.inversed());
	}
	inline void operator+=(const float& real_number){
		x += real_number; y += real_number; z += real_number;
	}
	inline void operator+=(const Vector3& other){
		x += other.x; y += other.y; z += other.z;
	}
	inline void operator-=(const float& real_number){
		operator+=(-real_number);
	}
	inline void operator-=(const Vector3& other){
		operator+=(other.reversed());
	}
	inline Vector3 operator*(const Vector3& other) const {
		Vector3 re = duplicate();
		re.operator*=(other);
		return re;
	}
	inline Vector3 operator/(const Vector3& other) const {
		Vector3 re = duplicate();
		re.operator/=(other);
		return re;
	}
	inline Vector3 operator+(const Vector3& other) const {
		Vector3 re = duplicate();
		re.operator+=(other);
		return re;
	}
	inline Vector3 operator-(const Vector3& other) const {
		Vector3 re = duplicate();
		re.operator-=(other);
		return re;
	}
	inline float length_squared() const {
		// Độ dài nhưng bình phương
		// Các thủ tục so sánh d8o65 dài véc tơ xài phương thức này sẽ có hiệu năng toy61 hơn
		// do không phải dùng hàm căn bậc
		return (x * x) + (y * y) + (z * z);
	}
	inline float length() const {
		// Độ dài
		return std::sqrt(length_squared());
	}
	inline Vector3 duplicate() const {
		// Sao chép
		return Vector3(*this);
	}
	inline Vector3& normalize() {
		// Đơn vị hóa
		*this /= length();
		return *this;
	}
	inline Vector3 normalized() const {
		// Đơn vị hóa nhưng là phương thức hằng
		return duplicate().normalize();
	}
	inline float dot(const Vector3& against) const {
		// Tích vô hướng
		return (x * against.x) + (y * against.y) + (z * against.z);
	}
	inline Vector3 cross(const Vector3& against) const {
		// Tích có hướng
		return  Vector3((y * against.z) - (z * against.y),
						(z * against.x) - (x * against.z),
						(x * against.y) - (y * against.x));
	}
	inline float distance_squared_to(const Vector3& to) const {
		// Khoảng cách bình phương đến
		// Giải thích ở trên
		return (to - *this).length_squared();
	}
	inline float distance_to(const Vector3& to) const {
		// Khoảng cách đến
		return (to - *this).length();
	}
	inline Vector3 direction_to(const Vector3& to) const {
		// Hướng đến
		return (to - *this).normalize();
	}
	std::string stringify() const {
		return std::string("(") + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}
};

#define Math_PI 3.14159265358979323846264338
#define FOUR_BY_THREE 1.333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333

struct Sphere {
	Vector3 origin;
	float radius = 1.0f;

	Sphere(const Vector3& origin = Vector3(), const float& radius = 1.0f){
		if (radius <= 0.0f) GENERATE_TRAP();
		this->origin = origin; this->radius = radius;
	}
	std::string stringify() const {
		// return std::string("(x - (") + std::to_string(origin.x) + "))^2 + (y - (" + std::to_string(origin.y) +"))^2 + (z - (" + std::to_string(origin.z) + "))^2 = " + std::to_string(radius * radius);
		return std::string("Sphere(") + origin.stringify() + ", " + std::to_string(radius) + ")";
	}
	bool is_part_of(const Vector3& point) const {
		float l = point.x - origin.x;
		float r = point.y - origin.y;
		float d = point.z - origin.z;
		return (l * l) + (r * r) + (d * d) == (radius * radius);
	}
	bool contains(const Vector3& point) const {
		float l = point.x - origin.x;
		float r = point.y - origin.y;
		float d = point.z - origin.z;
		return (l * l) + (r * r) + (d * d) < (radius * radius);
	}
	float surface() const { return 4.0 * Math_PI * radius * radius; }
	float volume() const { return FOUR_BY_THREE * Math_PI * radius * radius * radius; }
	bool touch(const Sphere& other) const {
		// Chạm nhau
		return origin.distance_to(other.origin) == radius + other.radius;
	}
	bool intersects(const Sphere& other) const {
		// Giao nhau
		return origin.distance_to(other.origin) < radius + other.radius;
	}
	bool collide(const Sphere& other) const {
		// Một trong hai
		return touch(other) || intersects(other);
	}
};

int main() {
	std::cout << Sphere(Vector3(1.0, 2.0, -1.0), 2.0).stringify() << "\n";
	std::cout << Sphere(Vector3(0.0, 0.0, 0.0), 2.0).collide(Sphere(Vector3(3.0, 3.0, 3.0), 5.0)) << "\n";
	return 0;
}
