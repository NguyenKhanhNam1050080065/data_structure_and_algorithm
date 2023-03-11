#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <initializer_list>
#include <string>

#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline
#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

template <class T>
struct IntegerCompareAscend {
	static _ALWAYS_INLINE_ bool compare(const T& left, const T& right) {
		return left < right;
	}
};

template <class T>
struct IntegerCompareDescend {
	static _ALWAYS_INLINE_ bool compare(const T& left, const T& right) {
		return left > right;
	}
};

template <class T>
struct IntegerStringifier {
	static _ALWAYS_INLINE_ std::string stringify(const T& val) {
		return std::to_string(val);
	}
};

template <class T = long, class Comparator = IntegerCompareAscend<T>, class Stringifier = IntegerStringifier<T>, uint16_t ITEMS_PER_ALLOCATOR = 1000U>
class InsertTimeAutoSort {
public:
	struct ITASC {
	private:
		T value{};
		ITASC* next = nullptr;

	public:
		friend class InsertTimeAutoSort<T, Comparator, Stringifier, ITEMS_PER_ALLOCATOR>;
		ITASC() {}
		_ALWAYS_INLINE_ const ITASC* adj() const { return next; }
		_ALWAYS_INLINE_ T get() const { return value; }
	};
	struct BatchAllocator {
		ITASC* cells = nullptr;;
		BatchAllocator* next = nullptr;
	};
private:
	BatchAllocator* first_allocator = nullptr;
	BatchAllocator* current_allocator = nullptr;
	ITASC* first_cell = nullptr;
	uint32_t size_cache = 0;
	uint32_t iter_cache = 0;

	static _ALWAYS_INLINE_ void add_item(const T& n, ITASC** cells_list, ITASC* allocated_cell) {
		allocated_cell->value = n;
		if (*cells_list == nullptr) {
			*cells_list = allocated_cell;
			return;
		}
		ITASC* prev = nullptr, * curr = *cells_list;
		while (curr) {
			if (Comparator::compare(n, curr->value)) {
				if (!prev) {
					*cells_list = allocated_cell;
					allocated_cell->next = curr;
				}
				else {
					prev->next = allocated_cell;
					allocated_cell->next = curr;
				}
				return;
			}
			prev = curr; curr = curr->next;
		}
		prev->next = allocated_cell;
	}
	static _ALWAYS_INLINE_ void new_allocator(BatchAllocator** allocator) {
		*allocator = new BatchAllocator();
		(*allocator)->cells = (ITASC*)malloc(sizeof(ITASC) * ITEMS_PER_ALLOCATOR);
		for (uint32_t i = 0; i < ITEMS_PER_ALLOCATOR; i++) {
			new (&((*allocator)->cells[i])) ITASC();
		}
	}
	static _ALWAYS_INLINE_ void free_allocator(BatchAllocator* allocator) {
		free(allocator->cells);
		delete allocator;
	}
public:
	void _ALWAYS_INLINE_ input(const T& n) {
		add_item(n, &first_cell, &current_allocator->cells[iter_cache]);
		size_cache++; iter_cache++;
		if (iter_cache >= ITEMS_PER_ALLOCATOR) {
			iter_cache = 0;
			new_allocator(&current_allocator);
		}
	}
	uint32_t _ALWAYS_INLINE_ size() const {
		return size_cache;
	}
	std::string _ALWAYS_INLINE_ output() const {
		std::string re = "";
		const ITASC* iter = first_cell;
		for (; iter;) {
			re += Stringifier::stringify(iter->get());
			re += ' ';
			iter = iter->next;
		}
		re += '\n';
		return re;
	}
	_ALWAYS_INLINE_ ITASC* first() { return first_allocator; }
	_ALWAYS_INLINE_ const ITASC* first() const { return first_allocator; }
	InsertTimeAutoSort() {
		new_allocator(&first_allocator);
		current_allocator = first_allocator;
	}
	InsertTimeAutoSort(const std::initializer_list<T>& init_list) : InsertTimeAutoSort() {
		for (const T& item : init_list) {
			input(item);
		}
	}
	~InsertTimeAutoSort() {
		BatchAllocator* al = current_allocator, * queue = nullptr;
		while (al) {
			queue = al;
			al = al->next;
			free_allocator(queue);
		}
	}
};

int main() {
	// Lớp ITAS như trên lớp đã làm, nhưng sử dụng đối tượng, sử dụng batch allocator và inline tất cả logic
	//
	// Hướng đối tượng: Cho dễ nhìn
	// Batch Allocator: Giảm thời gian chờ để cấp phát bộ nhớ cho ITASC
	// Always inline: vì chương trình ngắn và quan trọng hiệu năng
	//
	// Dùng ITAS khi:
	// 	+ Nhập dữ liệu ngắn và theo thời gian thực
	// Không dùng ITAS khi:
	//	+ Dữ liệu cần sắp xếp đã có sẵn/không cần nhập
	//
	// Tối ưu:
	//	+ Sử dụng Hash map với hàm hash phụ thuộc vào chiều đi dữ liệu
	std::cout << "Ascend :";
	InsertTimeAutoSort<long, IntegerCompareAscend<long>> itas_a{ 1, -7, 36, -12, 4, 7, 8, 3, 2, 4, 2 };
	std::cout << itas_a.output();
	std::cout << "Descend :";
	InsertTimeAutoSort<long, IntegerCompareDescend<long>> itas_d{ 1, -7, 36, -12, 4, 7, 8, 3, 2, 4, 2 };
	std::cout << itas_d.output();
	InsertTimeAutoSort<long, IntegerCompareAscend<long>> itas_na;
	InsertTimeAutoSort<long, IntegerCompareDescend<long>> itas_nd;
	int n;
	std::cout << "Ascending Array\n";
	do {
		std::cout << "N = ";
		std::cin >> n;
		itas_na.input(n);
	} while (n != 0);
	std::cout << itas_na.output();
	std::cout << "Descending Array\n";
	do {
		std::cout << "N = ";
		std::cin >> n;
		itas_nd.input(n);
	} while (n != 0);
	std::cout << itas_nd.output();
}
