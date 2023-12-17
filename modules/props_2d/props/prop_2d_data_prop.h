#ifndef PROP_2D_DATA_PROP_2D_H
#define PROP_2D_DATA_PROP_2D_H


#include "core/math/vector3.h"

#include "prop_2d_data_entry.h"

#include "prop_2d_data.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

class Prop2DDataProp2D : public Prop2DDataEntry {
	GDCLASS(Prop2DDataProp2D, Prop2DDataEntry);

public:
	Ref<Prop2DData> get_prop() const;
	void set_prop(const Ref<Prop2DData> value);

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	void _add_textures_into(Ref<TexturePacker> texture_packer);
#endif

	bool _processor_handles(Node *node);
	void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);

	Prop2DDataProp2D();
	~Prop2DDataProp2D();

protected:
	static void _bind_methods();

private:
	bool _snap_to_mesh;
	Vector3 _snap_axis;
	Ref<Prop2DData> _prop;
};

#endif
