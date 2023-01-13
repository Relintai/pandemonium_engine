#ifndef GSAITARGETACCELERATION_H
#define GSAITARGETACCELERATION_H


class GSAITargetAcceleration : public Reference {
 GDCLASS(GSAITargetAcceleration, Reference);

 public:

 Vector3 get_linear();
 void set_linear(const Vector3 &val);

 float get_angular() const;
 void set_angular(const float val);

 void set_zero();
 void add_scaled_accel(const GSAITargetAcceleration &accel, const float scalar);
 float get_magnitude_squared();
 float get_magnitude();

 GSAITargetAcceleration();
 ~GSAITargetAcceleration();

 protected:
 static void _bind_methods();

 // A desired linear and angular amount of acceleration requested by the steering
 // system.
 // @category - Base types
 // Linear acceleration
 Vector3 linear = Vector3.ZERO;
 // Angular acceleration
 float angular = 0.0;
 // Sets the linear and angular components to 0.
 // Adds `accel`'s components, multiplied by `scalar`, to this one.
 // Returns the squared magnitude of the linear and angular components.
 // Returns the magnitude of the linear and angular components.
};


#endif
