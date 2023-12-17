#ifndef PROP_DATA_PORTAL_H
#define PROP_DATA_PORTAL_H


#include "prop_data_entry.h"

class PropDataPortal : public PropDataEntry {
	GDCLASS(PropDataPortal, PropDataEntry);

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
	void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *_processor_get_node_for(const Transform &transform);

	PropDataPortal();
	~PropDataPortal();

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
