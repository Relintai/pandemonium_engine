#include "browsable_folder_serve_node.h"

#include "core/os/dir_access.h"

#include "../../html/html_builder.h"

#include "../../file_cache.h"
#include "../../http/web_permission.h"
#include "../../http/web_server_request.h"

#include "../../http/http_server_enums.h"

bool BrowsableFolderServeNode::get_should_render_menu() {
	return _should_render_menu;
}
void BrowsableFolderServeNode::set_should_render_menu(const bool &val) {
	_should_render_menu = val;
}

void BrowsableFolderServeNode::_handle_request(Ref<WebServerRequest> request) {
	String file_name = request->get_path(true, false);

	String *s = _folder_indexes[file_name];

	if (!s) {
		request->send_error(HTTPServerEnums::HTTP_STATUS_CODE_404_NOT_FOUND);
		return;
	}

	if (_should_render_menu) {
		render_main_menu(request);
	}

	request->body += (*s);
	request->compile_and_send_body();
}

void BrowsableFolderServeNode::render_index(Ref<WebServerRequest> request) {
	String *s = _folder_indexes["/"];

	if (!s) {
		return;
	}

	request->body += (*s);
}
void BrowsableFolderServeNode::render_preview(Ref<WebServerRequest> request) {
}

void BrowsableFolderServeNode::load() {
	if (_serve_folder == "") {
		return;
	}

	FolderServeNode::load();

	evaluate_dir(_serve_folder, true);
}

void BrowsableFolderServeNode::evaluate_dir(const String &path, const bool top_level) {
	DirAccess *dir = DirAccess::open(path);

	ERR_FAIL_COND_MSG(!dir, "Error opening folde!r: " + String(path));

	String dir_uri;

	if (!top_level) {
		dir_uri = path.substr(_serve_folder.size(), path.size() - _serve_folder.size());
	} else {
		dir_uri = "/";
	}

	Vector<String> folders;
	Vector<String> files;

	dir->list_dir_begin();

	String file = dir->get_next();

	while (file != "") {
		String np = path.append_path(file);
		String nnp = np.substr(_serve_folder.size(), np.size() - _serve_folder.size());

		if (dir->current_is_dir() && file != "." && file != "..") {
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

void BrowsableFolderServeNode::render_dir_page(const String &dir_uri, const Vector<String> &folders, const Vector<String> &files, const bool top_level) {
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
				b.a(uri + folders[i])->w("(Folder) ")->w(folders[i].get_basename())->ca();
			}
			b.cdiv();
		}

		for (int i = 0; i < files.size(); ++i) {
			b.div("file_list_entry");
			{
				b.a(uri + files[i])->w("(File) ")->w(files[i].get_basename())->ca();
			}
			b.cdiv();
		}
	}
	b.cdiv();

	String *s = memnew(String);
	s->operator+=(b.result);

	_folder_indexes[dir_uri] = s;
}

BrowsableFolderServeNode::BrowsableFolderServeNode() {
	_should_render_menu = true;
}

BrowsableFolderServeNode::~BrowsableFolderServeNode() {
	const String *key = nullptr;
	while ((key = _folder_indexes.next(key))) {
		String k = *key;

		String *v = _folder_indexes[k];

		if (v) {
			memdelete(v);
		}
	}

	_folder_indexes.clear();
}
