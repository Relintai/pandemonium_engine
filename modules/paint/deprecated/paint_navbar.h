#ifndef PAINT_NAVBAR_H
#define PAINT_NAVBAR_H

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

#include "scene/gui/box_container.h"

class PaintWindow;
class PaintCanvasOld;
class MenuButton;

class PaintNavbar : public BoxContainer {
	GDCLASS(PaintNavbar, BoxContainer);

public:
	enum {
		MENU_FILE_NEW = 0,
		MENU_FILE_SAVE,
		MENU_FILE_LOAD,

		MENU_EDIT_UNDO,
		MENU_EDIT_REDO,
		MENU_EDIT_CUT,
		MENU_EDIT_COPY,
		MENU_EDIT_PASTE,

		MENU_CANVAS_CHANGE_SIZE,
		MENU_CANVAS_CROP_TO_CONTENT,

		MENU_LAYER_ADD,
		MENU_LAYER_DELETE,
		MENU_LAYER_DUPLICATE,
		MENU_LAYER_CLEAR,
		MENU_LAYER_TOGGLE_ALPHA_LOCKED,

		MENU_GRID_TOGGLE,
		MENU_GRID_CHANGE_SIZE,

		MENU_MAGIC_CHANGE_SINGLE_COLOR,
		MENU_MAGIC_CHANGE_COLOR_RANGE,
		MENU_MAGIC_HSV_NOISE,
		MENU_MAGIC_HSV_COLOR_MODULATION,

		MENU_EDITOR_SETTINGS,
	};

	void handle_menu_item_pressed(const int id);

	bool is_any_menu_open();

	PaintNavbar();
	~PaintNavbar();

	PaintWindow *paint_window;
	PaintCanvasOld *canvas;

	MenuButton *file_menu_button;
	MenuButton *edit_menu_button;
	MenuButton *canvas_menu_button;
	MenuButton *layer_menu_button;
	MenuButton *grid_menu_button;
	MenuButton *magic_menu_button;
	MenuButton *editor_menu_button;

protected:
	static void _bind_methods();
};

#endif
