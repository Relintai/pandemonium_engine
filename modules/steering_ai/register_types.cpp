/*************************************************************************/
/*  register_types.cpp                                                   */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

/*
Copyright (c) 2023-present Péter Magyar
Copyright (c) 2020-2023 GDQuest

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "register_types.h"

#include "core/config/engine.h"

#include "gsai_utils.h"

#include "gsai_agent_location.h"
#include "gsai_group_behavior.h"
#include "gsai_path.h"
#include "gsai_steering_agent.h"
#include "gsai_steering_behavior.h"
#include "gsai_target_acceleration.h"
#include "proximities/gsai_proximity.h"

#include "proximities/gsai_infinite_proximity.h"
#include "proximities/gsai_radius_proximity.h"

#include "behaviors/gsai_arrive.cpp"
#include "behaviors/gsai_avoid_collisions.cpp"
#include "behaviors/gsai_blend.cpp"
#include "behaviors/gsai_cohesion.cpp"
#include "behaviors/gsai_evade.cpp"
#include "behaviors/gsai_face.cpp"
#include "behaviors/gsai_flee.cpp"
#include "behaviors/gsai_follow_path.cpp"
#include "behaviors/gsai_look_where_you_go.cpp"
#include "behaviors/gsai_match_orientation.cpp"
#include "behaviors/gsai_priority.cpp"
#include "behaviors/gsai_pursue.cpp"
#include "behaviors/gsai_seek.cpp"
#include "behaviors/gsai_separation.cpp"

#include "agents/gsai_kinematic_body_2d_agent.cpp"
#include "agents/gsai_kinematic_body_3d_agent.cpp"
#include "agents/gsai_rigid_body_2d_agent.cpp"
#include "agents/gsai_rigid_body_3d_agent.cpp"
#include "agents/gsai_specialized_agent.cpp"

static GSAIUtils *gs_ai_utils = NULL;

void register_steering_ai_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		gs_ai_utils = memnew(GSAIUtils);
		ClassDB::register_class<GSAIUtils>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("GSAIUtils", GSAIUtils::get_singleton()));
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<GSAITargetAcceleration>();
		ClassDB::register_class<GSAISteeringBehavior>();
		ClassDB::register_class<GSAISteeringAgent>();
		ClassDB::register_class<GSAIPath>();
		ClassDB::register_class<GSAIGroupBehavior>();
		ClassDB::register_class<GSAIAgentLocation>();
		ClassDB::register_class<GSAIProximity>();

		ClassDB::register_class<GSAIRadiusProximity>();
		ClassDB::register_class<GSAIInfiniteProximity>();

		ClassDB::register_class<GSAIArrive>();
		ClassDB::register_class<GSAIAvoidCollisions>();
		ClassDB::register_class<GSAIBlend>();
		ClassDB::register_class<GSAICohesion>();
		ClassDB::register_class<GSAIEvade>();
		ClassDB::register_class<GSAIFace>();
		ClassDB::register_class<GSAIFlee>();
		ClassDB::register_class<GSAIFollowPath>();
		ClassDB::register_class<GSAILookWhereYouGo>();
		ClassDB::register_class<GSAIMatchOrientation>();
		ClassDB::register_class<GSAIPriority>();
		ClassDB::register_class<GSAIPursue>();
		ClassDB::register_class<GSAISeek>();
		ClassDB::register_class<GSAISeparation>();

		ClassDB::register_class<GSAIKinematicBody2DAgent>();
		ClassDB::register_class<GSAIKinematicBody3DAgent>();
		ClassDB::register_class<GSAIRigidBody2DAgent>();
		ClassDB::register_class<GSAIRigidBody3DAgent>();
		ClassDB::register_class<GSAISpecializedAgent>();
	}
}

void unregister_steering_ai_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		if (gs_ai_utils) {
			memdelete(gs_ai_utils);
		}
	}
}
