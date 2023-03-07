#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>

#define NAMES_SIZE 30
#if true
#define inp std::cin
#define out std::cout
#define char_type int8_t
#define string_type std::string
#else
#define inp std::wcin
#define out std::wcout
#define char_type wchar_t
#define string_type std::wstring
#endif

#define BUFFER_READ(char_array, size) {          \
	std::string buffer;                          \
	std::getline(std::cin >> std::ws, buffer);   \
	buffer.resize(size);                         \
	memcpy(char_array, buffer.data(), size);     \
	char_array[size - 1] = '\0';                 \
}

struct Province {
	uint16_t id;
	char_type name[NAMES_SIZE];
	double area;
	Province() {
		id = 0; memset(name, 0, NAMES_SIZE * sizeof(char_type)); area = 0.0;
	}
	void deserialize(const std::vector<uint8_t>& bytes) {
		if (bytes.size() != sizeof(Province)) return;
		memcpy(this, bytes.data(), bytes.size() * sizeof(char_type));
	}
	std::vector<uint8_t> serialize() const {
		std::vector<uint8_t> re; re.resize(sizeof(Province));
		memcpy(re.data(), this, re.size());
		return re;
	}
	void read() {
		out << "Province's id: ";
		inp >> id;
		out << "Province's name: ";
		BUFFER_READ(name, NAMES_SIZE);
		out << "Surface area: ";
		inp >> area;
	}
	void write() const {
		out << "Province's id: " << id << "\n" << "Province's name: " << name << "\n" << "Surface area" << area << "\n";
	}
};

int main() {
	Province p{};
	p.read();
	p.write();
	return 0;
}
