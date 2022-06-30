#ifndef BROWSABLE_FOLDER_SERVE_NODE_H
#define BROWSABLE_FOLDER_SERVE_NODE_H

#include "core/string.h"
#include <map>

#include "folder_serve_node.h"

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
	RCPP_OBJECT(BrowsableFolderServeNode, FolderServeNode);

public:
	void _handle_request_main(Request *request);

	void render_index(Request *request);
	void render_preview(Request *request);

	virtual void load();

	void evaluate_dir(const String &path, const bool top_level = false);
	virtual void render_dir_page(const String &dir_uri, const Vector<String> &folders, const Vector<String> &files, const bool top_level);

	bool should_render_menu;

	BrowsableFolderServeNode();
	~BrowsableFolderServeNode();

protected:
	std::map<String, String *> _folder_indexes;
};

#endif