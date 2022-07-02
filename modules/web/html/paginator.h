#ifndef HTML_PAGINATOR_H
#define HTML_PAGINATOR_H

#include "core/ustring.h"
#include "core/vector.h"

#include "core/reference.h"

class HTMLPaginator : public Reference {
	GDCLASS(HTMLPaginator, Reference);

public:
	//settint the item count will update page count and vice versa
	int get_item_count() const;
	void set_item_count(const int val);

	int get_page_count() const;
	void set_page_count(const int val);

	int get_max_visible_links() const;
	void set_max_visible_links(const int val);

	String base_url;
	Vector<String> links;

	bool use_links_array;
	bool hide_if_one_page;

	String class_main_ul;
	String class_enabled_li;
	String class_disabled_li;
	String text_next_link;
	String text_prev_link;

	void start();
	String next();
	String get_current();

	String get_pagination_for_indx(const int page_index);
	String get_pagination_for_num(const int page_num);

	virtual String render_indexed(Ref<HTMLPaginator> target, const int page_index);
	virtual String render_links(Ref<HTMLPaginator> target, const int page_index);

	Ref<HTMLPaginator> renderer;

	//Old methods for the time being
	static String get_pagination_old(const String &base_url, const int max, const int current_index, const int max_visible_links = 10);
	static String get_pagination_links_old(const String &base_url, const Vector<String> &links, const int current_index, const int max_visible_links = 10);

	HTMLPaginator();
	~HTMLPaginator();

protected:
	int _item_count;
	int _page_count;
	int _max_visible_links;
	int _current_page_index;
};

#endif
