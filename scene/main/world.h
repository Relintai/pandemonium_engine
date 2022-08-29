#ifndef WORLD_H
#define WORLD_H

#include "node.h"

class World : public Node {
	GDCLASS(World, Node);

public:
	World();
	~World();

protected:
	void _notification(int p_what);

	static void _bind_methods();
};

#endif
