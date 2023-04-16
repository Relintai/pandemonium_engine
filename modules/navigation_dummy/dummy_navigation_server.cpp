
#include "dummy_navigation_server.h"

#include "scene/resources/navigation_mesh.h"

void DummyNavigationServer::region_set_navmesh(RID p_region, Ref<NavigationMesh> p_nav_mesh) const {
}
void DummyNavigationServer::region_bake_navmesh(Ref<NavigationMesh> r_mesh, Node *p_node) const {
}

DummyNavigationServer::DummyNavigationServer() {
}

DummyNavigationServer::~DummyNavigationServer() {
}