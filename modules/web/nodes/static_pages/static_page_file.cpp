#include "static_page_file.h"

#include "web/file_cache.h"
#include "web/html/html_builder.h"
#include "web/http/request.h"

void StaticPageFile::load() {
	if (file_path == "") {
		return;
	}

	if (process_if_can) {
		load_and_process_file(file_path);
	} else {
		load_file(file_path);
	}
}

void StaticPageFile::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE:
			load();
			break;
		default:
			break;
	}
}

StaticPageFile::StaticPageFile() :
		StaticPage() {

	process_if_can = true;
}

StaticPageFile::~StaticPageFile() {
}
