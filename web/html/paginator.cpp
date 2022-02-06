#include "paginator.h"

#include "core/math/math.h"
#include "html_builder.h"

int Paginator::get_item_count() const {
	return _item_count;
}
void Paginator::set_item_count(const int val) {
	_item_count = val;
	_page_count = Math::floorf_int(Math::divf(val, _max_visible_links));
}

int Paginator::get_page_count() const {
	return _page_count;
}
void Paginator::set_page_count(const int val) {
	_page_count = val;
	_item_count = val * _max_visible_links;
}

int Paginator::get_max_visible_links() const {
	return _max_visible_links;
}
void Paginator::set_max_visible_links(const int val) {
	_max_visible_links = val;

	set_item_count(_item_count);
}

void Paginator::start() {
	_current_page_index = 0;
}
String Paginator::next() {
	++_current_page_index;

	return get_current();
}
String Paginator::get_current() {
	return get_pagination_for_indx(_current_page_index);
}

String Paginator::get_pagination_for_indx(const int page_index) {
	if (use_links_array) {
		return render_links(page_index);
	} else {
		return render_indexed(page_index);
	}
}
String Paginator::get_pagination_for_num(const int page_num) {
	return get_pagination_for_indx(page_num - 1);
}

String Paginator::render_indexed(const int page_index) {
	String s = base_url;
	if (s.size() > 0 && s[s.size() - 1] != '/') {
		s += '/';
	}

	int starti = page_index - _max_visible_links / 2;
	int toi = page_index + _max_visible_links / 2;

	if (starti < 0) {
		toi += -starti;
		starti = 0;
	}

	if (toi > _page_count) {
		starti -= toi - _page_count;
		toi = _page_count;

		if (starti < 0) {
			starti = 0;
		}
	}

	// int toi = max > max_visible_links ? max_visible_links : max;

	HTMLBuilder b;

	b.ul()->clsse(class_main_ul);

	if (_page_count != 0 && page_index != 0) {
		b.li()->clsse(class_enabled_li);
		{
			b.a()->href(s + std::to_string(page_index - 1))->rel_prev()->f()->w(text_prev_link)->ca();
		}
		b.cli();
	} else {
		b.li()->clsse(class_disabled_li)->f()->w(text_prev_link)->cli();
	}

	if (starti != toi) {
		for (uint32_t i = starti; i < toi; ++i) {
			if (i != page_index) {
				b.li()->clsse(class_enabled_li);
				{
					b.a()->href(s + std::to_string(i + 1))->f()->w(std::to_string(i + 1))->ca();
				}
				b.cli();
			} else {
				b.li()->clsse(class_disabled_li)->f()->w(std::to_string(i + 1))->cli();
			}
		}
	} else {
		b.li()->clsse(class_disabled_li)->f()->w(std::to_string(1))->cli();
	}

	if (_page_count != 0 && page_index < _page_count - 1) {
		b.li()->clsse(class_enabled_li);
		{
			b.a()->href(s + std::to_string(page_index + 2))->rel_next()->f()->w(text_next_link)->ca();
		}
		b.cli();
	} else {
		b.li()->clsse(class_disabled_li)->f()->w(text_next_link)->cli();
	}

	b.cul();

	return b.result;
}
String Paginator::render_links(const int page_index) {
	if (page_index < 0) {
		return "";
	}

	String s = base_url;
	if (s.size() > 0 && s[s.size() - 1] != '/') {
		s += '/';
	}

	uint32_t max = links.size();

	int starti = page_index - _max_visible_links / 2;
	int toi = page_index + _max_visible_links / 2;

	if (starti < 0) {
		toi += -starti;
		starti = 0;
	}

	if (toi > max) {
		starti -= toi - max;
		toi = max;

		if (starti < 0) {
			starti = 0;
		}
	}

	// int toi = max > max_visible_links ? max_visible_links : max;

	HTMLBuilder b;

	b.ul()->clsse(class_main_ul);

	if (max != 0 && page_index != 0) {
		b.li()->clsse(class_enabled_li);
		{
			b.a()->href(s + links[page_index - 1])->rel_prev()->f()->w(text_prev_link)->ca();
		}
		b.cli();
	} else {
		b.li()->clsse(class_disabled_li)->f()->w(text_prev_link)->cli();
	}

	if (starti != toi) {
		for (uint32_t i = starti; i < toi; ++i) {
			if (i != page_index) {
				b.li()->clsse(class_enabled_li);
				{
					b.a()->href(s + links[i])->f()->w(std::to_string(i + 1))->ca();
				}
				b.cli();
			} else {
				b.li()->clsse(class_disabled_li)->f()->w(std::to_string(i + 1))->cli();
			}
		}
	} else {
		b.li()->clsse(class_disabled_li)->f()->w(std::to_string(1))->cli();
	}

	if (max != 0 && page_index < max - 1) {
		b.li()->clsse(class_enabled_li);
		{
			b.a()->href(s + links[page_index + 1])->rel_next()->f()->w(text_next_link)->ca();
		}
		b.cli();
	} else {
		b.li()->clsse(class_disabled_li)->f()->w(text_next_link)->cli();
	}

	b.cul();

	return b.result;
}

Paginator::Paginator() {
	use_links_array = false;
	hide_if_one_page = false;

	_item_count = 0;
	_page_count = 0;
	_max_visible_links = 10;
	_current_page_index = 0;

	class_main_ul = "pagination";
	// class_enabled_li; -> no class by default
	class_disabled_li = "disabled";
	text_next_link = "next";
	text_prev_link = "previous";
}

Paginator::~Paginator() {
}