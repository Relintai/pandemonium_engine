#ifndef GSAIPRIORITY_H
#define GSAIPRIORITY_H


class GSAIPriority : public GSAISteeringBehavior {
 GDCLASS(GSAIPriority, GSAISteeringBehavior);

 public:

 float get_zero_threshold() const;
 void set_zero_threshold(const float val);

 int get__last_selected_index() const;
 void set__last_selected_index(const int val);

 Array get__behaviors();
 void set__behaviors(const Array &val);

 void add_behavior(const GSAISteeringBehavior &behavior);
 GSAISteeringBehavior get_behavior(const int index);
 void remove_behavior(const int index);
 int get_behaviour_count();
 void _calculate_steering(const GSAITargetAcceleration &accel);

 GSAIPriority();
 ~GSAIPriority();

 protected:
 static void _bind_methods();

 // Container for multiple behaviors that returns the result of the first child
 // behavior with non-zero acceleration.
 // @category - Combination behaviors
 // If a behavior's acceleration is lower than this threshold, the container
 // considers it has an acceleration of zero.
 float zero_threshold = 0.0;
 // The index of the last behavior the container prioritized.
 int _last_selected_index = 0;
 Array _behaviors = Array();
 // Appends a steering behavior as a child of this container.
 // Returns the behavior at the position in the pool referred to by `index`, or
 // `null` if no behavior was found.
};


#endif
