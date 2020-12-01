#ifndef OBJECT_H
#define OBJECT_H

#include <list>
#include <string>
#include <vector>

//taken from GodotEngine's object.h
#define RCPP_OBJECT(m_class, m_inherits)                                                                                                         \
private:                                                                                                                                         \
	void operator=(const m_class &p_rval) {}                                                                                                     \
	mutable std::stringName _class_name;                                                                                                         \
                                                                                                                                                 \
public:                                                                                                                                          \
	virtual std::string get_class() const override {                                                                                             \
		return std::string(#m_class);                                                                                                            \
	}                                                                                                                                            \
	static _FORCE_INLINE_ void *get_class_ptr_static() {                                                                                         \
		static int ptr;                                                                                                                          \
		return &ptr;                                                                                                                             \
	}                                                                                                                                            \
	static _FORCE_INLINE_ std::string get_class_static() {                                                                                       \
		return std::string(#m_class);                                                                                                            \
	}                                                                                                                                            \
	static _FORCE_INLINE_ std::string get_parent_class_static() {                                                                                \
		return m_inherits::get_class_static();                                                                                                   \
	}                                                                                                                                            \
	static void get_inheritance_list_static(std::list<std::string> *p_inheritance_list) {                                                        \
		m_inherits::get_inheritance_list_static(p_inheritance_list);                                                                             \
		p_inheritance_list->push_back(std::string(#m_class));                                                                                    \
	}                                                                                                                                            \
	static std::string inherits_static() {                                                                                                       \
		return std::string(#m_inherits);                                                                                                         \
	}                                                                                                                                            \
	virtual bool is_class(const std::string &p_class) const override { return (p_class == (#m_class)) ? true : m_inherits::is_class(p_class); }  \
	virtual bool is_class_ptr(void *p_ptr) const override { return (p_ptr == get_class_ptr_static()) ? true : m_inherits::is_class_ptr(p_ptr); } \
                                                                                                                                                 \
	static void get_valid_parents_static(std::list<std::string> *p_parents) {                                                                    \
		if (m_class::_get_valid_parents_static != m_inherits::_get_valid_parents_static) {                                                       \
			m_class::_get_valid_parents_static(p_parents);                                                                                       \
		}                                                                                                                                        \
                                                                                                                                                 \
		m_inherits::get_valid_parents_static(p_parents);                                                                                         \
	}                                                                                                                                            \
                                                                                                                                                 \
private:

class Object {
public:
	Object();
	virtual ~Object();
};

#endif