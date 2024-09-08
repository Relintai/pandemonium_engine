
#include "procedural_tree_3d.h"

ProceduralTree3D::ProceduralTree3D() {
}
ProceduralTree3D::~ProceduralTree3D() {
}

void ProceduralTree3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_PROCESS: {
		} break;
		case NOTIFICATION_ENTER_TREE: {
		} break;
		case NOTIFICATION_EXIT_TREE: {
		} break;
	}
}

void ProceduralTree3D::_bind_methods() {
	/*
	ADD_SIGNAL(MethodInfo("error", PropertyInfo(Variant::DICTIONARY, "error")));

	ClassDB::bind_method(D_METHOD("get_client_id"), &ProceduralTree3D::get_client_id);
	ClassDB::bind_method(D_METHOD("set_client_id", "val"), &ProceduralTree3D::set_client_id);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "client_id"), "set_client_id", "get_client_id");

	ClassDB::bind_method(D_METHOD("get_tls_method"), &ProceduralTree3D::get_tls_method);
	ClassDB::bind_method(D_METHOD("set_tls_method", "val"), &ProceduralTree3D::set_tls_method);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tls_method", PROPERTY_HINT_ENUM, "NONE,STARTTLS,SMTPS"), "set_tls_method", "get_tls_method");

	ClassDB::bind_method(D_METHOD("send_email", "email"), &ProceduralTree3D::send_email);
	*/
}
