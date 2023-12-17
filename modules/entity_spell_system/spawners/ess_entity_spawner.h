#ifndef ESS_ENTITY_SPAWNER_H
#define ESS_ENTITY_SPAWNER_H


#include "scene/main/node.h"

#include "scene/main/scene_tree.h"

class EntityCreateInfo;

class ESSEntitySpawner : public Node {
	GDCLASS(ESSEntitySpawner, Node);

public:
	void request_entity_spawn(Ref<EntityCreateInfo> info);
	void request_entity_spawn_deferred(Ref<EntityCreateInfo> info);

	ESSEntitySpawner();
	~ESSEntitySpawner();

protected:
	static void _bind_methods();

private:
};

#endif
