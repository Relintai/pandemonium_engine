#ifndef BROWSABLE_FOLDER_SERVE_NODE_H
#define BROWSABLE_FOLDER_SERVE_NODE_H

#include "core/ustring.h"
#include "core/vector.h"

#include "folder_serve_node.h"

class WebServerRequest;

// On top of serving the files from the folder set to it's serve_folder property,
// this class also generates HTML directory lists. (Similar to apache's directory listing)
// It caches folder contents on ENTER_TREE, it does not watch for folder changes yet.

// if (should_render_menu) -> render_menu(request)
// <div class="file_list">
// <div class="file_list_entry"><a href="/files/">..</a></div>
// <div class="file_list_entry"><a href="/files/test_folder/">(Folder) test_folder</a></div>
// <div class="file_list_entry"><a href="/files/test_file.md">(File) test_file.md</a></div>
// ...
// </div>

class BrowsableFolderServeNode : public FolderServeNode {
	GDCLASS(BrowsableFolderServeNode, FolderServeNode);

public:
	bool get_should_render_menu();
	void set_should_render_menu(const bool &val);

	void _handle_request(Ref<WebServerRequest> request);

	void _render_index(Ref<WebServerRequest> request);
	void _render_preview(Ref<WebServerRequest> request);

	void load();

	void evaluate_dir(const String &path, const bool top_level = false);
	virtual void render_dir_page(const String &dir_uri, const Vector<String> &folders, const Vector<String> &files, const bool top_level);

	BrowsableFolderServeNode();
	~BrowsableFolderServeNode();

protected:
	static void _bind_methods();

	bool _should_render_menu;

	struct BFSNEntry {
		String uri;
		String data;
	};

	Vector<BFSNEntry> _folder_indexes;
	String _index;
};

#endif
