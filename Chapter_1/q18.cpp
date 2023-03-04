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
	enum Axis {
		AXIS_X = 2,
		AXIS_Y = 4,
		AXIS_Z = 8,
	};
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
	inline float distance_by_axis_projection(const Vector3& other, Axis axis) const {
		// Khoảng cách 2 điểm qua trục X/Y
		switch (axis){
			case AXIS_X: {
				return other.x - x;
			}
			case AXIS_Y: {
				return other.y - y;
			}
			case AXIS_Z: {
				return other.z - z;
			}
		}
	}
	inline Vector3 symmetrical_point_of(const uint32_t& axises = AXIS_X + AXIS_Y + AXIS_Z) const {
		// Điểm đổi xứng qua trục (X/Y/Z/XY/YZ/XZ/XYZ)
		// Cài đặt mặc định: qua gốc tọa độ
		Vector3 re = duplicate();
		if (axises & AXIS_X) re.x = -x;
		if (axises & AXIS_Y) re.y = -y;
		if (axises & AXIS_Z) re.z = -z;
		return re;
	}
	std::string stringify() const {
		return std::string("Vector3(") + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}
};

int main() {
	return 0;
}
