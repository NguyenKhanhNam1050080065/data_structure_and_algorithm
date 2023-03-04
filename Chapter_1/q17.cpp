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

	enum Axis : unsigned int {
		AXIS_X = 2U, AXIS_Y = 4U,
	};

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
	inline float distance_by_axis_projection(const Vector2& other, Axis axis) const {
		// Khoảng cách 2 điểm qua trục X/Y
		switch (axis){
			case AXIS_X: {
				return other.x - x;
			}
			case AXIS_Y: {
				return other.y - y;
			}
		}
	}
	inline Vector2 symmetrical_point_of(const uint32_t& axises = AXIS_X + AXIS_Y) const {
		// Đối xứng qua trục (tung/hoành/cả hai)
		// Cài đặt mặc định: qua gốc tọa độ
		Vector2 re = duplicate();
		if (axises & AXIS_X) re.x = -x;
		if (axises & AXIS_Y) re.y = -y;
		return re;
	}
	inline Vector2 symmetrical_line(const float& param, Axis axis) const {
		// Đối xứng qua đường y = a hoặc x = b
		Vector2 re = duplicate();
		switch (axis){
			case AXIS_X: {
				float offset = x - param;
				re.x = param - offset;
				break;
			}
			case AXIS_Y: {
				float offset = y - param;
				re.y = param - offset;
				break;
			}
		}
		return re;
	}
private:
	inline bool is_in_quarter(const float& quat) const {
		// Kiểm tra điểm thuộc phần tư thứ "quat"
		return std::abs(x) < quat && std::abs(y) < quat;
	}
public:
	inline bool is_in_first_quarter() const {
		return is_in_quarter(0.25f);
	}
	inline bool is_in_second_quarter() const {
		return is_in_quarter(0.5f);
	}
	inline bool is_in_first_quarter() const {
		return is_in_quarter(0.75f);
	}
	inline bool is_in_second_quarter() const {
		return is_in_quarter(1.0f);
	}
	std::string stringify() const {
		return std::string("Vector2(") + std::to_string(x) + ", " + std::to_string(y) + ")";
	}
};

int main() {
	std::cout << Vector2(1.0, 3.0).direction_to(Vector2(-3.7, 2.32)).stringify();
	return 0;
}
