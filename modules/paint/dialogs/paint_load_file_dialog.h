#ifndef PAINT_LOAD_FILE_DIALOG_H
#define PAINT_LOAD_FILE_DIALOG_H

/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "scene/gui/file_dialog.h"

class PaintCanvas;
class PaintWindow;

class PaintLoadFileDialog : public FileDialog {
	GDCLASS(PaintLoadFileDialog, FileDialog);

public:
	void load_img();

	void _on_LoadFileDialog_file_selected(const String &path);
	void _on_LoadFileDialog_confirmed();
	void _on_LoadFileDialog_about_to_show();
	void _on_LoadFileDialog_visibility_changed();

	PaintLoadFileDialog();
	~PaintLoadFileDialog();

	String file_path;

	PaintCanvas *canvas;
	PaintWindow *window;

protected:
	void _notification(int p_what);
	static void _bind_methods();
};

#endif
