#include <iostream>
#include <string>
#include <cstring>
#include <atomic>
#include <cstdio>
#include <stdio.h>
#include <cerrno>

//
// Copy từ bài tập chương I UwU
//

#define CLAMP(x, y, z) (x < y ? y : (x > z ? z : x))
#define SWAP(x, y) __swap_tmpl((x), (y))
template <class T>
inline void __swap_tmpl(T& x, T& y) {
	T aux = x;
	x = y;
	y = aux;
}

// Wrapper cho việc đọc file trên Windows 
// Hỗ trợ big/little endian
class File {
public:
	enum Mode {
		READ, WRITE
	};
	bool endian_swap = false;
private:
	FILE* f;
	mutable Mode current_mode, previous_mode = READ;
public:
	File() = default;
	File(const std::string& f_path, Mode mode) {
		_open(f_path, mode);
	}
	~File() { close(); }

	void close() {
		if (!f) return;
		fclose(f);
		f = nullptr;
	}
	bool is_open() const {
		return f;
	}
	void _open(const std::string& f_path, Mode mode = READ) {
		if (f) close();
		//const wchar_t* mode_string;
		//if (mode == READ) mode_string = L"rb";
		//else if (mode == WRITE) mode_string = L"wb";
		//else return;
		//current_mode = mode;
		//f = _wfsopen((const wchar_t*)f_path.c_str(), mode_string, 0x40); // deny none mode
		const char* mode_string;
		current_mode = mode;
		if (current_mode == READ) mode_string = "rb";
		else if (current_mode == WRITE) mode_string = "wb";
		else return;
		auto err = fopen_s(&f, f_path.c_str(), mode_string);
		char out[80];
		strerror_s(out, 80, err);
		std::cout << "File status: " << out << "\n";
	}
	unsigned long long length() const {
		unsigned long long pos = get_position();
		_fseeki64(f, 0, SEEK_END);
		unsigned long long size = get_position();
		_fseeki64(f, pos, SEEK_SET);

		return size;
	}
	unsigned long long get_position() const {
		return _ftelli64(f);
	}
	unsigned char get_8() const {
		if (!f || current_mode != READ) return '\0';
		if (previous_mode == WRITE) fflush(f);
		previous_mode = READ;
		unsigned char b;
		if (fread(&b, 1, 1, f) == 0) {
			b = '\0';
		};
		return b;
	}
	unsigned short get_16() const {
		uint16_t res;
		uint8_t a, b;

		a = get_8();
		b = get_8();

		if (endian_swap) {
			SWAP(a, b);
		}

		res = b;
		res <<= 8;
		res |= a;

		return res;
	}
	unsigned int get_32() const {
		uint32_t res;
		uint16_t a, b;

		a = get_16();
		b = get_16();

		if (endian_swap) {
			SWAP(a, b);
		}

		res = b;
		res <<= 16;
		res |= a;

		return res;
	}
	unsigned long long get_64() const {
		uint64_t res;
		uint32_t a, b;

		a = get_32();
		b = get_32();

		if (endian_swap) {
			SWAP(a, b);
		}

		res = b;
		res <<= 32;
		res |= a;

		return res;
	}
	void store_16(const unsigned short& what) {
		uint8_t a, b;

		a = what & 0xFF;
		b = what >> 8;

		if (endian_swap) {
			SWAP(a, b);
		}

		store_8(a);
		store_8(b);
	}
	void store_32(const unsigned int& what) {
		uint16_t a, b;

		a = what & 0xFFFF;
		b = what >> 16;

		if (endian_swap) {
			SWAP(a, b);
		}

		store_16(a);
		store_16(b);
	}
	void store_64(const unsigned long long& what) {
		uint32_t a, b;

		a = what & 0xFFFFFFFF;
		b = what >> 32;

		if (endian_swap) {
			SWAP(a, b);
		}

		store_32(a);
		store_32(b);
	}
	unsigned long long get_buffer(unsigned int* dest, unsigned long long size) const {
		if (!dest && length() > 0) return 0;
		if (!f) return 0;
		if (previous_mode == WRITE) fflush(f);
		previous_mode = READ;
		unsigned long long re = fread(dest, 1, size, f);
		return re;
	}
	void store_8(const unsigned char& what) {
		if (!f) return;
		previous_mode = WRITE;
		fwrite(&what, 1, 1, f);
	}
	unsigned long long store_buffer(const unsigned char* source, const unsigned long long& size) {
		if (!f) return 0;
		if (!size && size > 0) return 0;
		previous_mode = WRITE;
		fseek(f, 0, SEEK_CUR);
		return fwrite(source, 1, size, f);
	}
	void flush() {
		if (!f) return;
		fflush(f);
		previous_mode = READ;
	}
	void seek(const unsigned long long& loc) {
		if (!f) return;
		_fseeki64(f, loc, SEEK_SET);
		previous_mode = READ;
	}
	static File* open(const std::string& f_path, Mode mode = READ) {
		return new File(f_path, mode);
	}
};


// Thuật toán mã hóa chuỗi djb2
static inline unsigned long string_hash(const std::string& target) {
	const char* chr = target.c_str();
	uint32_t hashv = 5381;
	uint32_t c;

	while ((c = *chr++)) {
		hashv = ((hashv << 5) + hashv) + c;
	}

	return hashv;
}

struct StandardHasher {
	static inline unsigned long hash(unsigned char target) { return unsigned long(target); }
	static inline unsigned long hash(const std::string& target) { return string_hash(target); }
};

//
// Hash Map, hay Hash Table sử dụng nhiều loại thuật toán mã hóa để
// lưu trữ và truy xuất dữ liệu
// Hash table gồm key và value, key là "chìa khóa" dẫn đến value
// Sau khi được hash, hay mã hóa thì key trở thành một dãy số đặc biệt
// có thể dùng để trực tiếp tham chiếu value bằng toán tử [] mà không
// cần lặp qua hết tất cả các phần tử mảng, khiến nó đạt được
// best-case scenario là O(1)
// 
// Tuy nhiên để ưu tiên hiệu năng cho chương trình, thuật toán mã hóa cho hash table
// này chỉ trả về số nguyên có độ rộng 32-bit, khiến cho một vài key khác nhau sau khi
// mã hóa lại lại có hash giống như nhau, nếu trực tiếp lưu nó vào hash table 
// sẽ khiến chúng "đè" lên nhau
// 
// Dù cấp cho hash table 4 294 967 295 ô nhớ - trần giá trị của hàm hash 32-bit - thì
// các dữ liệu vẫn có khả năng ghi đè lên nhau
// Bởi vậy nên thay vì lưu trực tiếp dữ liệu vào hash table ta lưu linked list
// chứa các dữ liệu đó
//
// Về Hashfuncs:
// Kiểu char không cần được hash vì trần quá nhỏ (255), nhưng kiểu int, long có thể được hash bằng
// Knuth's multiplicative method, nhưng trong chương trình này không xài key
// kiểu int/long nên không cần thiết :V
// Kiểu string sử dụng thuật toán mã hóa chuỗi 32-bit djb2 do "không phải tôi" làm ra
//
// Về Power:
// Power quy định độ lớn của hash table, Power càng cao, linked list càng ít, 
// hash table hoạt động hiệu quả hơn, nhưng ngoài ra tốn bộ nhớ nhiều hơn (để chứa hash table)
// Nếu Power = 8 thì độ lớn của hash table là 1 << 8 = 2 ^ 7 = 64 nodes
//
// Về cấu trúc dữ liệu
// Mỗi hash table là một mảng các linked list, hay trong đây gọi là Element,
// mỗi Element chứa dữ liệu chính (key và value), ngoài ra còn chứa một
// Element "liền kề" nó để truy xuất.
// Việc truy xuất Element diễn ra một chiều nên không cần con trỏ đến Element trước nó
// Hash table này chỉ hoạt động nếu các key có thể trực tiếp so sánh bằng == (qua việc
// hỗ trợ native hoặc các hàm override) nhưng vì nó chỉ được dùng để chứa key
// char và string nên không cần viết thêm lớp So sánh dữ liệu
// 
// Viết bởi Nguyễn Khánh Nam
//
template<class K, class V, class Hasher = StandardHasher, unsigned char Power = 8>
class HashMap {
public:
	struct Pair {
		K key{};
		V value{};
	};
	struct Element {
		Element* next = nullptr;
		Pair pair{};
	};
private:
	Element** hash_table = nullptr;
	unsigned int s = 0;
	unsigned char p;

	Element* create_elem(const K& key) {
		Element* E = new Element();
		E->pair.key = key;
		s++;
		return E;
	}
public:
	void clear() {
		for (unsigned int i = 0; i < (1i64 << p); i++) {
			while (hash_table[i]) {
				Element* E = hash_table[i];
				hash_table[i] = E->next;
				delete E;
			}
		}
		s = 0;
	}
	unsigned int size() const {
		return s;
	}
	inline V& operator[](const K& key) {
		unsigned long hash = Hasher::hash(key) % (1i64 << p);
		if (!hash_table[hash]) {
			Element* E = create_elem(key);
			hash_table[hash] = E;
			return E->pair.value;
		}
		else {
			Element* iter = hash_table[hash];
			while (true) {
				if (iter->pair.key == key) return iter->pair.value;
				if (iter->next == nullptr) {
					Element* E = create_elem(key);
					iter->next = E;
					return E->pair.value;
				}
				iter = iter->next;
			}
		}
	}
	inline bool erase(const K& key) {
		unsigned long hash = Hasher::hash(key) % (1i64 << p);
		if (!hash_table[hash]) return false;
		else {
			Element* iter = hash_table[hash];
			Element* prev = nullptr;
			while (true) {
				if (iter->pair.key == key) {
					if (prev != nullptr) {
						prev->next = iter->next;
					}
					else hash_table[hash] = iter->next;
					delete iter;
					s--;
					return true;
				}
				if (iter->next == nullptr) {
					return false;
				}
				prev = iter;
				iter = iter->next;
			}
		}
		return false;
	}
	inline bool has(const K& key) const {
		unsigned long hash = Hasher::hash(key) % (1i64 << p);
		if (!hash_table[hash]) return false;
		else {
			Element* iter = hash_table[hash];
			while (true) {
				if (iter->pair.key == key) {
					return true;
				}
				if (iter->next == nullptr) {
					return false;
				}
				iter = iter->next;
			}
		}
		return false;
	}
	void copy(const HashMap& other) {
		// Copy thủ công dữ liệu từ HashMap khác thay vì dùng memcpy
		if (&other == this) return;
		clear();
		free(hash_table);
		hash_table = (Element**)malloc(sizeof(Element*) * (1i64 << p));
		p = other.p;
		for (unsigned int i = 0; i < (1i64 << p); i++) {
			hash_table[i] = nullptr;
			Element* E = other.hash_table[i];
			Element* iter = nullptr;
			while (E) {
				Element* le = new Element();
				le->pair.key = E->pair.key;
				le->pair.value = E->pair.value;
				if (iter == nullptr) {
					hash_table[i] = le;
				}
				else {
					iter->next = le;
				}
				iter = le;
				E = E->next;
			}
		}
	}
	inline const Element** get_elem() const { return const_cast<const Element * *>(hash_table); }
	unsigned char get_power() const { return p; }
	HashMap() : p(Power) {
		// Làm sạch hash table
		size_t s = sizeof(Element*) * (1i64 << p);
		hash_table = (Element**)malloc(s);
		memset(hash_table, 0, s);
	}
	HashMap(const HashMap& other) {
		copy(other);
	}
	HashMap& operator=(const HashMap& other) {
		copy(other);
	}
	~HashMap() {
		clear();
		free(hash_table);
	}

};


struct ChamCongNhanVien {
	unsigned char m_Permitted, m_Absent, m_Overtime, m_Month;
	std::string m_Rating;
	double m_Salary;
	float m_Boost;

	ChamCongNhanVien() {
		m_Permitted = m_Absent = m_Overtime = m_Month = 0;
		m_Salary = 0;
		m_Boost = 0.0f;
		m_Rating = "T";
	}
	void tinh_luong(const double& base, const float& boost) {
		m_Salary = base * (1.0f + boost);
		m_Boost = boost;
	}
};

class LyLichNhanVien {
public:
	struct _Data {
		// Bắt chước kiến trúc Copy-on-Write
		std::string m_Id{}, m_Name{}, m_Education{};
		bool m_Married{};
		unsigned char m_HeadCount{};
		double m_StartingSalary{};
		std::atomic_int m_RefCount;

	};
private:
	_Data* data;
public:
	LyLichNhanVien() {
		data = new _Data();
		data->m_RefCount.store(1);
	}
	LyLichNhanVien(const LyLichNhanVien& other) {
		data = const_cast<_Data*>(other.data);
		data->m_RefCount++;
	}
	~LyLichNhanVien() {
		if (--(data->m_RefCount) == 0) {
			delete data;
		}
	}
	_Data& get_data() { return *data; }
	const _Data& get_data() const { return *data; }

};

#define FILE_MAGIC 0x716C6E76U

class NhanVien {
private:
	LyLichNhanVien m_Portfolio;
	HashMap<unsigned char, ChamCongNhanVien> m_Record{};
public:
	NhanVien() {

	}
	NhanVien(const NhanVien& other){
		m_Portfolio = other.m_Portfolio;
		m_Record = other.m_Record;
	}
	inline LyLichNhanVien& ly_lich() { return m_Portfolio; }
	void cham_cong(const unsigned char& month) {
		ChamCongNhanVien& rec = m_Record[CLAMP(month, 1, 12)];
		float boost = 0.0f;
		if (m_Portfolio.get_data().m_HeadCount > 2) boost += 0.05f;
		if (m_Portfolio.get_data().m_Education == "CH") boost += 0.1f;
		boost += 0.04 * rec.m_Overtime;
		if (rec.m_Absent > 0) boost -= 0.05f * rec.m_Absent;
		rec.tinh_luong(m_Portfolio.get_data().m_StartingSalary, boost);
		m_Record[CLAMP(month, 1, 12)] = rec;
	}
	inline ChamCongNhanVien& bang_cham_cong(const unsigned char& month) { return m_Record[CLAMP(month, 1, 12)]; }
	inline bool xoa_bang_cham_cong(const unsigned char& month) { return m_Record.erase(CLAMP(month, 1, 12)); }
	void log() {
		std::string re;
		for (unsigned char i = 1; i <= 12; i++) {
			// Chuyển đổi từ số nguyên sang chữ số trong ASCII
			std::cout << "Thang " << int(i) << "  \t--- ";
			if (!m_Record.has(i)) {
				std::cout << '\n';
				continue;
			}
			ChamCongNhanVien& rec = m_Record[i];
			std::cout << "Luong thuc linh: " << rec.m_Salary << "; Phu troi: " << rec.m_Boost * 100.0f << "%\n";
		}
	}
	void to_file(File* f) const {
		const LyLichNhanVien::_Data& data = m_Portfolio.get_data();
		/*f->store_buffer((const unsigned char*)data.m_Id.c_str(), data.m_Id.size());
		f->store_8('\0');*/
		f->store_buffer((const unsigned char*)data.m_Name.c_str(), data.m_Name.size());
		f->store_8('\0');
		f->store_buffer((const unsigned char*)data.m_Education.c_str(), data.m_Education.size());
		f->store_8('\0');

		f->store_8((unsigned char)data.m_Married);
		f->store_8((unsigned char)data.m_HeadCount);
		f->store_64(*((unsigned long long*)&data.m_StartingSalary));
		f->store_32(m_Record.size());
		if (m_Record.size() == 0) return;
		//------------------------------------------------------
		const HashMap<unsigned char, ChamCongNhanVien>::Element** elem = m_Record.get_elem();
		unsigned char power = m_Record.get_power();
		for (unsigned int i = 0; i < (1i64 << power); i++) {
			if (!elem[i]) continue;
			const HashMap<unsigned char, ChamCongNhanVien>::Element* E = elem[i];
			while (E) {
				unsigned char month = E->pair.key;
				const ChamCongNhanVien& rec = E->pair.value;
				f->store_8(month);
				f->store_buffer((const unsigned char*)rec.m_Rating.c_str(), rec.m_Rating.size());
				f->store_8('\0');
				f->store_8(rec.m_Permitted);
				f->store_8(rec.m_Absent);
				f->store_8(rec.m_Overtime);
				f->store_64(*((unsigned long long*) & rec.m_Salary));
				f->store_32(*((unsigned int*) & rec.m_Boost));
				E = E->next;
			}
		}

	}
	void read_file(File* f) {
		LyLichNhanVien::_Data& data = m_Portfolio.get_data();
		std::string edu = "";
		while (true) {
			unsigned char c = f->get_8();
			if (c == '\0') break;
			edu += c;
		}
		data.m_Education = edu;
		data.m_Married = f->get_8();
		data.m_HeadCount = f->get_8();
		unsigned long long ss_buffer = f->get_64();
		data.m_StartingSalary = *((double*)&ss_buffer);
		unsigned int rec_size = f->get_32();
		for (unsigned int i = 0; i < rec_size; i++) {
			unsigned char month = f->get_8();
			ChamCongNhanVien& rec = m_Record[month];
			std::string rating = "";
			while (true) {
				unsigned char c = f->get_8();
				if (c == '\0') break;
				rating += c;
			}
			rec.m_Rating = rating;
			rec.m_Permitted = f->get_8();
			rec.m_Absent = f->get_8();
			rec.m_Overtime = f->get_8();
			unsigned long long s_buffer = f->get_64();
			unsigned int b_buffer = f->get_32();
			rec.m_Salary = *((double*)&s_buffer);
			rec.m_Boost = *((float*)&b_buffer);
		}
	}
};


// Nếu lưu NhanVien trực tiếp vào 2 HashMap thì nếu một cái bị thay đổi
// Thì cái kia không cập nhật được
//
// Bởi vậy nên 2 HashMap đó chỉ dùng để lưu trữ con trỏ đến dữ liệu thật
// trong khi dữ liệu đó được quản lý bởi m_RealMap (để tiện cho việc quản lý bộ nhớ)
//
class Hub {
private:
	HashMap<std::string, NhanVien> m_RealMap;
	HashMap<std::string, NhanVien*> m_ByID;
	HashMap<std::string, NhanVien*> m_ByName;
	static Hub* singleton;
public:
	Hub() {
		singleton = this;
	}
	static inline Hub* get_singleton() { return singleton; }
	inline NhanVien* tim_theo_ten(const std::string& name) {
		if (m_ByName.has(name)) return m_ByName[name];
		return nullptr;
	}
	inline NhanVien* tim_theo_id(const std::string& id) {
		if (m_ByID.has(id)) return m_ByID[id];
		return nullptr;
	}
	inline bool xoa_nhan_vien(const std::string& id) {
		if (!m_ByID.has(id)) return false;
		std::string name = m_ByID[id]->ly_lich().get_data().m_Name;
		m_ByID.erase(id);
		m_ByName.erase(name);
		m_RealMap.erase(id);
		return true;
	}
	inline bool them_nhan_vien(NhanVien& employee) {
		// Thêm nhân viên không cần bỏ vào m_RealMap vì nó không phải do m_RealMap quản lý
		if (tim_theo_id(employee.ly_lich().get_data().m_Id) != nullptr) return false;
		*m_ByID[employee.ly_lich().get_data().m_Id] = employee;
		*m_ByName[employee.ly_lich().get_data().m_Name] = employee;
		return true;
	}
	inline NhanVien* nhan_vien_moi(const std::string& name, const std::string& id) {
		if (m_ByID.has(id)) return nullptr;
		NhanVien* ptr = &m_RealMap[id];
		ptr->ly_lich().get_data().m_Name = name;
		ptr->ly_lich().get_data().m_Id = id;
		m_ByName[name] = ptr;
		m_ByID[id] = ptr;
		return ptr;
	}
	//
	// Cấu trúc lưu trữ:
	// 4 bytes đầu: magic, chuỗi định dạng file (magic này là ASCII của "qlnv")
	// 4 bytes tiếp: tổng số nhân viên => n_nv
	// (Lặp n_nv lần)
	// (n byte cho đến khi gặp ký tự \0): id nhân viên
	// (n byte cho đến khi gặp ký tự \0): tên nhân viên
	// (n byte cho đến khi gặp ký tự \0): trình độ giáo dục
	// 1 byte sau: tình trạng hôn nhân
	// 1 byte sau: số con
	// 8 bytes sau: lương khởi điểm
	// 4 bytes sau: tổng số chấm công nhân viên (cao nhất 12) => n_cc
	// (lặp n_cc lần)
	// 1 byte đầu: tháng thứ
	// (n byte cho đến khi gặp ký tự \0): kết quả công việc
	// 1 byte sau: số ngày nghỉ có phép trong tháng
	// 1 byte sau: số ngày nghỉ không phép trong tháng
	// 1 byte sau: số ngày làm thêm trong tháng
	// 8 bytes sau: lương thực lĩnh (tính bởi chương trình)
	// 4 bytes sau: phụ trội
	// (Hết lặp...)
	// (Hết lặp...)
	// <EOF>
	//
	void store_file(File* f) const {
		if (f == nullptr) {
			std::cout << "Khong mo duoc file\n";
			return;
		}
		else if (!f->is_open()) {
			std::cout << "File khong hop le\n";
			delete f;
			return;
		}
		if (m_RealMap.size() == 0) {
			std::cout << "Khong co gi de luu\n";
			delete f;
			return;
		}
		f->seek(0);
		f->store_32(FILE_MAGIC);
		f->store_32(m_RealMap.size());
		const HashMap<std::string, NhanVien>::Element** elem = m_RealMap.get_elem();
		unsigned char power = m_RealMap.get_power();
		for (unsigned int i = 0; i < (1i64 << power); i++) {
			if (!elem[i]) continue;
			const HashMap<std::string, NhanVien>::Element* E = elem[i];
			while (E) {
				std::string id = E->pair.key;
				std::cout << "Dang luu " << id << "...\n";
				// Kiểu chuỗi lúc nào cũng dư một ký tự \0 ở đuôi để đánh dấu kết thúc
				// nên store thêm một ký tự \0
				f->store_buffer((const unsigned char*)id.c_str(), id.size());
				f->store_8('\0');
				E->pair.value.to_file(f);
				E = E->next;
			}
		}
		std::cout << "Luu thanh cong\n";
		f->flush();
		f->close();
		delete f;
	}
	void load_file(File* f) {
		if (f == nullptr) {
			std::cout << "Khong mo duoc file\n";
			return;
		}
		else if (!f->is_open()) {
			std::cout << "File khong hop le\n";
			delete f;
			return;
		}
		if (m_RealMap.size() != 0) {
			std::cout << "Chi co the load file khi khong co du lieu\n";
			delete f;
			return;
		}
		f->seek(0);
		unsigned int magic = f->get_32();
		if (magic != FILE_MAGIC) {
			std::cout << "File khong hop le\n";
			delete f;
			return;
		}
		unsigned int employee_count = f->get_32();
		for (unsigned int i = 0; i < employee_count; i++) {
			std::string id = "", name = "";
			while (true) {
				unsigned char c = f->get_8();
				if (c == '\0') break;
				id += c;
			}
			std::cout << "Dang tai " << id << '\n';
			while (true) {
				unsigned char c = f->get_8();
				if (c == '\0') break;
				name += c;
			}
			auto employee = nhan_vien_moi(name, id);
			employee->read_file(f);
		}
		std::cout << "Doc file thanh cong\n";
		delete f;
	}
};

void chinh_sua_ly_lich(NhanVien* nv = nullptr) {
	std::cout << "Chinh sua ly lich nhan vien\n";
	std::string my_id;
	if (nv == nullptr) {
		do {
			std::cout << "\t- Ma nhan vien (nhieu nhat 8 ky tu): ";
			std::getline(std::cin >> std::ws, my_id);
		} while (my_id.size() > 8);
		nv = Hub::get_singleton()->tim_theo_id(my_id);
		if (nv == nullptr) {
			std::cout << "Khong tim thay nhan vien\n";
			return;
		}
	}
	else my_id = nv->ly_lich().get_data().m_Name;
	std::cout << "\t- Tinh trang hon nhan (0 = chua ket hon, 1 = da ket hon): ";
	std::cin >> nv->ly_lich().get_data().m_Married;
	int child_count;
	do {
		std::cout << "\t- So con ( <= 20): ";
		std::cin >> child_count;
	} while (child_count < 0 || child_count > 20);
	nv->ly_lich().get_data().m_HeadCount = child_count;
	std::string edu;
	do {
		std::cout << "\t- Trinh do giao duc (C1/C2/C3/DH/CH): ";
		std::getline(std::cin >> std::ws, edu);
	} while (!(edu == "C1" || edu == "C2" || edu == "C3" || edu == "DH" || edu == "CH"));
	nv->ly_lich().get_data().m_Education = edu;
	double salary;
	do {
		std::cout << "\t- Luong can bang ( <= 1000000): ";
		std::cin >> salary;
	} while (salary > 1'000'000);
	nv->ly_lich().get_data().m_StartingSalary = salary;
	std::cout << "Hoan thanh\n";
}

void them_nhan_vien() {
	std::string name, id;
	std::cout << "Them nhan vien\n";
	do {
		std::cout << "\t- Ten nhan vien (nhieu nhat 20 ky tu): ";
		std::getline(std::cin >> std::ws, name);
	} while (name.size() > 20);
	do {
		std::cout << "\t- Ma nhan vien (nhieu nhat 8 ky tu): ";
		std::getline(std::cin >> std::ws, id);
	} while (id.size() > 8);
	NhanVien* nv = Hub::get_singleton()->nhan_vien_moi(name, id);
	if (nv == nullptr) {
		std::cout << "Nhan vien da ton tai\n";
		return;
	}
	chinh_sua_ly_lich(nv);
	std::cout << "Da them nhan vien\n";
}

void xoa_nhan_vien() {
	std::string name, id;
	std::cout << "Xoa nhan vien\n";
	std::cout << "\t- Ma nhan vien (nhieu nhat 8 ky tu): ";
	std::getline(std::cin >> std::ws, id);
	if (Hub::get_singleton()->xoa_nhan_vien(id)) {
		std::cout << "Xoa nhan vien thanh cong\n";
	}
	else {
		std::cout << "Khong tim thay nhan vien\n";
	}
}

void chinh_sua_bang_cham_cong() {
	std::string id;
	unsigned int month;
	std::cout << "Them/Chinh sua bang cham cong\n";
	std::cout << "\t- Ma nhan vien (nhieu nhat 8 ky tu): ";
	std::getline(std::cin >> std::ws, id);
	NhanVien* nv = Hub::get_singleton()->tim_theo_id(id);
	if (nv == nullptr) {
		std::cout << "Khong tim thay nhan vien\n";
		return;
	}
	do {
		std::cout << "\t- Month (1-12): ";
		std::cin >> month;
	} while (month < 1 && month > 12);
	unsigned int permitted, absent, overtime;
	std::string rating;
	do {
		std::cout << "\t- So ngay nghi co phep ( <= 28): ";
		std::cin >> permitted;
	} while (permitted > 28);
	do {
		std::cout << "\t- So ngay nghi khong phep ( <= 28): ";
		std::cin >> absent;
	} while (absent > 28);
	do {
		std::cout << "\t- So ngay lam them trong thang ( <= 28): ";
		std::cin >> overtime;
	} while (overtime > 28);
	do {
		std::cout << "\t- Ket qua cong viec (T/TB/K): ";
		std::getline(std::cin >> std::ws, rating);
	} while (rating != "T" && rating != "TB" && rating != "K");
	ChamCongNhanVien& rec = nv->bang_cham_cong(month);
	rec.m_Absent = absent; rec.m_Overtime = overtime; rec.m_Permitted = permitted; rec.m_Rating = rating;
	nv->cham_cong(month);
	std::cout << "Hoan thanh\n";
}

void xoa_bang_cham_cong() {
	std::cout << "Xoa bang cham cong\n";
	unsigned int month;
	std::string id;
	std::cout << "\t- Ma nhan vien (nhieu nhat 8 ky tu): ";
	std::getline(std::cin >> std::ws, id);
	NhanVien* nv = Hub::get_singleton()->tim_theo_id(id);
	if (nv == nullptr) {
		std::cout << "Khong tim thay nhan vien\n";
		return;
	}
	do {
		std::cout << "\t- Month (1-12): ";
		std::cin >> month;
	} while (month < 1 && month > 12);
	nv->xoa_bang_cham_cong(month);
	std::cout << "Xoa bang cham cong thanh cong\n";
}

void xem_bang_luong() {
	std::cout << "Bang luong hang thang\n";
	std::string id;
	std::cout << "\t- Ma nhan vien (nhieu nhat 8 ky tu): ";
	std::getline(std::cin >> std::ws, id);
	NhanVien* nv = Hub::get_singleton()->tim_theo_id(id);
	if (nv == nullptr) {
		std::cout << "Khong tim thay nhan vien\n";
		return;
	}
	nv->log();
}

void thong_tin_nhan_vien(NhanVien& nv) {
	LyLichNhanVien::_Data* data = &(nv.ly_lich().get_data());
	std::cout << "Thong tin nhan vien\n";
	std::cout << "\t-- Ma nhan vien: " << data->m_Id << "\n";
	std::cout << "\t-- Ten nhan vien: " << data->m_Name << "\n";
	std::cout << "\t-- Tinh trang hon nhan: " << (data->m_Married ? "Da ket hon" : "Chua ket hon") << "\n";
	std::cout << "\t-- So con: " << int(data->m_HeadCount) << "\n";
	std::cout << "\t-- Trinh do van hoa: " << data->m_Education << "\n";
	std::cout << "\t-- Luong can bang: " << data->m_StartingSalary << "\n";
}

void tim_nhan_vien_id() {
	std::cout << "Tim nhan vien (bang id)\n";
	std::string id;
	std::cout << "\t- Ma nhan vien (nhieu nhat 8 ky tu): ";
	std::getline(std::cin >> std::ws, id);
	NhanVien* nv = Hub::get_singleton()->tim_theo_id(id);
	if (nv == nullptr) {
		std::cout << "Khong tim thay nhan vien\n";
		return;
	}
	thong_tin_nhan_vien(*nv);
}
void tim_nhan_vien_ten() {
	std::cout << "Tim nhan vien (bang ten)\n";
	std::string name;
	std::cout << "\t- Ten nhan vien (nhieu nhat 20 ky tu): ";
	std::getline(std::cin >> std::ws, name);
	NhanVien* nv = Hub::get_singleton()->tim_theo_ten(name);
	if (nv == nullptr) {
		std::cout << "Khong tim thay nhan vien\n";
		return;
	}
	thong_tin_nhan_vien(*nv);
}

void luu_file() {
	std::cout << "Luu file\n";
	std::string f_path;
	std::cout << "\t- Nhap duong dan cua file: ";
	std::getline(std::cin >> std::ws, f_path);
	File* f = File::open(f_path, File::WRITE);
	f->endian_swap = true;
	Hub::get_singleton()->store_file(f);
}
void doc_file() {
	std::cout << "Doc file\n";
	std::string f_path;
	std::cout << "\t- Nhap duong dan cua file: ";
	std::getline(std::cin >> std::ws, f_path);
	File* f = File::open(f_path, File::READ);
	f->endian_swap = true;
	Hub::get_singleton()->load_file(f);
}

bool nhap_lenh() {
	std::cout   << "Chon cau lenh: \n"
				<< "\t1. Them nhan vien\n"
				<< "\t2. Xoa nhan vien\n"
				<< "\t3. Chinh sua ly lich nhan vien\n"
				<< "\t4. Them/Chinh sua bang cham cong\n"
				<< "\t5. Xoa bang cham cong\n"
				<< "\t6. Xem bang luong hang thang\n"
				<< "\t7. Tim nhan vien (bang ten)\n"
				<< "\t8. Tim nhan vien (bang ma nhan vien)\n"
				<< "\t9. Luu file\n"
				<< "\t10: Doc file\n"
				<< "\t0. Ket thuc\nLua chon: ";
	// --------------------------------------------------------------------------------------------------
	unsigned int select;
	std::cin >> select;
	switch (select) {
	case 1: { them_nhan_vien(); break; }
	case 2: { xoa_nhan_vien(); break; }
	case 3: { chinh_sua_ly_lich(); break; }
	case 4: { chinh_sua_bang_cham_cong(); break; }
	case 5: { xoa_bang_cham_cong(); break; }
	case 6: { xem_bang_luong(); break; }
	case 7: { tim_nhan_vien_ten(); break; }
	case 8: { tim_nhan_vien_id(); break; }
	case 9: { luu_file(); break; }
	case 10: { doc_file(); break; }
	case 0: { return true;  }
	default: {
		std::cout << "Lua chon khong hop le!\n";
	}
	}
	std::getchar();
	return false;
}


Hub* Hub::singleton = nullptr;

int main(int argc, char** argv) {
	// Kiến trúc singleton
	Hub* hub_instance = new Hub();
	// --------------------------------------------------------------------------------------------------
	std::cout << "Phan mem quan ly nhan vien\n";
	bool exit = false;
	while (!exit) {
		exit = nhap_lenh();
	}
	// --------------------------------------------------------------------------------------------------
	delete hub_instance;
	return 0;
}
