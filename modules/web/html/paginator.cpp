#include "paginator.h"

#include "core/math/math_funcs.h"
#include "html_builder.h"

int HTMLPaginator::get_item_count() const {
	return _item_count;
}
void HTMLPaginator::set_item_count(const int val) {
	_item_count = val;
	_page_count = Math::floorf_int(Math::divf(val, _max_visible_links));
}

int HTMLPaginator::get_page_count() const {
	return _page_count;
}
void HTMLPaginator::set_page_count(const int val) {
	_page_count = val;
	_item_count = val * _max_visible_links;
}

int HTMLPaginator::get_max_visible_links() const {
	return _max_visible_links;
}
void HTMLPaginator::set_max_visible_links(const int val) {
	_max_visible_links = val;

	set_item_count(_item_count);
}

void HTMLPaginator::start() {
	_current_page_index = 0;
}
String HTMLPaginator::next() {
	++_current_page_index;

	return get_current();
}
String HTMLPaginator::get_current() {
	return get_pagination_for_indx(_current_page_index);
}

String HTMLPaginator::get_pagination_for_indx(const int page_index) {
	if (use_links_array) {
		return render_links(renderer, page_index);
	} else {
		return render_indexed(renderer, page_index);
	}
}
String HTMLPaginator::get_pagination_for_num(const int page_num) {
	return get_pagination_for_indx(page_num - 1);
}

String HTMLPaginator::render_indexed(Ref<HTMLPaginator> target, const int page_index) {
	if (!target.is_valid()) {
		target = Ref<HTMLPaginator>(this);
	}

	String s = target->base_url;
	if (s.size() > 0 && s[s.size() - 1] != '/') {
		s += '/';
	}

	int max_visible_links = target->_max_visible_links;
	int page_count = target->_page_count;
	const String &tclass_main_ul = target->class_main_ul;
	const String &tclass_enabled_li = target->class_enabled_li;
	const String &tclass_disabled_li = target->class_disabled_li;
	const String &ttext_next_link = target->text_next_link;
	const String &ttext_prev_link = target->text_prev_link;

	int starti = page_index - max_visible_links / 2;
	int toi = page_index + max_visible_links / 2;

	if (starti < 0) {
		toi += -starti;
		starti = 0;
	}

	if (toi > page_count) {
		starti -= toi - page_count;
		toi = page_count;

		if (starti < 0) {
			starti = 0;
		}
	}

	// int toi = max > max_visible_links ? max_visible_links : max;

	HTMLBuilder b;

	b.ul()->clsse(tclass_main_ul);

	if (page_count != 0 && page_index != 0) {
		b.li()->clsse(tclass_enabled_li);
		{
			b.a()->href(s + itos(page_index - 1))->rel_prev()->f()->w(ttext_prev_link)->ca();
		}
		b.cli();
	} else {
		b.li()->clsse(tclass_disabled_li)->f()->w(ttext_prev_link)->cli();
	}

	if (starti != toi) {
		for (uint32_t i = starti; i < toi; ++i) {
			if (i != page_index) {
				b.li()->clsse(tclass_enabled_li);
				{
					b.a()->href(s + itos(i + 1))->f()->w(itos(i + 1))->ca();
				}
				b.cli();
			} else {
				b.li()->clsse(tclass_disabled_li)->f()->w(itos(i + 1))->cli();
			}
		}
	} else {
		b.li()->clsse(tclass_disabled_li)->f()->w(itos(1))->cli();
	}

	if (page_count != 0 && page_index < page_count - 1) {
		b.li()->clsse(tclass_enabled_li);
		{
			b.a()->href(s + itos(page_index + 2))->rel_next()->f()->w(ttext_next_link)->ca();
		}
		b.cli();
	} else {
		b.li()->clsse(tclass_disabled_li)->f()->w(ttext_next_link)->cli();
	}

	b.cul();

	return b.result;
}
String HTMLPaginator::render_links(Ref<HTMLPaginator> target, const int page_index) {
	if (!target.is_valid()) {
		target = Ref<HTMLPaginator>(this);
	}

	if (page_index < 0) {
		return "";
	}

	String s = target->base_url;
	if (s.size() > 0 && s[s.size() - 1] != '/') {
		s += '/';
	}

	uint32_t max = target->links.size();

	int max_visible_links = target->_max_visible_links;
	//int page_count = target->_page_count;
	const String &tclass_main_ul = target->class_main_ul;
	const String &tclass_enabled_li = target->class_enabled_li;
	const String &tclass_disabled_li = target->class_disabled_li;
	const String &ttext_next_link = target->text_next_link;
	const String &ttext_prev_link = target->text_prev_link;

	int starti = page_index - max_visible_links / 2;
	int toi = page_index + max_visible_links / 2;

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

	b.ul()->clsse(tclass_main_ul);

	if (max != 0 && page_index != 0) {
		b.li()->clsse(tclass_enabled_li);
		{
			b.a()->href(s + target->links[page_index - 1])->rel_prev()->f()->w(ttext_prev_link)->ca();
		}
		b.cli();
	} else {
		b.li()->clsse(tclass_disabled_li)->f()->w(ttext_prev_link)->cli();
	}

	if (starti != toi) {
		for (uint32_t i = starti; i < toi; ++i) {
			if (i != page_index) {
				b.li()->clsse(tclass_enabled_li);
				{
					b.a()->href(s + target->links[i])->f()->w(itos(i + 1))->ca();
				}
				b.cli();
			} else {
				b.li()->clsse(tclass_disabled_li)->f()->w(itos(i + 1))->cli();
			}
		}
	} else {
		b.li()->clsse(tclass_disabled_li)->f()->w(itos(1))->cli();
	}

	if (max != 0 && page_index < max - 1) {
		b.li()->clsse(tclass_enabled_li);
		{
			b.a()->href(s + target->links[page_index + 1])->rel_next()->f()->w(ttext_next_link)->ca();
		}
		b.cli();
	} else {
		b.li()->clsse(tclass_disabled_li)->f()->w(ttext_next_link)->cli();
	}

	b.cul();

	return b.result;
}

HTMLPaginator::HTMLPaginator() {
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

HTMLPaginator::~HTMLPaginator() {
	renderer.unref();
}
