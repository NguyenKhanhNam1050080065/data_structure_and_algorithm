#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>

#define NAMES_SIZE 20

#define YEAR_TO_SECOND  31556926
#define MONTH_TO_SECOND 2629743
#define DAY_TO_SECOND   86400

static const std::vector<const char*> DAYS_IN_WEEK = { "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };


class Date {
private:
	uint64_t unix_second = 0;
public:
	struct DateGroup {
		uint32_t day, month, year;
		DateGroup(const uint32_t& d, const uint32_t& m, const uint32_t& y) {
			day = d; month = m; year = y;
		}
	};
public:
	static uint64_t dmy_to_unix_second(const uint32_t& day, const uint32_t& month, const uint32_t& year) {
		uint64_t accumulate = 0;
		// 1/1/1970 = 0 second giờ unix
		// => năm - 1970
		// => tháng - 1
		// => ngày - 1
		accumulate += (year - 1970) * YEAR_TO_SECOND;
		accumulate += (month - 1) * MONTH_TO_SECOND;
		accumulate += (day - 1) * DAY_TO_SECOND;
		return accumulate;
	}
public:
	static uint32_t zeller_congruence(uint32_t day, uint32_t month, uint32_t year) {
		if (month == 1) {
			month = 13;
			year--;
		}
		if (month == 2) {
			month = 14;
			year--;
		}
		int q = day;
		int m = month;
		int k = year % 100;
		int j = year / 100;
		int h = q + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j;
		h = h % 7;
		return h;
	}
	Date& set_date(const uint32_t& day = 1, const uint32_t& month = 1, const uint32_t& year = 1970) {
		// Chuyển giờ từ định dạng dd/mm/yyyy thành giờ unix
		// Bằng việc dùng giờ unix, lưu trữ và sử dụng kiểu Date sẽ hiệu quả và dễ dàng hơn
		if (!day || !month || year < 1970) unix_second = dmy_to_unix_second(1, 1, 1970);
		uint32_t max_day = 31;
		if (month == 4 || month == 6 || month == 9 || month == 11) max_day = 30;
		else if (month == 2) max_day = year % 4 == 0 ? 29 : 28;
		unix_second = dmy_to_unix_second(max_day < day ? max_day : day, month, year);
		return *this;
	}
	Date(const uint32_t& day = 1, const uint32_t& month = 1, const uint32_t& year = 1970) {
		set_date(day, month, year);
	}
	Date(const uint64_t& unix_time) {
		unix_second = unix_time;
	}
	uint64_t get_unix_time() const { return unix_second; }
	void from_unix_time(const uint64_t& unix_time) {
		unix_second = unix_time;
	}
	DateGroup get_date_group() const {
		uint32_t year, month, day;
		uint64_t left = unix_second;
		year = left / YEAR_TO_SECOND;
		left -= year * YEAR_TO_SECOND;
		month = left / MONTH_TO_SECOND;
		left -= month * MONTH_TO_SECOND;
		day = left / DAY_TO_SECOND;
		year += 1970; month += 1; day += 1;
		return DateGroup(day, month, year);
	}
	uint32_t day_since_epoch() const {
		// Tính số ngày từ ngày 1/1/1970
		return unix_second / DAY_TO_SECOND;
	}
	static Date find_day(const uint32_t& day_no, const uint32_t& year_no) {
		// Tìm ngày khi biết năm và số thứ tự của ngày trong năm
		uint64_t unix_second = year_no * YEAR_TO_SECOND;
		unix_second += day_no * DAY_TO_SECOND;
		return Date(unix_second);
	}
	static Date find_day_since_epoch(const uint32_t& day_no) {
		// Tìm ngày khi biết số thứ tự kể từ 1/1/1970
		return Date(day_no * DAY_TO_SECOND);
	}
	Date next_by(const uint32_t& d) const {
		// Ngày kế đó d ngày
		return Date((unix_second * d) + DAY_TO_SECOND);
	}
	Date prev_by(const uint32_t& d) const {
		// Ngày trước đó d nagy2
		return Date((unix_second * d) - DAY_TO_SECOND);
	}
	Date tomorrow() const {
		// Ngày mai
		return next_by(1);
	}
	Date yesterday() const {
		// Ngày hôm qua
		return prev_by(1);
	}
	const char* day_in_week() const {
		DateGroup dg = get_date_group();
		return DAYS_IN_WEEK[zeller_congruence(dg.day, dg.month, dg.year)];
	}
	uint32_t distance_by_date(const Date& from) const {
		// Khoảng cách giữa hai ngày
		//
		// Để tránh bị overflow, phải lặp điều kiện thủ công
		if (from.unix_second > unix_second) {
			return (from.unix_second - unix_second) / DAY_TO_SECOND;
		}
		return (unix_second - from.unix_second) / DAY_TO_SECOND;
	}
	bool operator==(const Date& other) const {
		// So sánh 2 ngày
		return unix_second == other.unix_second;
	}
	uint32_t day_no(const uint32_t& day, const uint32_t& year) {
		uint64_t left = unix_second;
		uint32_t y = left / YEAR_TO_SECOND;
		left -= y * YEAR_TO_SECOND;

	}
	std::string to_ddmmyyyy() const {
		// Ngày/Tháng/Năm
		DateGroup dg = get_date_group();
		return std::string("Date(\"dd/mm/yyyy\", ") + std::to_string(dg.day) + "/" + std::to_string(dg.month) + "/" + std::to_string(dg.year) + ")";
	}
	std::string to_mmddyyyy() const {
		// Tháng/Ngày/Năm
		DateGroup dg = get_date_group();
		return std::string("Date(\"mm/dd/yyyy\", ") + std::to_string(dg.month) + "/" + std::to_string(dg.day) + "/" + std::to_string(dg.year) + ")";
	}
	std::string to_yyyymmdd() const {
		// Năm/Tháng/Ngày
		DateGroup dg = get_date_group();
		return std::string("Date(\"yyyy/mm/dd\", ") + std::to_string(dg.year) + "/" + std::to_string(dg.month) + "/" + std::to_string(dg.day) + ")";
	}
	std::string stringify() const {
		DateGroup dg = get_date_group();
		return std::string("(") + day_in_week() + ", " + std::to_string(dg.day) + "/" + std::to_string(dg.month) + "/" + std::to_string(dg.year) + ")";
	}
	static Date read() {
		std::string buffer, s_day, s_month, s_year;
		uint32_t stage = 0;
		// std::cout << "Input Date: ";
		std::getline(std::cin >> std::ws, buffer);
		for (uint32_t i = 0, last = 0; i < buffer.size(); i++) {
			if (buffer[i] == '/') {
				if (stage == 0) {
					s_day.resize(i - last);
					memcpy(&s_day[0], &buffer[last], i - last);
				}
				else if (stage == 1) {
					s_month.resize(i - last);
					memcpy(&s_month[0], &buffer[last], i - last);
				}
				stage++;
				last = i + 1;
				if (stage == 2) {
					s_year.resize(4);
					memcpy(&s_year[0], &buffer[last], 4);
					break;
				}
			}
		}
		Date re(std::stoi(s_day), std::stoi(s_month), std::stoi(s_year));
		return re;
	}
};

#define BUFFER_READ(char_array, size) {          \
	std::string buffer;                          \
	std::getline(std::cin >> std::ws, buffer);   \
	buffer.resize(size);                         \
	memcpy(char_array, buffer.data(), size);     \
	char_array[size - 1] = '\0';                 \
}

struct MilkCarton {
	int8_t name[NAMES_SIZE];
	double net_weight;
	Date expiration_date;
	MilkCarton() {
		memset(name, 0, NAMES_SIZE * sizeof(int8_t));
	}
	void deserialize(const std::vector<uint8_t>& bytes) {
		if (bytes.size() != sizeof(MilkCarton)) return;
		memcpy(this, bytes.data(), bytes.size() * sizeof(int8_t));
	}
	std::vector<uint8_t> serialize() const {
		std::vector<uint8_t> re; re.resize(sizeof(MilkCarton));
		memcpy(re.data(), this, re.size());
		return re;
	}
	void read() {
		std::string buffer;
		std::cout << "Unit's name: ";
		std::getline(std::cin >> std::ws, buffer);
		BUFFER_READ(name, NAMES_SIZE);
		std::cout << "Unit's weight: ";
		std::cin >> net_weight;
		std::cout << "Expiration date: ";
		expiration_date = Date::read();
	}
	void write() const {
		std::cout << "Unit's name: " << name << "\nUnit's weight: " << net_weight << "\nExpiration date: " << expiration_date.stringify() << "\n";
	}
};

int main() {
	MilkCarton p{};
	p.read();
	p.write();
	return 0;
}
