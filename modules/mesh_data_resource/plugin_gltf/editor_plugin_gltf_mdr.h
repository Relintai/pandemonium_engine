#ifndef EDITOR_PLUGIN_GLTF_MDR
#define EDITOR_PLUGIN_GLTF_MDR


#include "core/string/ustring.h"

#include "editor/editor_plugin.h"

#include "editor_import_gltf_mdr.h"

class EditorPluginGLTFMdr : public EditorPlugin {
	GDCLASS(EditorPluginGLTFMdr, EditorPlugin);

public:
	EditorPluginGLTFMdr(EditorNode *node);

protected:
	void _notification(int p_what);

private:
	EditorNode *_node;
	Ref<EditorImportGLTFMdr> _importer;
};

#endif
