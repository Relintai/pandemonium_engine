

#include "editor_plugin_gltf_mdr.h"

void EditorPluginGLTFMdr::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
			_importer.instance();

			add_import_plugin(_importer);

			break;
		case NOTIFICATION_EXIT_TREE:
			remove_import_plugin(_importer);

			_importer.unref();

			break;
	}
}

EditorPluginGLTFMdr::EditorPluginGLTFMdr(EditorNode *node) {
	_node = node;
}
