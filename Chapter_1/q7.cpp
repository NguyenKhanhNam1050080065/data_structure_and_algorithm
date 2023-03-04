#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>

class Date {
private:
	uint64_t unix_second = 0;
public:
	static const uint32_t YEAR_TO_SECOND  = 31556926;
	static const uint32_t MONTH_TO_SECOND = 2629743;
	static const uint32_t DAY_TO_SECOND   = 86400;
public:
	static uint64_t dmy_to_unix_second(const uint32_t& day, const uint32_t& month, const uint32_t& year){
		uint64_t accumulate = 0;
		// 1/1/1970 = 0 second giờ unix
		// => năm - 1970
		// => tháng - 1
		// => ngày - 1
		accumulate += (year - 1970) * YEAR_TO_SECOND;
		accumulate +=  (month - 1) * MONTH_TO_SECOND;
		accumulate += (day - 1) * DAY_TO_SECOND;
		return accumulate;
	}
public:
	Date& set_date(const uint32_t& day = 1, const uint32_t& month = 1, const uint32_t& year = 1970){
		// Chuyển giờ từ định dạng dd/mm/yyyy thành giờ unix
		// Bằng việc dùng giờ unix, lưu trữ và sử dụng kiểu Date sẽ hiệu quả và dễ dàng hơn
		if (!day || !month || year < 1970) unix_second = dmy_to_unix_second(1, 1, 1970);
		uint32_t max_day = 31;
		if (month == 4 || month == 6 || month == 9 || month == 11) max_day = 30;
		else if (month == 2) max_day = year % 4 == 0 ? 29 : 28;
		unix_second = dmy_to_unix_second(max_day < day ? max_day : day, month, year);
		return *this;
	}
	Date(const uint32_t& day = 1, const uint32_t& month = 1, const uint32_t& year = 1970){
		set_date(day, month, year);
	}
	uint64_t get_unix_time() const { return unix_second; }
	void from_unix_time(const uint64_t& unix_time) {
		unix_second = unix_time;
	}
	std::string stringify() const {
		uint32_t year, month, day;
		uint64_t left = unix_second;
		year = left / YEAR_TO_SECOND;
		left -= year * YEAR_TO_SECOND;
		month = left / MONTH_TO_SECOND;
		left -= month * MONTH_TO_SECOND;
		day = left / DAY_TO_SECOND;
		year += 1970; month += 1; day += 1;
		return std::string("Date(\"dd/mm/yyyy\", ") + std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year) + ")";
	}
};

int main() {
	std::cout << Date(4, 3, 2023).stringify();
	return 0;
}
