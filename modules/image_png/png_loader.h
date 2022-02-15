#ifndef PNG_LOADER_H
#define PNG_LOADER_H

#include "core/containers/vector.h"
#include "core/error_list.h"
#include "core/image/image.h"

class PNGLoader {
public:
	static Error load_image(Ref<Image> p_image, const char* f, bool p_force_linear);
	static Error save_image(const char* f, const Ref<Image> &p_img);

	// Taken from the Godot Engine (MIT License)
	// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
	// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static Ref<Image> load_mem_png(const uint8_t *p_png, int p_size);

	// Taken from the Godot Engine (MIT License)
	// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
	// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static Error load_image(Ref<Image> p_image, const String &f, bool p_force_linear);

	// Taken from the Godot Engine (MIT License)
	// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
	// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static Error save_image(const String &p_path, const Ref<Image> &p_img);

	// Taken from the Godot Engine (MIT License)
	// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
	// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
	static Vector<uint8_t> save_image_to_buffer(const Ref<Image> &p_img);

	PNGLoader();
};

#endif // RESOURCE_SAVER_PNG_H
