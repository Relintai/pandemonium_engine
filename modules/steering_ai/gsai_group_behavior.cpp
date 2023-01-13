
#include "gsai_group_behavior.h"


GSAIProximity GSAIGroupBehavior::get_*proximity() {
 return *proximity;
}

void GSAIGroupBehavior::set_*proximity(const GSAIProximity &val) {
*proximity = val;
}


Ref<FuncRef> GSAIGroupBehavior::get__callback() {
 return _callback;
}

void GSAIGroupBehavior::set__callback(const Ref<FuncRef> &val) {
_callback = val;
}



 // Base type for group-based steering behaviors.;
 // @category - Base types;
 // Container to find neighbors of the agent and calculate group behavior.;
  GSAIProximity *proximity;
  Ref<FuncRef> _callback = funcref(self, "_report_neighbor");

 FuncRef GSAIGroupBehavior::get_callback() {
  return _callback;
}

 // Internal callback for the behavior to define whether or not a member is;
 // relevant;
 // @tags - virtual;

 bool GSAIGroupBehavior::_report_neighbor(const GSAISteeringAgent &_neighbor) {
  return false;
}

}

 GSAIGroupBehavior::GSAIGroupBehavior() {
  *proximity;
  _callback = funcref(self, "_report_neighbor");
 }

 GSAIGroupBehavior::~GSAIGroupBehavior() {
 }


 static void GSAIGroupBehavior::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*proximity"), &GSAIGroupBehavior::get_*proximity);
   ClassDB::bind_method(D_METHOD("set_*proximity", "value"), &GSAIGroupBehavior::set_*proximity);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*proximity", PROPERTY_HINT_RESOURCE_TYPE, "GSAIProximity"), "set_*proximity", "get_*proximity");


   ClassDB::bind_method(D_METHOD("get__callback"), &GSAIGroupBehavior::get__callback);
   ClassDB::bind_method(D_METHOD("set__callback", "value"), &GSAIGroupBehavior::set__callback);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_callback", PROPERTY_HINT_RESOURCE_TYPE, "Ref<FuncRef>"), "set__callback", "get__callback");


  ClassDB::bind_method(D_METHOD("get_callback"), &GSAIGroupBehavior::get_callback);
  ClassDB::bind_method(D_METHOD("_report_neighbor", "_neighbor"), &GSAIGroupBehavior::_report_neighbor);

 }



