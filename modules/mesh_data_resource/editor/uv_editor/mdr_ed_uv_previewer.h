#ifndef MDR_ED_UV_PREVIEWER_H
#define MDR_ED_UV_PREVIEWER_H



#include "scene/main/control.h"

#include "core/object/reference.h"

class MeshDataResource;
class MeshDataInstance;
class Texture;

class MDREdUVPreviewer : public Control {
	GDCLASS(MDREdUVPreviewer, Control);

public:
	void set_mesh_data_resource(const Ref<MeshDataResource> &a);
	void set_mesh_data_instance(MeshDataInstance *a);
	void on_mdr_changed();
	void _draw();

	MDREdUVPreviewer();
	~MDREdUVPreviewer();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	Ref<MeshDataResource> mesh_data_resource;
	Ref<Texture> background_texture;
};

#endif
