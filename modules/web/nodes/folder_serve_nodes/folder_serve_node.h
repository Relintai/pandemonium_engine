#ifndef FOLDER_SERVE_NODE_H
#define FOLDER_SERVE_NODE_H

#include "core/string.h"

#include "web/file_cache.h"
#include "web/http/web_node.h"

// This class will serve the files from the folder set to it's serve_folder property.
// It will cache the folder's contents on ENTER_TREE, and will match against the cached list,
// this means directory walking (for example sending http://webapp.com/files/../../../etc/passwd), 
// and other techniques like it should not be possible.

class FolderServeNode : public WebNode {
	RCPP_OBJECT(FolderServeNode, WebNode);

public:
	void handle_request_main(Request *request);

	virtual void load();

	void _notification(const int what);

	String serve_folder;

	FolderServeNode();
	~FolderServeNode();

protected:
	FileCache *file_cache;
};

#endif