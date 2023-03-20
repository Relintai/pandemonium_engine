#include "paginator.h"

#include "core/math/math_funcs.h"
#include "html_builder.h"

String HTMLPaginator::get_base_url() {
	return base_url;
}
void HTMLPaginator::set_base_url(const String &val) {
	base_url = val;
}

PoolStringArray HTMLPaginator::get_links() {
	return links;
}
void HTMLPaginator::set_links(const PoolStringArray &val) {
	links = val;
}

bool HTMLPaginator::get_use_links_array() {
	return use_links_array;
}
void HTMLPaginator::set_use_links_array(const bool &val) {
	use_links_array = val;
}

bool HTMLPaginator::get_hide_if_one_page() {
	return hide_if_one_page;
}
void HTMLPaginator::set_hide_if_one_page(const bool &val) {
	hide_if_one_page = val;
}

String HTMLPaginator::get_class_main_ul() {
	return class_main_ul;
}
void HTMLPaginator::set_class_main_ul(const String &val) {
	class_main_ul = val;
}

String HTMLPaginator::get_class_enabled_li() {
	return class_enabled_li;
}
void HTMLPaginator::set_class_enabled_li(const String &val) {
	class_enabled_li = val;
}

String HTMLPaginator::get_class_disabled_li() {
	return class_disabled_li;
}
void HTMLPaginator::set_class_disabled_li(const String &val) {
	class_disabled_li = val;
}

String HTMLPaginator::get_text_next_link() {
	return text_next_link;
}
void HTMLPaginator::set_text_next_link(const String &val) {
	text_next_link = val;
}

String HTMLPaginator::get_text_prev_link() {
	return text_prev_link;
}
void HTMLPaginator::set_text_prev_link(const String &val) {
	text_prev_link = val;
}

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

Ref<HTMLPaginator> HTMLPaginator::get_renderer() {
	return renderer;
}
void HTMLPaginator::set_renderer(const Ref<HTMLPaginator> &val) {
	renderer = val;
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
	return call("_render_indexed", target, page_index);
}

String HTMLPaginator::_render_indexed(Ref<HTMLPaginator> target, const int page_index) {
	if (!target.is_valid()) {
		target = Ref<HTMLPaginator>(this);
	}

	String s = target->base_url;
	if (s.length() > 0 && s[s.length() - 1] != '/') {
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
		for (int i = starti; i < toi; ++i) {
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
	return call("_render_links", target, page_index);
}

String HTMLPaginator::_render_links(Ref<HTMLPaginator> target, const int page_index) {
	if (!target.is_valid()) {
		target = Ref<HTMLPaginator>(this);
	}

	if (page_index < 0) {
		return "";
	}

	String s = target->base_url;
	if (s.length() > 0 && s[s.length() - 1] != '/') {
		s += '/';
	}

	int max = target->links.size();

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
		for (int i = starti; i < toi; ++i) {
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

String HTMLPaginator::get_pagination_old(const String &base_url, const int max, const int current_index, const int max_visible_links) {
	String s = base_url;
	if (s.length() > 0 && s[s.length() - 1] != '/') {
		s += '/';
	}

	int starti = current_index - max_visible_links / 2;
	int toi = current_index + max_visible_links / 2;

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

	//int toi = max > max_visible_links ? max_visible_links : max;

	HTMLBuilder b;

	b.ul()->cls("pagination");

	if (max != 0 && current_index != 0) {
		b.li();
		b.a()->href(s + itos(current_index - 1));
		b.w("previous");
		b.ca();
		b.cli();
	} else {
		b.li()->cls("disabled");
		b.w("previous");
		b.cli();
	}

	if (starti != toi) {
		for (int i = starti; i < toi; ++i) {
			if (i != current_index) {
				b.li();
				b.a()->href(s + itos(i + 1));
				b.w(itos(i + 1));
				b.ca();
				b.cli();
			} else {
				b.li()->cls("disabled");
				b.w(itos(i + 1));
				b.cli();
			}
		}
	} else {
		b.li()->cls("disabled");
		b.w(itos(1));
		b.cli();
	}

	if (max != 0 && current_index < max - 1) {
		b.li();
		b.a()->href(s + itos(current_index + 2));
		b.w("next");
		b.ca();
		b.cli();
	} else {
		b.li()->cls("disabled");
		b.w("next");
		b.cli();
	}

	b.cul();

	return b.result;
}

String HTMLPaginator::get_pagination_links_old(const String &base_url, const Vector<String> &links, const int current_index, const int max_visible_links) {
	String s = base_url;
	if (s.length() > 0 && s[s.length() - 1] != '/') {
		s += '/';
	}

	int max = links.size();

	int starti = current_index - max_visible_links / 2;
	int toi = current_index + max_visible_links / 2;

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

	//int toi = max > max_visible_links ? max_visible_links : max;

	HTMLBuilder b;

	b.ul()->cls("pagination");

	if (max != 0 && current_index != 0) {
		b.li();
		b.a()->href(s + links[current_index - 1]);
		b.w("previous");
		b.ca();
		b.cli();
	} else {
		b.li()->cls("disabled");
		b.w("previous");
		b.cli();
	}

	if (starti != toi) {
		for (int i = starti; i < toi; ++i) {
			if (i != current_index) {
				b.li();
				b.a()->href(s + links[i]);
				b.w(itos(i + 1));
				b.ca();
				b.cli();
			} else {
				b.li()->cls("disabled");
				b.w(itos(i + 1));
				b.cli();
			}
		}
	} else {
		b.li()->cls("disabled");
		b.w(itos(1));
		b.cli();
	}

	if (max != 0 && current_index < max - 1) {
		b.li();
		b.a()->href(s + links[current_index + 1]);
		b.w("next");
		b.ca();
		b.cli();
	} else {
		b.li()->cls("disabled");
		b.w("next");
		b.cli();
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

void HTMLPaginator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_item_count"), &HTMLPaginator::get_item_count);
	ClassDB::bind_method(D_METHOD("set_item_count", "val"), &HTMLPaginator::set_item_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "item_count"), "set_item_count", "get_item_count");

	ClassDB::bind_method(D_METHOD("get_page_count"), &HTMLPaginator::get_page_count);
	ClassDB::bind_method(D_METHOD("set_page_count", "val"), &HTMLPaginator::set_page_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "page_count"), "set_page_count", "get_page_count");

	ClassDB::bind_method(D_METHOD("get_max_visible_links"), &HTMLPaginator::get_max_visible_links);
	ClassDB::bind_method(D_METHOD("set_max_visible_links", "val"), &HTMLPaginator::set_max_visible_links);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_visible_links"), "set_max_visible_links", "get_max_visible_links");

	ClassDB::bind_method(D_METHOD("get_base_url"), &HTMLPaginator::get_base_url);
	ClassDB::bind_method(D_METHOD("set_base_url", "val"), &HTMLPaginator::set_base_url);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "base_url"), "set_base_url", "get_base_url");

	ClassDB::bind_method(D_METHOD("get_links"), &HTMLPaginator::get_links);
	ClassDB::bind_method(D_METHOD("set_links", "val"), &HTMLPaginator::set_links);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "links"), "set_links", "get_links");

	ClassDB::bind_method(D_METHOD("get_use_links_array"), &HTMLPaginator::get_use_links_array);
	ClassDB::bind_method(D_METHOD("set_use_links_array", "val"), &HTMLPaginator::set_use_links_array);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_links_array"), "set_use_links_array", "get_use_links_array");

	ClassDB::bind_method(D_METHOD("get_hide_if_one_page"), &HTMLPaginator::get_hide_if_one_page);
	ClassDB::bind_method(D_METHOD("set_hide_if_one_page", "val"), &HTMLPaginator::set_hide_if_one_page);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hide_if_one_page"), "set_hide_if_one_page", "get_hide_if_one_page");

	ClassDB::bind_method(D_METHOD("get_class_main_ul"), &HTMLPaginator::get_class_main_ul);
	ClassDB::bind_method(D_METHOD("set_class_main_ul", "val"), &HTMLPaginator::set_class_main_ul);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "class_main_ul"), "set_class_main_ul", "get_class_main_ul");

	ClassDB::bind_method(D_METHOD("get_class_enabled_li"), &HTMLPaginator::get_class_enabled_li);
	ClassDB::bind_method(D_METHOD("set_class_enabled_li", "val"), &HTMLPaginator::set_class_enabled_li);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "class_enabled_li"), "set_class_enabled_li", "get_class_enabled_li");

	ClassDB::bind_method(D_METHOD("get_class_disabled_li"), &HTMLPaginator::get_class_disabled_li);
	ClassDB::bind_method(D_METHOD("set_class_disabled_li", "val"), &HTMLPaginator::set_class_disabled_li);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "class_disabled_li"), "set_class_disabled_li", "get_class_disabled_li");

	ClassDB::bind_method(D_METHOD("get_text_next_link"), &HTMLPaginator::get_text_next_link);
	ClassDB::bind_method(D_METHOD("set_text_next_link", "val"), &HTMLPaginator::set_text_next_link);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "text_next_link"), "set_text_next_link", "get_text_next_link");

	ClassDB::bind_method(D_METHOD("get_text_prev_link"), &HTMLPaginator::get_text_prev_link);
	ClassDB::bind_method(D_METHOD("set_text_prev_link", "val"), &HTMLPaginator::set_text_prev_link);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "text_prev_link"), "set_text_prev_link", "get_text_prev_link");

	ClassDB::bind_method(D_METHOD("get_renderer"), &HTMLPaginator::get_renderer);
	ClassDB::bind_method(D_METHOD("set_renderer", "val"), &HTMLPaginator::set_renderer);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "renderer", PROPERTY_HINT_RESOURCE_TYPE, "HTMLPaginator"), "set_renderer", "get_renderer");

	ClassDB::bind_method(D_METHOD("start"), &HTMLPaginator::start);
	ClassDB::bind_method(D_METHOD("next"), &HTMLPaginator::next);
	ClassDB::bind_method(D_METHOD("get_current"), &HTMLPaginator::get_current);

	ClassDB::bind_method(D_METHOD("get_pagination_for_indx", "page_index"), &HTMLPaginator::get_pagination_for_indx);
	ClassDB::bind_method(D_METHOD("get_pagination_for_num", "page_num"), &HTMLPaginator::get_pagination_for_num);

	BIND_VMETHOD(MethodInfo("_render_indexed", PropertyInfo(Variant::OBJECT, "target", PROPERTY_HINT_RESOURCE_TYPE, "HTMLPaginator"), PropertyInfo(Variant::INT, "page_index")));
	ClassDB::bind_method(D_METHOD("render_indexed", "target", "page_index"), &HTMLPaginator::render_indexed);
	ClassDB::bind_method(D_METHOD("_render_indexed", "target", "page_index"), &HTMLPaginator::_render_indexed);

	BIND_VMETHOD(MethodInfo("_render_links", PropertyInfo(Variant::OBJECT, "target", PROPERTY_HINT_RESOURCE_TYPE, "HTMLPaginator"), PropertyInfo(Variant::INT, "page_index")));
	ClassDB::bind_method(D_METHOD("render_links", "target", "page_index"), &HTMLPaginator::render_links);
	ClassDB::bind_method(D_METHOD("_render_links", "target", "page_index"), &HTMLPaginator::_render_links);
}
