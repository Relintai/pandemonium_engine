#ifndef PROP_2D_DATA_PORTAL_H
#define PROP_2D_DATA_PORTAL_H
/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "prop_2d_data_entry.h"

class Prop2DDataPortal : public Prop2DDataEntry {
	GDCLASS(Prop2DDataPortal, Prop2DDataEntry);

public:
	bool get_is_active() const;
	void set_is_active(bool p_active);

	bool get_is_two_way() const;
	void set_is_two_way(bool p_two_way);

	bool get_use_default_margin() const;
	void set_use_default_margin(bool p_use);

	real_t get_portal_margin() const;
	void set_portal_margin(real_t p_margin);

	PoolVector<Vector2> get_points() const;
	void set_points(const PoolVector<Vector2> &p_points);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);

	Prop2DDataPortal();
	~Prop2DDataPortal();

protected:
	static void _bind_methods();

private:
	bool _is_active;
	bool _is_two_way;
	bool _use_default_margin;
	real_t _portal_margin;
	PoolVector<Vector2> _points;
};

#endif
