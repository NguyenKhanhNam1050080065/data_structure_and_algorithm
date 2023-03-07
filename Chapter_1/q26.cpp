#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>

#define NAMES_SIZE 20
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

struct Good {
	char_type name[NAMES_SIZE];
	uint32_t price, stock;
	Good() {
		memset(name, 0, NAMES_SIZE * sizeof(char_type));
	}
	void deserialize(const std::vector<uint8_t>& bytes) {
		if (bytes.size() != sizeof(Good)) return;
		memcpy(this, bytes.data(), bytes.size() * sizeof(char_type));
	}
	std::vector<uint8_t> serialize() const {
		std::vector<uint8_t> re; re.resize(sizeof(Good));
		memcpy(re.data(), this, re.size());
		return re;
	}
	void read() {
		out << "Good's name: ";
		BUFFER_READ(name, NAMES_SIZE);
		out << "Good's price: ";
		inp >> price;
		out << "Good's stock: ";
		inp >> stock;
	}
	void write() const {
		out << "Good's name: " << name << "\nGood's price: " << price << "\nGood's stock: " << stock << "\n";
	}
};

int main() {
	Good p{};
	p.read();
	p.write();
	return 0;
}
