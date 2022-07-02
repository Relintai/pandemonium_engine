#ifndef HTML_UTILS_H
#define HTML_UTILS_H

#include "core/ustring.h"
#include "core/vector.h"

class HTMLUtils {
public:
	static String get_pagination(const String &base_url, const uint32_t max, const uint32_t current_index, const uint32_t max_visible_links = 10);
	static String get_pagination_links(const String &base_url, const Vector<String> &links, const uint32_t current_index, const uint32_t max_visible_links = 10);

protected:
};

#endif
