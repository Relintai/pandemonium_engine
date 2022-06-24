#include "utils.h"

#include "html_builder.h"
#include <maddy/parser.h>
#include <memory>
#include <sstream>
#include <string>

void Utils::markdown_to_html(String *str) {
	std::shared_ptr<maddy::ParserConfig> config = std::make_shared<maddy::ParserConfig>();
	config->isEmphasizedParserEnabled = false;
	config->isHTMLWrappedInParagraph = true;

	std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>(config);

	std::stringstream ss((*str));

	String htmlOutput = parser->Parse(ss);

	(*str) = htmlOutput;
}

String Utils::get_pagination(const String &base_url, const uint32_t max, const uint32_t current_index, const uint32_t max_visible_links) {
	String s = base_url;
	if (s.size() > 0 && s[s.size() - 1] != '/') {
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
		b.a()->href(s + std::to_string(current_index - 1));
		b.w("previous");
		b.ca();
		b.cli();
	} else {
		b.li()->cls("disabled");
		b.w("previous");
		b.cli();
	}

	if (starti != toi) {
		for (uint32_t i = starti; i < toi; ++i) {
			if (i != current_index) {
				b.li();
				b.a()->href(s + std::to_string(i + 1));
				b.w(std::to_string(i + 1));
				b.ca();
				b.cli();
			} else {
				b.li()->cls("disabled");
				b.w(std::to_string(i + 1));
				b.cli();
			}
		}
	} else {
		b.li()->cls("disabled");
		b.w(std::to_string(1));
		b.cli();
	}

	if (max != 0 && current_index < max - 1) {
		b.li();
		b.a()->href(s + std::to_string(current_index + 2));
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

String Utils::get_pagination_links(const String &base_url, const Vector<String> &links, const uint32_t current_index, const uint32_t max_visible_links) {
	String s = base_url;
	if (s.size() > 0 && s[s.size() - 1] != '/') {
		s += '/';
	}

	uint32_t max = links.size();

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
		for (uint32_t i = starti; i < toi; ++i) {
			if (i != current_index) {
				b.li();
				b.a()->href(s + links[i]);
				b.w(std::to_string(i + 1));
				b.ca();
				b.cli();
			} else {
				b.li()->cls("disabled");
				b.w(std::to_string(i + 1));
				b.cli();
			}
		}
	} else {
		b.li()->cls("disabled");
		b.w(std::to_string(1));
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

