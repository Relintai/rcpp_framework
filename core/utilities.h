#ifndef UTILS_H
#define UTILS_H

#include "core/containers/vector.h"
#include "core/string.h"
#include "core/typedefs.h"

#include <string>
#include <vector>

class Utilities {
public:
	// Taken from trantor (MIT License) - Funcs.h
	// Copyright (c) 2018 An Tao
	static _ALWAYS_INLINE_ std::vector<std::string> splitString(const std::string &s,
			const std::string &delimiter,
			bool acceptEmptyString = false) {
		if (delimiter.empty())
			return std::vector<std::string>{};
		std::vector<std::string> v;
		size_t last = 0;
		size_t next = 0;
		while ((next = s.find(delimiter, last)) != std::string::npos) {
			if (next > last || acceptEmptyString)
				v.push_back(s.substr(last, next - last));
			last = next + delimiter.length();
		}
		if (s.length() > last || acceptEmptyString)
			v.push_back(s.substr(last));
		return v;
	}

protected:
};

#endif