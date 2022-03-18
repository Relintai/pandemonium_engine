In case it's needed later

		+ void Skeleton::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < bones.size(); i++) {
		String prep = "bones/" + itos(i) + "/";
		-p_list->push_back(PropertyInfo(Variant::STRING, prep + "name"));
		-p_list->push_back(PropertyInfo(Variant::INT, prep + "parent", PROPERTY_HINT_RANGE, "-1," + itos(bones.size() - 1) + ",1"));
		-p_list->push_back(PropertyInfo(Variant::TRANSFORM, prep + "rest"));
		-p_list->push_back(PropertyInfo(Variant::BOOL, prep + "enabled"));
		+p_list->push_back(PropertyInfo(Variant::STRING, prep + "name", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
		+p_list->push_back(PropertyInfo(Variant::INT, prep + "parent", PROPERTY_HINT_RANGE, "-1," + itos(bones.size() - 1) + ",1", PROPERTY_USAGE_NOEDITOR));
		+p_list->push_back(PropertyInfo(Variant::TRANSFORM, prep + "rest", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
		+p_list->push_back(PropertyInfo(Variant::BOOL, prep + "enabled", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
		p_list->push_back(PropertyInfo(Variant::TRANSFORM, prep + "pose", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR));
		-p_list->push_back(PropertyInfo(Variant::ARRAY, prep + "bound_children"));
		+p_list->push_back(PropertyInfo(Variant::ARRAY, prep + "bound_children", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR));
	}
}
