#ifndef POLYGON_DIALOG_H
#define POLYGON_DIALOG_H

/*************************************************************************/
/*  polygon_dialog.h                                                     */
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

#include "scene/gui/dialogs.h"

class Button;
class PolygonEditor;
class PolygonBase;

class PolygonDialog : public WindowDialog {
	GDCLASS(PolygonDialog, WindowDialog);

public:
	bool get_closed() const;
	void set_closed(const bool c = true);

	PoolVector2Array get_previous_points();
	void set_previous_points(const PoolVector2Array &val);

	Ref<PolygonBase> get_polygon();
	void set_polygon(const Ref<PolygonBase> &val);
	void edit_polygon(const Ref<PolygonBase> &poly);

	void _on_CurveDialog_popup_hide();
	void _on_OK_pressed();
	void _on_Cancel_pressed();
	void _on_PolygonEditor_value_changed(const Variant &value);

	PolygonDialog();
	~PolygonDialog();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	bool closed;
	PoolVector2Array previous_points;
	Ref<PolygonBase> polygon;

	PolygonEditor *_polygon_editor;
	Button *_ok_button;
	Button *_cancel_button;
};

#endif
