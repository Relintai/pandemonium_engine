

#include "model_visual.h"

#include "../../singletons/ess.h"

#include "../../defines.h"

int ModelVisual::get_layer() {
	return _layer;
}
void ModelVisual::set_layer(int layer) {
	_layer = layer;
}

//ModelVisualEntries

Ref<ModelVisualEntry> ModelVisual::get_visual_entry(const int index) const {
	ERR_FAIL_INDEX_V(index, _visual_entries.size(), Ref<ModelVisualEntry>());

	return _visual_entries.get(index);
}
void ModelVisual::set_visual_entry(const int index, const Ref<ModelVisualEntry> visual_entry) {
	ERR_FAIL_INDEX(index, _visual_entries.size());

	_visual_entries.set(index, visual_entry);
}
void ModelVisual::add_visual_entry(const Ref<ModelVisualEntry> visual_entry) {
	_visual_entries.push_back(visual_entry);
}
void ModelVisual::remove_visual_entry(const int index) {
	ERR_FAIL_INDEX(index, _visual_entries.size());

	_visual_entries.remove(index);
}

int ModelVisual::get_visual_entry_count() const {
	return _visual_entries.size();
}

Vector<Variant> ModelVisual::get_visual_entries() {
	VARIANT_ARRAY_GET(_visual_entries);
}
void ModelVisual::set_visual_entries(const Vector<Variant> &visual_entries) {
	VARIANT_ARRAY_SET(visual_entries, _visual_entries, ModelVisualEntry);
}

ModelVisual::ModelVisual() {
	_layer = 0;
}

ModelVisual::~ModelVisual() {
	_visual_entries.clear();
}

void ModelVisual::_validate_property(PropertyInfo &property) const {
	String name = property.name;

	if (name == "layer") {
		property.hint_string = ESS::get_singleton()->texture_layers_get();
	}
}

#ifndef DISABLE_DEPRECATED
// TODO REMOVE AFTER NEXT RELEASE
bool ModelVisual::_set(const StringName &p_name, const Variant &p_value) {
	if (p_name == "get_visual_entries") {
		set_visual_entries(p_value);

		return true;
	}

	return false;
}
#endif

void ModelVisual::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_layer"), &ModelVisual::get_layer);
	ClassDB::bind_method(D_METHOD("set_layer", "layer"), &ModelVisual::set_layer);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "layer", PROPERTY_HINT_ENUM, ""), "set_layer", "get_layer");

	//ModelVisualEntry
	ClassDB::bind_method(D_METHOD("get_visual_entry", "index"), &ModelVisual::get_visual_entry);
	ClassDB::bind_method(D_METHOD("set_visual_entry", "index", "data"), &ModelVisual::set_visual_entry);
	ClassDB::bind_method(D_METHOD("add_visual_entry", "visual_entry"), &ModelVisual::add_visual_entry);
	ClassDB::bind_method(D_METHOD("remove_visual_entry", "index"), &ModelVisual::remove_visual_entry);

	ClassDB::bind_method(D_METHOD("get_visual_entry_count"), &ModelVisual::get_visual_entry_count);

	ClassDB::bind_method(D_METHOD("get_visual_entries"), &ModelVisual::get_visual_entries);
	ClassDB::bind_method(D_METHOD("set_visual_entries", "visual_entrys"), &ModelVisual::set_visual_entries);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "visual_entries", PROPERTY_HINT_NONE, "23/20:ModelVisualEntry", PROPERTY_USAGE_DEFAULT, "ModelVisualEntry"), "set_visual_entries", "get_visual_entries");
}
