#include "paint_node.h"

#include "paint_project.h"

Vector2i PaintNode::get_size() {
	return _size;
}
void PaintNode::set_size(const Vector2i &size) {
	_size = size;
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

PaintNode::PaintNode() {
}

PaintNode::~PaintNode() {
}

void PaintNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_size"), &PaintNode::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &PaintNode::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
}
