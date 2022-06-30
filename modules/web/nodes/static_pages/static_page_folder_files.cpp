#include "static_page_folder_files.h"

#include "core/os/directory.h"
#include "web/file_cache.h"
#include "web/html/html_builder.h"
#include "web/html/utils.h"
#include "web/http/request.h"

void StaticPageFolderFiles::load() {
	if (dir_path == "") {
		return;
	}

	Ref<Directory> d;
	d.instance();

	ERR_FAIL_COND_MSG(!d->open_dir(dir_path), "Dir Path = " + dir_path);

	String str;
	while (d->has_next()) {
		if (d->current_is_file()) {
			String fn = dir_path;
			fn.append_path(d->current_get_name_cstr());

			d->read_file_into(fn, &str);

			if (process_if_can && d->current_get_extension() == "md") {
				Utils::markdown_to_html(&str);
			}

			append_data(str);
		}
	}
}

void StaticPageFolderFiles::append_data(const String &d) {
	data += d;
}

void StaticPageFolderFiles::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

StaticPageFolderFiles::StaticPageFolderFiles() :
		StaticPage() {

	process_if_can = true;
}

StaticPageFolderFiles::~StaticPageFolderFiles() {
}
