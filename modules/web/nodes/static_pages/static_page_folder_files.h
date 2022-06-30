#ifndef STATIC_PAGE_FOLDER_FILES_H
#define STATIC_PAGE_FOLDER_FILES_H

#include "core/string.h"

#include "static_page.h"

class StaticPageFolderFiles : public StaticPage {
	RCPP_OBJECT(StaticPageFolderFiles, StaticPage);

public:
	void load();
	virtual void append_data(const String &d);

	void _notification(const int what);

	String dir_path;
	bool process_if_can;

	StaticPageFolderFiles();
	~StaticPageFolderFiles();
};

#endif