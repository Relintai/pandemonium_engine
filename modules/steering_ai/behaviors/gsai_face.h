#ifndef GSAI_FACE_H
#define GSAI_FACE_H

#include "core/object/reference.h"

#include "gsai_match_orientation.h"

class GSAITargetAcceleration;

class GSAIFace : public GSAIMatchOrientation {
	GDCLASS(GSAIFace, GSAIMatchOrientation);

public:
	void face(const Ref<GSAITargetAcceleration> &acceleration, const Vector3 &target_position);
	virtual void _face(Ref<GSAITargetAcceleration> acceleration, Vector3 target_position);

	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAIFace();
	~GSAIFace();

protected:
	static void _bind_methods();
};

#endif
