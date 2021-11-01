#include "string.h"

#include "error_macros.h"
#include <stdlib.h>
#include <cstdio>
#include <cstring>

void String::push_back(const char element) {
	ensure_capacity(_size + 1);

	_data[_size++] = element;
	_data[_size] = '\0';
}

void String::pop_back() {
	if (_size == 0) {
		return;
	}

	--_size;

	_data[_size] = '\0';
}

void String::remove(const int index) {
	_data[index] = _data[_size - 1];

	--_size;

	_data[_size] = '\0';
}

void String::erase(const char element) {
	int index = find(element);

	if (index != -1) {
		remove(index);
	}
}

void String::erase(const int start_index, const int length) {
	int sil = start_index + length;

	if (sil >= _size) {
		_size = 0;
		_data[_size] = '\0';
		return;
	}

	int j = start_index;
	for (int i = sil; i < _size; ++i) {
		_data[j] = _data[i];
		++j;
	}

	_size -= length;

	_data[_size] = '\0';
}

void String::clear() {
	_size = 0;
}

bool String::empty() const {
	return _size == 0;
}

char String::get(const int index) {
	return _data[index];
}

const char String::get(const int index) const {
	return _data[index];
}

void String::set(const int index, const char value) {
	_data[index] = value;
}

int String::size() const {
	return _size;
}

int String::capacity() const {
	return _actual_size;
}

void String::ensure_capacity(const int capacity) {
	if (capacity < _actual_size) {
		return;
	}

	int tsize = capacity + _grow_by;

	char *nd = new char[tsize];

	if (_data) {
		for (int i = 0; i < _size; ++i) {
			nd[i] = _data[i];
		}

		delete[] _data;
	}

	_data = nd;
	_actual_size = tsize;
}

void String::resize(const int s) {
	ensure_capacity(s + 1); // +1 for the null terminator

	_size = s;

	_data[_size] = '\0';
}

int String::find(const char val, const int from) const {
	for (int i = from; i < _size; ++i) {
		if (_data[i] == val) {
			return i;
		}
	}

	return -1;
}

int String::find(const String &val, const int from) const {
	int ve = _size - val.size();

	for (int i = from; i < ve; ++i) {
		bool found = true;
		for (int j = 0; j < val.size(); ++j) {
			if (_data[i + j] != val[j]) {
				found = false;
				break;
			}
		}

		if (found) {
			return i;
		}
 	}

	return -1;
}

void String::get_substr(char *into_buf, const int start_index, const int len) {
	ERR_FAIL_INDEX(start_index + len - 1, _size);

	int j = 0;
	for (int i = start_index; i < start_index + len; ++i) {
		into_buf[j++] = _data[i];
	}
}

void String::get_substr_nt(char *into_buf, const int start_index, const int len) {
	ERR_FAIL_INDEX(start_index + len - 1, _size);

	int j = 0;
	for (int i = start_index; i < start_index + len; ++i) {
		into_buf[j++] = _data[i];
	}

	into_buf[len + 1] = '\0';
}

String String::substr(const int start_index, const int len) {
	ERR_FAIL_INDEX_V(start_index, _size, String());

	int sil = start_index + len;

	ERR_FAIL_INDEX_V(sil, _size + 1, String());

	String str;
	str.ensure_capacity(len + 1);
	for (int i = start_index; i < sil; ++i) {
		str._data[str._size++] = _data[i];
	}

	str._data[str._size] = '\0';

	return str;
}

void String::replace_from(const int start_index, const int length, const String &with) {
	ERR_FAIL_INDEX(start_index, _size);

	int sil = start_index + length;

	ERR_FAIL_INDEX(sil, _size + 1);

	if (length < with.size()) {
		int loffs = with.size() - length;

		ensure_capacity(_size + loffs + 1);

		_size += loffs;
		_data[_size] = '\0';

		for (int i = _size - 1; i > start_index + loffs; --i) {
			_data[i] = _data[i - loffs];
		}
	} else if (length > with.size()) {
		int loffs = length - with.size();

		for (int i = start_index + with.size(); i < _size; ++i) {
			_data[i] = _data[i + loffs];
		}

		_size -= loffs;
	}

	for (int i = 0; i < length; ++i) {
		_data[i + start_index] = with._data[i];
	}
}

void String::replace(const String &find_str, const String &with) {
	if (empty()) {
		return;
	}

	if (find_str.empty())
		return;

	int start_pos = 0;
	while ((start_pos = find(find_str, start_pos)) != -1) {
		replace_from(start_pos, find_str.size(), with);
		start_pos += with.size();
	}
}

int String::compare(const String &other) const {
	if (size() < other.size()) {
		return 1;
	} else if (size() > other.size()) {
		return 2;
	} else {
		for (int i = 0; i < _size; ++i) {
			if (_data[i] < other._data[i]) {
				return 1;
			} else if (_data[i] > other._data[i]) {
				return 2;
			}
		}

		return 0;
	}
}

uint8_t String::read_uint8_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index, _size, 0);

	if (advance_index) {
		return static_cast<uint8_t>(_data[index++]);
	} else {
		return static_cast<uint8_t>(_data[index]);
	}
}
uint16_t String::read_uint16_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index + 1, _size, 0);

	char carr[3];
	char *p = carr;

	//printf("%u %u\n", static_cast<uint8_t>(p[0]),  static_cast<uint8_t>(p[1]));

	get_substr_nt(p, index, 2);

	const uint16_t *vp = static_cast<const uint16_t *>((void *)p);

	if (advance_index) {
		index += 2;
	}

	return *vp;
}
uint32_t String::read_uint32_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index + 3, _size, 0);

	char carr[5];
	char *p = carr;

	get_substr_nt(p, index, 4);

	const uint32_t *vp = static_cast<const uint32_t *>((void *)p);

	if (advance_index) {
		index += 4;
	}

	return *vp;
}
uint64_t String::read_uint64_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index + 7, _size, 0);

	char carr[9];
	char *p = carr;

	get_substr_nt(p, index, 8);

	const uint64_t *vp = static_cast<const uint64_t *>((void *)p);

	if (advance_index) {
		index += 8;
	}

	return *vp;
}

int8_t String::read_int8_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index, _size, 0);

	if (advance_index) {
		return static_cast<int8_t>(_data[index++]);
	} else {
		return static_cast<int8_t>(_data[index]);
	}
}
int16_t String::read_int16_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index + 1, _size, 0);

	char carr[3];
	char *p = carr;

	//printf("%u %u\n", static_cast<uint8_t>(p[0]),  static_cast<uint8_t>(p[1]));

	get_substr_nt(p, index, 2);

	const int16_t *vp = static_cast<const int16_t *>((void *)p);

	if (advance_index) {
		index += 2;
	}

	return *vp;
}
int32_t String::read_int32_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index + 3, _size, 0);

	char carr[5];
	char *p = carr;

	get_substr_nt(p, index, 4);

	const int32_t *vp = static_cast<const int32_t *>((void *)p);

	if (advance_index) {
		index += 4;
	}

	return *vp;
}
int64_t String::read_int64_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index + 7, _size, 0);

	char carr[9];
	char *p = carr;

	get_substr_nt(p, index, 8);

	const int64_t *vp = static_cast<const int64_t *>((void *)p);

	if (advance_index) {
		index += 8;
	}

	return *vp;
}

void String::append_uint8_bytes(const uint8_t val) {
	ensure_capacity(_size + 1);

	_data[_size++] = val;
	_data[_size] = '\0';
}
void String::append_uint16_bytes(const uint16_t val) {
	ensure_capacity(_size + 2);

	const char *vp = static_cast<const char *>((void *)&val);

	//printf("a %u %u\n", static_cast<uint8_t>(vp[0]),  static_cast<uint8_t>(vp[1]));

	memcpy(&_data[_size], vp, 2);

	_size += 2;

	_data[_size] = '\0';
}
void String::append_uint32_bytes(const uint32_t val) {
	ensure_capacity(_size + 4);

	const char *vp = static_cast<const char *>((void *)&val);

	memcpy(&_data[_size], vp, 4);

	_size += 4;

	_data[_size] = '\0';
}
void String::append_uint64_bytes(const uint64_t val) {
	ensure_capacity(_size + 8);

	const char *vp = static_cast<const char *>((void *)&val);

	memcpy(&_data[_size], vp, 8);

	_size += 8;

	_data[_size] = '\0';
}

void String::append_int8_bytes(const int8_t val) {
	ensure_capacity(_size + 1);

	_data[_size++] = val;
	_data[_size] = '\0';
}
void String::append_int16_bytes(const int16_t val) {
	ensure_capacity(_size + 2);

	const char *vp = static_cast<const char *>((void *)&val);

	//printf("a %u %u\n", static_cast<uint8_t>(vp[0]),  static_cast<uint8_t>(vp[1]));

	memcpy(&_data[_size], vp, 2);

	_size += 2;

	_data[_size] = '\0';
}
void String::append_int32_bytes(const int32_t val) {
	ensure_capacity(_size + 4);

	const char *vp = static_cast<const char *>((void *)&val);

	memcpy(&_data[_size], vp, 4);

	_size += 4;

	_data[_size] = '\0';
}
void String::append_int64_bytes(const int64_t val) {
	ensure_capacity(_size + 8);

	const char *vp = static_cast<const char *>((void *)&val);

	memcpy(&_data[_size], vp, 8);

	_size += 8;

	_data[_size] = '\0';
}

float String::read_float_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index + 3, _size, 0);

	char carr[5];
	char *p = carr;

	get_substr_nt(p, index, 4);

	const float *vp = static_cast<const float *>((void *)p);

	if (advance_index) {
		index += 4;
	}

	return *vp;
}
void String::append_float_bytes(const float val) {
	ensure_capacity(_size + 4);

	const char *vp = static_cast<const char *>((void *)&val);

	memcpy(&_data[_size], vp, 4);

	_size += 4;

	_data[_size] = '\0';
}

double String::read_double_bytes_at(int &index, bool advance_index) {
	ERR_FAIL_INDEX_V(index + 7, _size, 0);

	char carr[9];
	char *p = carr;

	get_substr_nt(p, index, 8);

	const double *vp = static_cast<const double *>((void *)p);

	if (advance_index) {
		index += 8;
	}

	return *vp;
}
void String::append_double_bytes(const double val) {
	ensure_capacity(_size + 8);

	const char *vp = static_cast<const char *>((void *)&val);

	memcpy(&_data[_size], vp, 8);

	_size += 8;

	_data[_size] = '\0';
}

void String::append_str(const char *str) {
	if (str == nullptr) {
		return;
	}

	int i = 0;

	while (str[i] != '\0') {
		push_back(str[i++]);
	}
}

void String::append_str(const String &other) {
	ensure_capacity(_size + other._size + 1); // +1 for the null terminator

	for (int i = 0; i < other._size; ++i) {
		_data[_size++] = other._data[i];
	}

	_data[_size] = '\0';
}

void String::append_str(const std::string &str) {
	ensure_capacity(_size + str.size() + 1); // +1 for the null terminator

	for (int i = 0; i < str.size(); ++i) {
		_data[_size++] = str[i];
	}

	_data[_size] = '\0';
}

float String::to_float() {
	return atof(c_str());
}

double String::to_double() {
	return atof(c_str());
}

int String::to_int() {
	return atoi(c_str());
}

uint32_t String::to_uint() {
	return static_cast<uint32_t>(atoll(c_str()));
}

std::string String::to_string() const {
	return std::string(c_str());
}

void String::print() const {
	::printf("%s\n", c_str());
}

char *String::c_str() {
	return _data;
}

const char *String::c_str() const {
	return _data;
}

char *String::dataw() {
	return _data;
}

const char *String::data() const {
	return _data;
}

const char String::operator[](const int index) const {
	return _data[index];
}

char &String::operator[](const int index) {
	return _data[index];
}

String &String::operator+=(const String &b) {
	ensure_capacity(_size + b._size + 1); // +1 for the null terminator

	for (int i = 0; i < b._size; ++i) {
		_data[_size++] = b._data[i];
	}

	_data[_size] = '\0';

	return *this;
}

String &String::operator+=(const char chr) {
	push_back(chr);

	return *this;
}

String &String::operator+=(const char *p_c_str) {
	int i = 0;
	while (p_c_str[i] != '\0') {
		push_back(p_c_str[i]);
		++i;
	}

	return *this;
}

String &String::operator+=(const std::string &b) {
	append_str(b);

	return *this;
}
String operator+(String lhs, const String &rhs) {
	lhs.append_str(rhs);

	return lhs;
}

String operator+(String lhs, const char *rhs) {
	lhs.append_str(rhs);

	return lhs;
}

String operator+(String lhs, const char rhs) {
	lhs.push_back(rhs);

	return lhs;
}

String operator+(String lhs, const std::string &rhs) {
	lhs.append_str(rhs);

	return lhs;
}

bool operator==(const String &a, const String &b) {
	if (a._size != b._size) {
		return false;
	}

	for (int i = 0; i < a._size; ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}

	return true;
}

bool operator!=(const String &a, const String &b) {
	return !(a == b);
}

bool operator==(const String &a, const char *b) {
	int i = 0;
	while (b[i] != '\0' && i < a._size) {
		if (a[i] != b[i]) {
			return false;
		}

		++i;
	}

	if (i != a._size) {
		return false;
	}

	return true;
}

bool operator!=(const String &a, const char *b) {
	return !(a == b);
}

bool operator==(const char *b, const String &a) {
	int i = 0;
	while (b[i] != '\0' && i < a._size) {
		if (a[i] != b[i]) {
			return false;
		}

		++i;
	}

	if (i != a._size) {
		return false;
	}

	return true;
}

bool operator!=(const char *b, const String &a) {
	return !(a == b);
}

bool operator==(const String &a, std::string &b) {
	if (a._size != b.size()) {
		return false;
	}

	char *bp = &b[0];

	for (int i = 0; i < a._size; ++i) {
		if (a[i] != bp[i]) {
			return false;
		}
	}

	return true;
}
bool operator!=(const String &a, std::string &b) {
	return !(a == b);
}

bool operator==(std::string &b, const String &a) {
	return (a == b);
}
bool operator!=(std::string &b, const String &a) {
	return !(a == b);
}

bool operator<(const String &a, const String &b) {
	return a.compare(b) == 1;
}
bool operator>(const String &a, const String &b) {
	return a.compare(b) == 2;
}
bool operator<=(const String &a, const String &b) {
	int c = a.compare(b);

	return c == 0 || c == 1;
}
bool operator>=(const String &a, const String &b) {
	int c = a.compare(b);

	return c == 0 || c == 2;
}

String &String::operator=(const String &other) {
	clear();

	append_str(other);

	return *this;
}

String &String::operator=(const std::string &other) {
	clear();

	append_str(other);

	return *this;
}

String &String::operator=(const char *other) {
	clear();

	append_str(other);

	return *this;
}

String::String() {
	_data = nullptr;
	_actual_size = 0;
	_size = 0;
	_grow_by = 100;

	ensure_capacity(100);

	_data[0] = '\0';
}

String::String(const String &other) {
	_data = nullptr;
	_actual_size = 0;
	_size = 0;
	_grow_by = 100;

	//+1 for the null terminator in case its needed
	ensure_capacity(other.size() + 1);

	for (int i = 0; i < other._size; ++i) {
		_data[i] = other._data[i];
	}
	_size = other._size;
	_data[other._size] = '\0';
}

String::String(const String &other, int grow_by) {
	_data = nullptr;
	_actual_size = 0;
	_size = 0;
	_grow_by = grow_by;

	//+1 for the null terminator in case its needed
	ensure_capacity(other.size() + 1);

	for (int i = 0; i < other._size; ++i) {
		_data[i] = other._data[i];
	}

	_size = other._size;

	_data[_size] = '\0';
}

String::String(const char *p_c_str) {
	_data = nullptr;
	_actual_size = 0;
	_size = 0;
	_grow_by = 100;

	append_str(p_c_str);
}

String::String(const char *p_c_str, const int grow_by) {
	_data = nullptr;
	_actual_size = 0;
	_size = 0;
	_grow_by = grow_by;

	append_str(p_c_str);
}

String::String(int prealloc) {
	_data = nullptr;
	_actual_size = 0;
	_size = 0;
	_grow_by = 100;

	ensure_capacity(prealloc);

	_data[0] = '\0';
}

String::String(int prealloc, int grow_by) {
	_data = nullptr;
	_actual_size = 0;
	_size = 0;
	_grow_by = grow_by;

	ensure_capacity(prealloc);

	_data[0] = '\0';
}

String::String(const std::string &str) {
	_data = nullptr;
	_actual_size = 0;
	_size = 0;
	_grow_by = 100;

	append_str(str);
}

String::~String() {
	if (_data) {
		delete[] _data;
		_data = nullptr;
	}
}
