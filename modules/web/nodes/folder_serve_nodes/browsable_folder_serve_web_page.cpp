#include "browsable_folder_serve_web_page.h"

#include "core/os/dir_access.h"

#include "../../html/html_builder.h"

#include "../../file_cache.h"
#include "../../http/web_permission.h"
#include "../../http/web_server_request.h"

#include "../../http/http_server_enums.h"

bool BrowsableFolderServeWebPage::get_should_render_menu() {
	return _should_render_menu;
}
void BrowsableFolderServeWebPage::set_should_render_menu(const bool &val) {
	_should_render_menu = val;
}

void BrowsableFolderServeWebPage::_handle_request(Ref<WebServerRequest> request) {
	String file_name = request->get_path(true, false);

	for (int i = 0; i < _folder_indexes.size(); ++i) {
		const BFSNEntry &e = _folder_indexes[i];

		if (e.uri == file_name) {
			if (_should_render_menu) {
				render_menu(request);
			}

			request->body += e.data;
			request->compile_and_send_body();
			return;
		}
	}

	request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
}

void BrowsableFolderServeWebPage::_render_index(Ref<WebServerRequest> request) {
	request->body += _index;
}
void BrowsableFolderServeWebPage::_render_preview(Ref<WebServerRequest> request) {
}

void BrowsableFolderServeWebPage::load() {
	FolderServeWebPage::load();

	if (_serve_folder == "") {
		return;
	}

	evaluate_dir(_file_cache->get_wwwroot_abs(), true);
}

void BrowsableFolderServeWebPage::evaluate_dir(const String &path, const bool top_level) {
	DirAccess *dir = DirAccess::open(path);

	ERR_FAIL_COND_MSG(!dir, "Error opening folde!r: " + String(path));

	String serve_folder = _file_cache->get_wwwroot_abs();

	String dir_uri;

	if (!top_level) {
		dir_uri = path.substr(serve_folder.length(), path.length() - serve_folder.length());
	} else {
		dir_uri = "/";
	}

	Vector<String> folders;
	Vector<String> files;

	dir->list_dir_begin();

	String file = dir->get_next();

	while (file != "") {
		String np = path.append_path(file);
		String nnp = np.substr(serve_folder.length(), np.length() - serve_folder.length());

		if (dir->current_is_dir()) {
			if (file == "." || file == "..") {
				file = dir->get_next();
				continue;
			}

			folders.push_back(nnp);
			evaluate_dir(np);
		} else {
			files.push_back(nnp);
		}

		file = dir->get_next();
	}

	dir->list_dir_end();

	memdelete(dir);

	folders.sort();
	files.sort();

	//folders.sort_inc();
	//files.sort_inc();

	render_dir_page(dir_uri, folders, files, top_level);
}

void BrowsableFolderServeWebPage::render_dir_page(const String &dir_uri, const Vector<String> &folders, const Vector<String> &files, const bool top_level) {
	HTMLBuilder b;

	String uri = get_full_uri(false);

	b.div("file_list");
	{
		if (!top_level) {
			b.div("file_list_entry");
			{
				b.a(uri + dir_uri.path_get_prev_dir())->w("..")->ca();
			}
			b.cdiv();
		}

		for (int i = 0; i < folders.size(); ++i) {
			b.div("file_list_entry");
			{
				b.a(uri + folders[i])->w("(Folder) ")->w(folders[i].get_file())->ca();
			}
			b.cdiv();
		}

		for (int i = 0; i < files.size(); ++i) {
			b.div("file_list_entry");
			{
				b.a(uri + files[i])->w("(File) ")->w(files[i].get_file())->ca();
			}
			b.cdiv();
		}
	}
	b.cdiv();

	BFSNEntry e;
	e.uri = dir_uri;
	e.data = b.result;

	_folder_indexes.push_back(e);

	if (dir_uri == "/") {
		_index = b.result;
	}
}

BrowsableFolderServeWebPage::BrowsableFolderServeWebPage() {
	_should_render_menu = true;
}

BrowsableFolderServeWebPage::~BrowsableFolderServeWebPage() {
	_folder_indexes.clear();
}
void BrowsableFolderServeWebPage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_should_render_menu"), &BrowsableFolderServeWebPage::get_should_render_menu);
	ClassDB::bind_method(D_METHOD("set_should_render_menu", "val"), &BrowsableFolderServeWebPage::set_should_render_menu);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "should_render_menu"), "set_should_render_menu", "get_should_render_menu");
}
