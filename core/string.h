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
	void erase(const int start_index, const int length);
	void clear();
	bool empty() const;
	char get(const int index);
	const char get(const int index) const;
	void set(const int index, const char value);

	int size() const;
	int capacity() const;
	void ensure_capacity(const int capacity);
	void resize(const int s);
	int find(const char val, const int from = 0) const;
	int find(const String &val, const int from = 0) const;
	void get_substr(char *into_buf, const int start_index, const int len);
	void get_substr_nt(char *into_buf, const int start_index, const int len);
	String substr(const int start_index, const int len) const;

	void replace_from(const int start_index, const int length, const String &with);
	void replace(const String &find_str, const String &with);

	int compare(const String &other) const;

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

	float to_float() const;
	double to_double() const;
	int to_int() const;

	bool is_numeric() const;
	bool is_int() const;
	bool is_uint() const;
	bool is_zero() const;

	uint32_t to_uint() const;
	std::string to_string() const;
	void print() const;
	
	static String bool_num(bool val);
	static String bool_str(bool val);

	//Taken from the Godot Engine (MIT License)
	//Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
	//Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static String num(double p_num, int p_decimals = -1);
	static String num_scientific(double p_num);
	static String num_real(double p_num, bool p_trailing = true);
	static String num_int64(int64_t p_num, int base = 10, bool capitalize_hex = false);
	static String num_uint64(uint64_t p_num, int base = 10, bool capitalize_hex = false);
	static String chr(char32_t p_char);

	//Taken from the Godot Engine (MIT License)
	//Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
	//Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	String ascii(bool p_allow_extended = false) const;
	String utf8() const;
	bool parse_utf8(const char *p_utf8, int p_len = -1); //return true on error
	static String utf8(const char *p_utf8, int p_len = -1);

	char *c_str();
	const char *c_str() const;

	char *dataw();
	const char *data() const;

	const char operator[](const int index) const;
	char &operator[](const int index);

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

	friend bool operator<(const String &a, const String &b);
	friend bool operator>(const String &a, const String &b);
	friend bool operator<=(const String &a, const String &b);
	friend bool operator>=(const String &a, const String &b);

	operator std::string() { return to_string(); }
	operator std::string() const { return to_string(); }

	String& operator=(const String &other);
	String& operator=(const std::string &other);
	String& operator=(const char* other);

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
