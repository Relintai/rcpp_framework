#ifndef STRING_H
#define STRING_H

#include <inttypes.h>

#include <string>

class String {
public:
	void push_back(const char element);
	void pop_back();
	void remove(const int index);
	void erase(const char element);
	void clear();
	bool empty() const;
	char get(const int index);
	const char get(const int index) const;
	void set(const int index, const char value);

	int size() const;
	int capacity() const;
	void ensure_capacity(const int capacity);
	void resize(const int s);
	int find(const char val) const;
	void get_substr(char *into_buf, const int start_index, const int len);
	void get_substr_nt(char *into_buf, const int start_index, const int len);

	uint8_t read_uint8_bytes_at(int &index, bool advance_index = true);
	uint16_t read_uint16_bytes_at(int &index, bool advance_index = true);
	uint32_t read_uint32_bytes_at(int &index, bool advance_index = true);
	uint64_t read_uint64_bytes_at(int &index, bool advance_index = true);

	int8_t read_int8_bytes_at(int &index, bool advance_index = true);
	int16_t read_int16_bytes_at(int &index, bool advance_index = true);
	int32_t read_int32_bytes_at(int &index, bool advance_index = true);
	int64_t read_int64_bytes_at(int &index, bool advance_index = true);

	void append_uint8_bytes(const uint8_t val);
	void append_uint16_bytes(const uint16_t val);
	void append_uint32_bytes(const uint32_t val);
	void append_uint64_bytes(const uint64_t val);

	void append_int8_bytes(const int8_t val);
	void append_int16_bytes(const int16_t val);
	void append_int32_bytes(const int32_t val);
	void append_int64_bytes(const int64_t val);

	float read_float_bytes_at(int &index, bool advance_index = true);
    void append_float_bytes(const float val);
    double read_double_bytes_at(int &index, bool advance_index = true);
    void append_double_bytes(const double val);

	void append_str(const char* str);
	void append_str(const String &other);
	void append_str(const std::string &str);

	float to_float();
	double to_double();
	int to_int();
	uint32_t to_uint();
	std::string to_string() const;

	char *c_str();
	const char *c_str() const;

	char *dataw();
	const char *data() const;

	const char operator[](const int index) const;
	char operator[](const int index);

	String &operator+=(const String &b);
	String &operator+=(const char chr);
	String &operator+=(const char *p_c_str);
	String &operator+=(const std::string &b);

	friend String operator+(String lhs, const String &rhs);
	friend String operator+(String lhs, const char *rhs);
	friend String operator+(String lhs, const char rhs);
	friend String operator+(String lhs, const std::string &rhs);

	friend bool operator==(const String &a, const String &b);
	friend bool operator!=(const String &a, const String &b);

	friend bool operator==(const String &a, const char *b);
	friend bool operator!=(const String &a, const char *b);

	friend bool operator==(const char *b, const String &a);
	friend bool operator!=(const char *b, const String &a);

	friend bool operator==(const String &a, std::string &b);
	friend bool operator!=(const String &a, std::string &b);

	friend bool operator==(std::string &b, const String &a);
	friend bool operator!=(std::string &b, const String &a);

	operator std::string() { return to_string(); }
	operator std::string() const { return to_string(); }

	String();
	String(const String &other);
	String(const String &other, const int grow_by);
	String(const char* p_c_str);
	String(const char* p_c_str, const int grow_by);
	String(const int prealloc);
	String(const int prealloc, const int grow_by);
	String(const std::string &str);
	~String();

private:
	char *_data;
	int _actual_size;
	int _size;
	int _grow_by;
};

#endif
