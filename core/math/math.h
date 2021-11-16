#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <cstdint>
#include "math_defs.h"
#include "core/typedefs.h"

#define MATH_PI 3.1415926535897932384626433833
#define EPSILON 0.00001

class Math {
public:
	static const uint64_t RANDOM_32BIT_MAX = 0xFFFFFFFF;

	inline static float sin(const float x) { return ::sinf(x); }
	inline static double sin(const double x) { return ::sin(x); }

	inline static float cos(const float x) { return ::cosf(x); }
	inline static double cos(const double x) { return ::cos(x); }

	inline static float tan(const float x) { return ::tanf(x); }
	inline static double tan(const double x) { return ::tan(x); }

	inline static float asin(const float x) { return ::asinf(x); }
	inline static double asin(const double x) { return ::asin(x); }

	inline static float acos(const float x) { return ::acosf(x); }
	inline static double acos(const double x) { return ::acos(x); }

	inline static float atan(const float x) { return ::atanf(x); }
	inline static double atan(const double x) { return ::atan(x); }

	inline static float atan2(const float x, const float y) { return ::atan2f(x, y); }
	inline static double atan2(const double x, const float y) { return ::atan2(x, y); }

	inline static float sqrt(const float x) { return ::sqrtf(x); }
	inline static double sqrt(const double x) { return ::sqrt(x); }

	inline static float fmod(const float x, const float y) { return ::fmodf(x, y); }
	inline static double fmod(const double x, const float y) { return ::fmod(x, y); }

	inline static float floor(const float x) { return ::floorf(x); }
	inline static double floor(const double x) { return ::floor(x); }

	inline static float ceil(const float x) { return ::ceilf(x); }
	inline static double ceil(const double x) { return ::ceil(x); }

	inline static float pow(const float x, const float y) { return ::powf(x, y); }
	inline static double pow(const double x, const float y) { return ::pow(x, y); }

	inline static float log(const float x) { return ::logf(x); }
	inline static double log(const double x) { return ::log(x); }

	static float inv_sqrt(const float x);
	static float fast_inv_sqrt(const float x);

	inline static float abs(const float x) { return x > 0 ? x : -x; }
	inline static double abs(const double x) { return x > 0 ? x : -x; }
	inline static int absi(const int x) { return x > 0 ? x : -x; }

	inline static float deg2rad(const float x) { return x * MATH_PI / 180.0; }
	inline static double deg2rad(const double x) { return x * MATH_PI / 180.0; }
	inline static int deg2rad(const int x) { return x * MATH_PI / 180.0; }

	inline static float rad2deg(const float x) { return x * 180.0 / MATH_PI; }
	inline static double rad2deg(const double x) { return x * 180.0 / MATH_PI; }
	inline static int rad2deg(const int x) { return x * 180.0 / MATH_PI; }

	inline static double lerp(double from, double to, double weight) { return from + (to - from) * weight; }
	inline static float lerp(float from, float to, float weight) { return from + (to - from) * weight; }

	static float is_equal_approx(const float a, const float b);
	static float is_zero_approx(const float a);

    //Taken from the Godot Engine (MIT License)
    //Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
    //Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static _ALWAYS_INLINE_ bool is_equal_approx_ratio(float a, float b, float epsilon = CMP_EPSILON, float min_epsilon = CMP_EPSILON) {
		// this is an approximate way to check that numbers are close, as a ratio of their average size
		// helps compare approximate numbers that may be very big or very small
		real_t diff = abs(a - b);
		if (diff == 0.0 || diff < min_epsilon) {
			return true;
		}
		real_t avg_size = (abs(a) + abs(b)) / 2.0;
		diff /= avg_size;
		return diff < epsilon;
	}

	static void seed(const unsigned int s);
	static void randomize();

	static int rand();
	static float randf();
	static double randd();

	static int rand(const int m);

	static int rand(const int from, const int to);
	static float rand(const float from, const float to);
	static float rand(const double from, const double to);

    //Taken from the Godot Engine (MIT License)
    //Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
    //Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static bool is_nan(double p_val) {
#ifdef _MSC_VER
		return _isnan(p_val);
#elif defined(__GNUC__) && __GNUC__ < 6
		union {
			uint64_t u;
			double f;
		} ieee754;
		ieee754.f = p_val;
		// (unsigned)(0x7ff0000000000001 >> 32) : 0x7ff00000
		return ((((unsigned)(ieee754.u >> 32) & 0x7fffffff) + ((unsigned)ieee754.u != 0)) > 0x7ff00000);
#else
		return isnan(p_val);
#endif
	}

    //Taken from the Godot Engine (MIT License)
    //Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
    //Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static bool is_nan(float p_val) {
#ifdef _MSC_VER
		return _isnan(p_val);
#elif defined(__GNUC__) && __GNUC__ < 6
		union {
			uint32_t u;
			float f;
		} ieee754;
		ieee754.f = p_val;
		// -----------------------------------
		// (single-precision floating-point)
		// NaN : s111 1111 1xxx xxxx xxxx xxxx xxxx xxxx
		//     : (> 0x7f800000)
		// where,
		//   s : sign
		//   x : non-zero number
		// -----------------------------------
		return ((ieee754.u & 0x7fffffff) > 0x7f800000);
#else
		return isnan(p_val);
#endif
	}

    //Taken from the Godot Engine (MIT License)
    //Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
    //Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static bool is_inf(double p_val) {
#ifdef _MSC_VER
		return !_finite(p_val);
// use an inline implementation of isinf as a workaround for problematic libstdc++ versions from gcc 5.x era
#elif defined(__GNUC__) && __GNUC__ < 6
		union {
			uint64_t u;
			double f;
		} ieee754;
		ieee754.f = p_val;
		return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) == 0x7ff00000 &&
			   ((unsigned)ieee754.u == 0);
#else
		return isinf(p_val);
#endif
	}

    //Taken from the Godot Engine (MIT License)
    //Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
    //Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static bool is_inf(float p_val) {
#ifdef _MSC_VER
		return !_finite(p_val);
// use an inline implementation of isinf as a workaround for problematic libstdc++ versions from gcc 5.x era
#elif defined(__GNUC__) && __GNUC__ < 6
		union {
			uint32_t u;
			float f;
		} ieee754;
		ieee754.f = p_val;
		return (ieee754.u & 0x7fffffff) == 0x7f800000;
#else
		return isinf(p_val);
#endif
	}
};

#endif
