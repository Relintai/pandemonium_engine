/*************************************************************************/
/*  paint_node.cpp                                                       */
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

#include "paint_node.h"

#include "../paint_utilities.h"
#include "core/math/geometry.h"
#include "paint_project.h"

Vector2i PaintNode::get_size() {
	return _size;
}
void PaintNode::set_size(const Vector2i &size) {
	if (size == _size) {
		return;
	}

	_size = size;

	emit_signal("size_changed");
	_propagate_notification_resized();
	update();
}

bool PaintNode::get_draw_outline() {
	return _draw_outline;
}
void PaintNode::set_draw_outline(const bool val) {
	_draw_outline = val;

	update();
}

Ref<Image> PaintNode::render_image() {
	_propagate_notification_project_pre_render();
	Ref<Image> img = call("_render_image");
	_propagate_notification_project_post_render();

	return img;
}
Ref<Image> PaintNode::_render_image() {
	Ref<Image> image;
	image.instance();

	Vector2i size = get_size();

	if (size.x <= 0 || size.y <= 0) {
		return image;
	}

	image->create(size.x, size.y, false, Image::FORMAT_RGBA8);

	for (int i = 0; i < get_child_count(); ++i) {
		PaintNode *pn = Object::cast_to<PaintNode>(get_child(i));

		if (pn && pn->is_visible()) {
			//dont apply own transform
			render_evaluate_paint_node(pn, Transform2D(), image);
		}
	}

	render_paint_node(this, Transform2D(), image);

	return image;
}

Ref<Image> PaintNode::get_rendered_image() {
	return call("_get_rendered_image");
}
Ref<Image> PaintNode::_get_rendered_image() {
	return Ref<Image>();
}

bool PaintNode::render_should_evaluate_children() {
	return call("_render_should_evaluate_children");
}
bool PaintNode::_render_should_evaluate_children() {
	return true;
}

void PaintNode::render_evaluate_paint_node(PaintNode *node, Transform2D transform, Ref<Image> image) {
	ERR_FAIL_COND(!node);
	ERR_FAIL_COND(!image.is_valid());

	Transform2D currtf = transform * node->get_transform();

	if (node->render_should_evaluate_children()) {
		for (int i = 0; i < node->get_child_count(); ++i) {
			PaintNode *pn = Object::cast_to<PaintNode>(node->get_child(i));

			if (pn && pn->is_visible()) {
				render_evaluate_paint_node(pn, currtf, image);
			}
		}
	}

	render_paint_node(node, currtf, image);
}

void PaintNode::render_paint_node(PaintNode *node, Transform2D transform, Ref<Image> image) {
	ERR_FAIL_COND(!node);
	ERR_FAIL_COND(!image.is_valid());

	Ref<Image> save_image = node->get_rendered_image();

	if (!save_image.is_valid() || save_image->empty()) {
		return;
	}

	Vector2i save_image_size = save_image->get_size();
	Vector2i image_size = image->get_size();

	Vector2 p0 = transform.xform(Vector2(0, 0));
	Vector2 p1 = transform.xform(Vector2(0, save_image_size.y));
	Vector2 p2 = transform.xform(Vector2(save_image_size.x, 0));
	Vector2 p3 = transform.xform(Vector2(save_image_size.x, save_image_size.y));

	Rect2i bound(p0, Vector2());
	bound.expand_to(p1);
	bound.expand_to(p2);
	bound.expand_to(p3);
	bound = bound.grow(1);

	Transform2D inverse = transform.affine_inverse();

	save_image->lock();
	image->lock();

	for (int x = bound.position.x; x < bound.position.x + bound.size.x; ++x) {
		for (int y = bound.position.y; y < bound.position.y + bound.size.y; ++y) {
			Point2i p = Point2i(x, y);

			if (p.x < 0 || p.y < 0 || p.x >= image_size.x || p.y >= image_size.y) {
				continue;
			}

			Vector2 npos = inverse.xform(Vector2(p.x, p.y));

			if (npos.x < 0 || npos.y < 0 || npos.x >= save_image_size.x || npos.y >= save_image_size.y) {
				continue;
			}

			Color sic = save_image->get_pixel(npos.x, npos.y);
			Color oic = image->get_pixel(p.x, p.y);

			image->set_pixel(p.x, p.y, oic.blend(sic));
		}
	}

	image->unlock();
	save_image->unlock();
}

PoolVector2iArray PaintNode::util_get_pixels_in_line(const Vector2i &from, const Vector2i &to) {
	return PaintUtilities::get_pixels_in_line(from, to);
}

int PaintNode::util_to_1d_v(const Vector2i &p, int w) {
	return PaintUtilities::to_1D_v(p, w);
}
int PaintNode::util_to_1d(int x, int y, int w) {
	return PaintUtilities::to_1D(x, y, w);
}
Vector2i PaintNode::util_to_2d(int idx, int w) {
	return PaintUtilities::to_2D(idx, w);
}

Color PaintNode::util_color_from_array(const PoolRealArray &color_array) {
	return PaintUtilities::color_from_array(color_array);
}
Color PaintNode::util_random_color() {
	return PaintUtilities::random_color();
}
Color PaintNode::util_random_color_alt() {
	return PaintUtilities::random_color_alt();
}

bool PaintNode::forward_canvas_gui_input(const Ref<InputEvent> &p_event) {
	return call("_forward_canvas_gui_input", p_event);
}
bool PaintNode::_forward_canvas_gui_input(const Ref<InputEvent> &p_event) {
	return false;
}

PaintProject *PaintNode::get_paint_project() {
	PaintNode *p = this;

	while (p) {
		PaintProject *pp = Object::cast_to<PaintProject>(p);

		if (pp) {
			return pp;
		}

		p = Object::cast_to<PaintNode>(p->get_parent());
	}

	return NULL;
}

PaintNode *PaintNode::get_parent_paint_node() {
	return Object::cast_to<PaintNode>(get_parent());
}

PaintNode *PaintNode::find_parent_paint_node() {
	Node *p = get_parent();

	while (p) {
		PaintNode *pn = Object::cast_to<PaintNode>(p);

		if (pn) {
			return pn;
		}

		p = Object::cast_to<PaintNode>(p->get_parent());
	}

	return NULL;
}

String PaintNode::get_configuration_warning() const {
	const PaintNode *p = this;

	while (p) {
		const PaintProject *pp = Object::cast_to<PaintProject>(p);

		if (pp) {
			return "";
		}

		p = Object::cast_to<PaintNode>(p->get_parent());
	}

	return "This Node should be a child of a PaintProject!";
}

void PaintNode::_propagate_notification_resized() {
	//Only send it to children
	//So if they don't resize themselves, the notification stops

	for (int i = 0; i < get_child_count(); ++i) {
		PaintNode *pn = Object::cast_to<PaintNode>(get_child(i));

		if (pn) {
			pn->notification(NOTIFICATION_PARENT_PAINT_NODE_RESIZED);
		}
	}
}

void PaintNode::_propagate_notification_project_pre_render() {
	notification(NOTIFICATION_PAINT_PROJECT_PRE_RENDER);

	for (int i = 0; i < get_child_count(); ++i) {
		PaintNode *pn = Object::cast_to<PaintNode>(get_child(i));

		if (pn) {
			pn->_propagate_notification_project_pre_render();
		}
	}
}
void PaintNode::_propagate_notification_project_post_render() {
	notification(NOTIFICATION_PAINT_PROJECT_POST_RENDER);

	for (int i = 0; i < get_child_count(); ++i) {
		PaintNode *pn = Object::cast_to<PaintNode>(get_child(i));

		if (pn) {
			pn->_propagate_notification_project_post_render();
		}
	}
}

PaintNode::PaintNode() {
	_draw_outline = true;
}

PaintNode::~PaintNode() {
}

void PaintNode::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POST_ENTER_TREE: {
			if (get_size() == Vector2i(0, 0)) {
				PaintNode *pn = get_parent_paint_node();

				if (pn) {
					set_size(pn->get_size());
				}
			}
		} break;
		case NOTIFICATION_DRAW: {
			if (_draw_outline) {
				draw_line(Point2(0, 0), Point2(_size.x, 0), Color(0, 0, 0, 1));
				draw_line(Point2(0, _size.y), Point2(_size.x, _size.y), Color(0, 0, 0, 1));
				draw_line(Point2(0, 0), Point2(0, _size.y), Color(0, 0, 0, 1));
				draw_line(Point2(_size.x, 0), Point2(_size.x, _size.y), Color(0, 0, 0, 1));
			}
		} break;
		default:
			break;
	}
}

void PaintNode::_bind_methods() {
	ADD_SIGNAL(MethodInfo("size_changed"));

	ClassDB::bind_method(D_METHOD("get_size"), &PaintNode::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &PaintNode::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("get_draw_outline"), &PaintNode::get_draw_outline);
	ClassDB::bind_method(D_METHOD("set_draw_outline", "val"), &PaintNode::set_draw_outline);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_outline"), "set_draw_outline", "get_draw_outline");

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "r", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "_render_image"));
	ClassDB::bind_method(D_METHOD("render_image"), &PaintNode::render_image);
	ClassDB::bind_method(D_METHOD("_render_image"), &PaintNode::_render_image);

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "r", PROPERTY_HINT_RESOURCE_TYPE, "Image"), "_get_rendered_image"));
	ClassDB::bind_method(D_METHOD("get_rendered_image"), &PaintNode::get_rendered_image);
	ClassDB::bind_method(D_METHOD("_get_rendered_image"), &PaintNode::_get_rendered_image);

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "r"), "_render_should_evaluate_children"));
	ClassDB::bind_method(D_METHOD("render_should_evaluate_children"), &PaintNode::render_should_evaluate_children);
	ClassDB::bind_method(D_METHOD("_render_should_evaluate_children"), &PaintNode::_render_should_evaluate_children);

	ClassDB::bind_method(D_METHOD("util_get_pixels_in_line", "from", "to"), &PaintNode::util_get_pixels_in_line);

	ClassDB::bind_method(D_METHOD("util_to_1d_v", "p", "w"), &PaintNode::util_to_1d_v);
	ClassDB::bind_method(D_METHOD("util_to_1d", "x", "y", "w"), &PaintNode::util_to_1d);
	ClassDB::bind_method(D_METHOD("util_to_2d", "idx", "w"), &PaintNode::util_to_2d);

	ClassDB::bind_method(D_METHOD("util_color_from_array", "color_array"), &PaintNode::util_color_from_array);
	ClassDB::bind_method(D_METHOD("util_random_color"), &PaintNode::util_random_color);
	ClassDB::bind_method(D_METHOD("util_random_color_alt"), &PaintNode::util_random_color_alt);

	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_forward_canvas_gui_input", PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "InputEvent")));
	ClassDB::bind_method(D_METHOD("forward_canvas_gui_input", "event"), &PaintNode::forward_canvas_gui_input);
	ClassDB::bind_method(D_METHOD("_forward_canvas_gui_input", "event"), &PaintNode::_forward_canvas_gui_input);

	ClassDB::bind_method(D_METHOD("get_paint_project"), &PaintNode::get_paint_project);
	ClassDB::bind_method(D_METHOD("get_parent_paint_node"), &PaintNode::get_parent_paint_node);
	ClassDB::bind_method(D_METHOD("find_parent_paint_node"), &PaintNode::find_parent_paint_node);

	BIND_CONSTANT(NOTIFICATION_PARENT_PAINT_NODE_RESIZED);
	BIND_CONSTANT(NOTIFICATION_PAINT_PROJECT_PRE_RENDER);
	BIND_CONSTANT(NOTIFICATION_PAINT_PROJECT_POST_RENDER);
}
