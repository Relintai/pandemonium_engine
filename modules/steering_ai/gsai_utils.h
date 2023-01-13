#ifndef GSAIUTILS_H
#define GSAIUTILS_H

class GSAIUtils {
public:
	static Vector3 clampedv3(const Vector3 &vector, const float limit);
	static float vector3_to_angle(const Vector3 &vector);
	static float vector2_to_angle(const Vector2 &vector);
	static Vector2 angle_to_vector2(const float angle);
	static Vector2 to_vector2(const Vector3 &vector);
	static Vector3 to_vector3(const Vector2 &vector);

	GSAIUtils();
	~GSAIUtils();

protected:
	static void _bind_methods();

	// Math and vector utility functions.
	// @Category - Utilities
	// Returns the `vector` with its length capped to `limit`.
	// Returns an angle in radians between the positive X axis and the `vector`.
	//
	// This assumes orientation for 3D agents that are upright and rotate
	// around the Y axis.
	// Returns an angle in radians between the positive X axis and the `vector`.
	// Returns a directional vector from the given orientation angle.
	//
	// This assumes orientation for 2D agents or 3D agents that are upright and
	// rotate around the Y axis.
	// Returns a vector2 with `vector`'s x and y components.
	// Returns a vector3 with `vector`'s x and y components and 0 in z.
};

#endif
