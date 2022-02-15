
#include "png_loader.h"

#include "png_driver_common.h"

#include "core/os/directory.h"

// Taken from the Godot Engine (MIT License)
// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
Error PNGLoader::load_image(Ref<Image> p_image, const String &f, bool p_force_linear) {
	Ref<Directory> d;
	d.instance();

	Vector<uint8_t> file_buffer;
	d->read_file_into_bin(f, &file_buffer);

	return PNGDriverCommon::png_to_image(file_buffer.data(), file_buffer.size(), p_force_linear, p_image);
}

// Taken from the Godot Engine (MIT License)
// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
Ref<Image> PNGLoader::load_mem_png(const uint8_t *p_png, int p_size) {
	Ref<Image> img;
	img.instance();

	// the value of p_force_linear does not matter since it only applies to 16 bit
	Error err = PNGDriverCommon::png_to_image(p_png, p_size, false, img);
	ERR_FAIL_COND_V(err, Ref<Image>());

	return img;
}

// Taken from the Godot Engine (MIT License)
// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
Error PNGLoader::save_image(const String &p_path, const Ref<Image> &p_img) {
	Vector<uint8_t> buffer;
	Error err = PNGDriverCommon::image_to_png(p_img, buffer);

	ERR_FAIL_COND_V_MSG(err, err, "Can't convert image to PNG.");

	Ref<Directory> d;
	d.instance();

	err = d->write_file_bin(p_path, buffer);

	ERR_FAIL_COND_V_MSG(err, err, "Can't save PNG at path: " + p_path);

	return OK;
}

// Taken from the Godot Engine (MIT License)
// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
Vector<uint8_t> PNGLoader::save_image_to_buffer(const Ref<Image> &p_img) {
	Vector<uint8_t> buffer;
	Error err = PNGDriverCommon::image_to_png(p_img, buffer);

	ERR_FAIL_COND_V_MSG(err, Vector<uint8_t>(), "Can't convert image to PNG.");

	return buffer;
}

PNGLoader::PNGLoader() {
}