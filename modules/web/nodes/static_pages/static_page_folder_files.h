#ifndef STATIC_PAGE_FOLDER_FILES_H
#define STATIC_PAGE_FOLDER_FILES_H

#include "core/ustring.h"

#include "static_page.h"

class StaticPageFolderFiles : public StaticPage {
	GDCLASS(StaticPageFolderFiles, StaticPage);

public:
	String get_dir_path();
	void set_dir_path(const String &val);

	bool get_process_if_can();
	void set_process_if_can(const bool &val);

	void load();
	virtual void append_data(const String &d);

	StaticPageFolderFiles();
	~StaticPageFolderFiles();

protected:
	void _notification(const int what);
	static void _bind_methods();

	String _dir_path;
	bool _process_if_can;
};

#endif
