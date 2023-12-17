#ifndef BOX_CONTAINER_H
#define BOX_CONTAINER_H

/*************************************************************************/
/*  box_container.h                                                      */
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

#include "scene/gui/container.h"

class BoxContainer : public Container {
	GDCLASS(BoxContainer, Container);

	struct _MinSizeCache {
		int min_size;
		bool will_stretch;
		int final_size;
	};

public:
	enum AlignMode {
		ALIGN_BEGIN,
		ALIGN_CENTER,
		ALIGN_END
	};

	void add_spacer(bool p_begin = false);

	void set_alignment(AlignMode p_align);
	AlignMode get_alignment() const;

	virtual Size2 get_minimum_size() const;

	BoxContainer(bool p_vertical = false);

protected:
	bool _get_vertical() const;
	void _set_vertical(bool p_vertical);

	void _resort();

	void _notification(int p_what);

	static void _bind_methods();

private:
	bool vertical;
	AlignMode align;
};

class HBoxContainer : public BoxContainer {
	GDCLASS(HBoxContainer, BoxContainer);

public:
	HBoxContainer() :
			BoxContainer(false) {}
};

class MarginContainer;
class VBoxContainer : public BoxContainer {
	GDCLASS(VBoxContainer, BoxContainer);

public:
	MarginContainer *add_margin_child(const String &p_label, Control *p_control, bool p_expand = false);

	VBoxContainer() :
			BoxContainer(true) {}
};

class CBoxContainer : public BoxContainer {
	GDCLASS(CBoxContainer, BoxContainer);

public:
	enum ContainerMode {
		CONTAINER_MODE_HORIZONTAL = 0,
		CONTAINER_MODE_VERTICAL,
	};

	ContainerMode get_mode() const;
	void set_mode(const ContainerMode p_mode);

	CBoxContainer();
	~CBoxContainer();

protected:
	static void _bind_methods();

	ContainerMode _mode;
};

VARIANT_ENUM_CAST(BoxContainer::AlignMode);
VARIANT_ENUM_CAST(CBoxContainer::ContainerMode);

#endif // BOX_CONTAINER_H
