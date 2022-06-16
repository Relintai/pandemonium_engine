
#include "sd_shape_polygon.h"


Ref<Resource> SdShapePolygon::get_output() {
 return output;
}

void SdShapePolygon::set_output(const Ref<Resource> &val) {
output = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> output;

 void SdShapePolygon::_init_properties() {

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
  output.slot_name = ">>>   Output    >>>";
  output.get_value_from_owner = true;
  register_output_property(output);
}


 void SdShapePolygon::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(output);
  mm_graph_node.add_slot_polygon();
}


 float SdShapePolygon::_get_property_value(const Vector2 &uv) {
  return MMAlgos.sdPolygon(uv, points);
}


 void SdShapePolygon::_polygon_changed() {
  emit_changed();
  output.emit_changed();
}

}

 SdShapePolygon::SdShapePolygon() {
  output;
 }

 SdShapePolygon::~SdShapePolygon() {
 }


 static void SdShapePolygon::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_output"), &SdShapePolygon::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &SdShapePolygon::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SdShapePolygon::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SdShapePolygon::_register_methods);
  ClassDB::bind_method(D_METHOD("_get_property_value", "uv"), &SdShapePolygon::_get_property_value);
  ClassDB::bind_method(D_METHOD("_polygon_changed"), &SdShapePolygon::_polygon_changed);

 }



