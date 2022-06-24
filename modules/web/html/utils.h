#ifndef UTILS_H
#define UTILS_H

#include "core/string.h"
#include "core/containers/vector.h"

class Utils {
public:
	static void markdown_to_html(String *str);

	static String get_pagination(const String &base_url, const uint32_t max, const uint32_t current_index, const uint32_t max_visible_links = 10);
	static String get_pagination_links(const String &base_url, const Vector<String> &links, const uint32_t current_index, const uint32_t max_visible_links = 10);

protected:
};

#endif