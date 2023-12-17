#ifndef FOLDER_SERVE_WEB_PAGE_H
#define FOLDER_SERVE_WEB_PAGE_H

/*************************************************************************/
/*  folder_serve_web_page.h                                              */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "../../http/web_node.h"

class WebServerRequest;
class FileCache;

// This class will serve the files from the folder set to it's serve_folder property.
// It will cache the folder's contents on ENTER_TREE, and will match against the cached list,
// this means directory walking (for example sending http://webapp.com/files/../../../etc/passwd),
// and other techniques like it should not be possible.

class FolderServeWebPage : public WebNode {
	GDCLASS(FolderServeWebPage, WebNode);

public:
	String get_serve_folder();
	void set_serve_folder(const String &val);

	void _handle_request_main(Ref<WebServerRequest> request);

	virtual void load();

	FolderServeWebPage();
	~FolderServeWebPage();

protected:
	void _notification(const int what);
	static void _bind_methods();

	String _serve_folder;

	Ref<FileCache> _file_cache;
};

#endif
