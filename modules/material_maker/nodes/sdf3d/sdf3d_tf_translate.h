#ifndef SDF3D_TF_TRANSLATE_H
#define SDF3D_TF_TRANSLATE_H


class Sdf3dTfTranslate : public MMNode {
 GDCLASS(Sdf3dTfTranslate, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector3 get_translation();
 void set_translation(const Vector3 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Vector2 _get_property_value_sdf3d(const Vector3 &uv3);
 Vector3 get_translation();
 void set_translation(const Vector3 &val);
 void on_input_changed();

 Sdf3dTfTranslate();
 ~Sdf3dTfTranslate();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector3) 
 Vector3 translation = Vector3();
 //translation
};


#endif
