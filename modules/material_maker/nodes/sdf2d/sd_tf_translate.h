#ifndef SD_TF_TRANSLATE_H
#define SD_TF_TRANSLATE_H


class SdTfTranslate : public MMNode {
 GDCLASS(SdTfTranslate, MMNode);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector2 get_translation();
 void set_translation(const Vector2 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _get_property_value(const Vector2 &uv);
 Vector2 get_translation();
 void set_translation(const Vector2 &val);

 SdTfTranslate();
 ~SdTfTranslate();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector2) 
 Vector2 translation = Vector2(0, 0);
 //a
};


#endif
