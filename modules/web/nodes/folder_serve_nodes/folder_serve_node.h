#ifndef FOLDER_SERVE_NODE_H
#define FOLDER_SERVE_NODE_H

#include "core/reference.h"
#include "core/ustring.h"

#include "../../http/web_node.h"

class WebServerRequest;
class FileCache;

// This class will serve the files from the folder set to it's serve_folder property.
// It will cache the folder's contents on ENTER_TREE, and will match against the cached list,
// this means directory walking (for example sending http://webapp.com/files/../../../etc/passwd),
// and other techniques like it should not be possible.

class FolderServeNode : public WebNode {
	GDCLASS(FolderServeNode, WebNode);

public:
	String get_serve_folder();
	void set_serve_folder(const String &val);

	void _handle_request_main(Ref<WebServerRequest> request);

	virtual void load();

	FolderServeNode();
	~FolderServeNode();

protected:
	void _notification(const int what);
	static void _bind_methods();

	String _serve_folder;

	Ref<FileCache> _file_cache;
};

#endif
