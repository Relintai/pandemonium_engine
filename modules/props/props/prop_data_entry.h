#ifndef PROP_DATA_DATA_H
#define PROP_DATA_DATA_H


#include "core/math/transform.h"
#include "core/object/resource.h"

#include "modules/modules_enabled.gen.h"

class PropData;
class PropMesher;
class TexturePacker;

class PropDataEntry : public Resource {
	GDCLASS(PropDataEntry, Resource);

public:
	Transform get_transform() const;
	void set_transform(const Transform &value);

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	void add_textures_into(Ref<TexturePacker> texture_packer);
#endif

	bool processor_handles(Node *node);
	void processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *processor_get_node_for(const Transform &transform);
	bool processor_evaluate_children();

	virtual bool _processor_handles(Node *node);
	virtual void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	virtual Node *_processor_get_node_for(const Transform &transform);
	virtual bool _processor_evaluate_children();

	PropDataEntry();
	~PropDataEntry();

protected:
	static void _bind_methods();

private:
	Transform _transform;
};

#endif
