#ifndef GSAIFACE_H
#define GSAIFACE_H


class GSAIFace : public GSAIMatchOrientation {
 GDCLASS(GSAIFace, GSAIMatchOrientation);

 public:

 void face(const GSAITargetAcceleration &acceleration, const Vector3 &target_position);
 void _face(const GSAITargetAcceleration &acceleration, const Vector3 &target_position);
 void _calculate_steering(const GSAITargetAcceleration &acceleration);

 GSAIFace();
 ~GSAIFace();

 protected:
 static void _bind_methods();

 // Calculates angular acceleration to rotate a target to face its target's
 // position. The behavior attemps to arrive with zero remaining angular velocity.
 // @category - Individual behaviors
};


#endif
