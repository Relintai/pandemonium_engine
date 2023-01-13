
#include "gsai_infinite_proximity.h"



 // Determines any agent that is in the specified list as being neighbors with the;
 // owner agent, regardless of distance.;
 // @category - Proximities;
 // Returns a number of neighbors based on a `callback` function.;
 //;
 // `_find_neighbors` calls `callback` for each agent in the `agents` array and;
 // adds one to the count if its `callback` returns true.;
 // @tags - virtual;

 int GSAIInfiniteProximity::_find_neighbors(const FuncRef &callback) {
   int neighbor_count = 0;
   int agent_count = agents.size();

  for (int i = 0; i < agent_count; ++i) { //i in range(agent_count)
    GSAISteeringAgent *current_agent = agents[i] as GSAISteeringAgent;

   if (current_agent != agent) {

    if (callback.call_func(current_agent)) {
     neighbor_count += 1;
}

}

}

  return neighbor_count;
}

}

 GSAIInfiniteProximity::GSAIInfiniteProximity() {
 }

 GSAIInfiniteProximity::~GSAIInfiniteProximity() {
 }


 static void GSAIInfiniteProximity::_bind_methods() {
  ClassDB::bind_method(D_METHOD("_find_neighbors", "callback"), &GSAIInfiniteProximity::_find_neighbors);

 }



