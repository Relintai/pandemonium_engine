
#include "dummy_navigation_server.h"

#include "scene/resources/navigation_mesh.h"

void DummyNavigationServer::region_set_navmesh(RID p_region, Ref<NavigationMesh> p_nav_mesh) const {
}

NavigationUtilities::PathQueryResult DummyNavigationServer::_query_path(const NavigationUtilities::PathQueryParameters &p_parameters) const {
	return NavigationUtilities::PathQueryResult();
}

DummyNavigationServer::DummyNavigationServer() {
}

DummyNavigationServer::~DummyNavigationServer() {
}