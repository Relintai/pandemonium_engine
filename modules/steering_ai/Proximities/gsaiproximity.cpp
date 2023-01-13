
#include "gsaiproximity.h"


GSAISteeringAgent GSAIProximity::get_*agent() {
 return *agent;
}

void GSAIProximity::set_*agent(const GSAISteeringAgent &val) {
*agent = val;
}


Array GSAIProximity::get_agents() {
 return agents;
}

void GSAIProximity::set_agents(const Array &val) {
agents = val;
}



 // Base container type that stores data to find the neighbors of an agent.;
 // @category - Proximities;
 // @tags - abstract;
 // The owning agent whose neighbors are found in the group;
  GSAISteeringAgent *agent;
 // The agents who are part of this group and could be potential neighbors;
  Array agents = Array();

 int GSAIProximity::find_neighbors(const FuncRef &_callback) {
  return call("_find_neighbors", _callback);
}

 // Returns a number of neighbors based on a `callback` function.;
 //;
 // `_find_neighbors` calls `callback` for each agent in the `agents` array and;
 // adds one to the count if its `callback` returns true.;
 // @tags - virtual;

 int GSAIProximity::_find_neighbors(const FuncRef &_callback) {
  return 0;
}

}

 GSAIProximity::GSAIProximity() {
  *agent;
  agents = Array();
 }

 GSAIProximity::~GSAIProximity() {
 }


 static void GSAIProximity::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*agent"), &GSAIProximity::get_*agent);
   ClassDB::bind_method(D_METHOD("set_*agent", "value"), &GSAIProximity::set_*agent);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*agent", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent"), "set_*agent", "get_*agent");


   ClassDB::bind_method(D_METHOD("get_agents"), &GSAIProximity::get_agents);
   ClassDB::bind_method(D_METHOD("set_agents", "value"), &GSAIProximity::set_agents);
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "agents"), "set_agents", "get_agents");


  ClassDB::bind_method(D_METHOD("find_neighbors", "_callback"), &GSAIProximity::find_neighbors);
  ClassDB::bind_method(D_METHOD("_find_neighbors", "_callback"), &GSAIProximity::_find_neighbors);

 }



