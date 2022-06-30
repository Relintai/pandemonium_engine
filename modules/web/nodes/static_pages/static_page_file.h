#ifndef STATIC_PAGE_FILE_H
#define STATIC_PAGE_FILE_H

#include "core/string.h"

#include "static_page.h"

class StaticPageFile : public StaticPage {
	RCPP_OBJECT(StaticPageFile, StaticPage);

public:
	void load();

	void _notification(const int what);

	String file_path;
	bool process_if_can;

	StaticPageFile();
	~StaticPageFile();
};

#endif