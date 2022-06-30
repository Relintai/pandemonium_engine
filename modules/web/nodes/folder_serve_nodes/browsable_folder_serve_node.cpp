#include "browsable_folder_serve_node.h"

#include "core/os/directory.h"
#include "web/file_cache.h"
#include "web/html/html_builder.h"
#include "web/http/request.h"

void BrowsableFolderServeNode::_handle_request_main(Request *request) {
	String file_name = request->get_path(true, false);

	String *s = _folder_indexes[file_name];

	if (!s) {
		request->send_error(HTTP_STATUS_CODE_404_NOT_FOUND);
		return;
	}

	if (should_render_menu) {
		render_menu(request);
	}

	request->body += (*s);
	request->compile_and_send_body();
}

void BrowsableFolderServeNode::render_index(Request *request) {
	String *s = _folder_indexes["/"];

	if (!s) {
		return;
	}

	request->body += (*s);
}
void BrowsableFolderServeNode::render_preview(Request *request) {
}

void BrowsableFolderServeNode::load() {
	if (serve_folder == "") {
		return;
	}

	FolderServeNode::load();

	evaluate_dir(serve_folder, true);
}

void BrowsableFolderServeNode::evaluate_dir(const String &path, const bool top_level) {
	Ref<Directory> dir;
	dir.instance();

	ERR_FAIL_COND_MSG(dir->open_dir(path) != OK, "Error opening folde!r: " + String(path));

	String dir_uri;

	if (!top_level) {
		dir_uri = path.substr(serve_folder.size(), path.size() - serve_folder.size());
	} else {
		dir_uri = "/";
	}

	Vector<String> folders;
	Vector<String> files;

	while (dir->next()) {
		String np = dir->current_get_path();
		np = np.substr(serve_folder.size(), np.size() - serve_folder.size());

		if (dir->current_is_file()) {
			files.push_back(np);

		} else {
			folders.push_back(np);
			evaluate_dir(dir->current_get_path());
		}
	}

	dir->close_dir();

	folders.sort_inc();
	files.sort_inc();

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
				b.a(uri + folders[i])->w("(Folder) ")->w(folders[i].path_get_basename())->ca();
			}
			b.cdiv();
		}

		for (int i = 0; i < files.size(); ++i) {
			b.div("file_list_entry");
			{
				b.a(uri + files[i])->w("(File) ")->w(files[i].path_get_basename())->ca();
			}
			b.cdiv();
		}
	}
	b.cdiv();

	String *s = new String();
	s->append_str(b.result);

	_folder_indexes[dir_uri] = s;
}

BrowsableFolderServeNode::BrowsableFolderServeNode() :
		FolderServeNode() {

	should_render_menu = true;
}

BrowsableFolderServeNode::~BrowsableFolderServeNode() {
	for (std::map<String, String *>::iterator E = _folder_indexes.begin(); E != _folder_indexes.end(); E++) {
		if (E->second) {
			delete E->second;
		}
	}

	_folder_indexes.clear();
}
