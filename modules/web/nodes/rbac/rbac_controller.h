#ifndef RBAC_CONTROLLER_H
#define RBAC_CONTROLLER_H

#include <map>

#include "web_modules/admin_panel/admin_node.h"

#include "core/containers/vector.h"
#include "core/string.h"

#include "rbac_permission.h"
#include "rbac_rank.h"

class Request;
class FormValidator;

class RBACController : public AdminNode {
	RCPP_OBJECT(RBACController, AdminNode);

public:
	void handle_request_main(Request *request);
	void create_validators();

	void admin_handle_request_main(Request *request);
	String admin_get_section_name();
	void admin_add_section_links(Vector<AdminSectionLinkInfo> *links);

	struct RBACAdminRankViewData {
		Ref<RBACRank> rank;
		Vector<String> messages;

		int id = 0;
		String name = "";
		String name_internal = "";
		String settings = "";
		int rank_permissions = 0;
	};

	void admin_handle_new_rank(Request *request);
	void admin_handle_edit_rank(Request *request);
	void render_rank_view(Request *request, RBACAdminRankViewData *data);

	struct RBACAdminEditPermissionView {
		Ref<RBACRank> rank;
		Ref<RBACPermission> permission;
		Vector<String> messages;

		int rank_id = 0;
		int permission_id = 0;
	};

	void admin_permission_editor(Request *request);
	void admin_render_permission_editor_main_view(Request *request, RBACAdminEditPermissionView *data);
	void admin_render_permission_editor_entry_edit_create_view(Request *request, RBACAdminEditPermissionView *data);
	bool admin_process_permission_editor_entry_edit_create_post(Request *request, RBACAdminEditPermissionView *data);

	void admin_render_rank_list(Request *request);
	void admin_render_rank_editor(Request *request);

	void register_permission(const String &name, const int val);
	void register_rank_permission(const String &name, const int val);
	void clear_registered_permissions();

	void initialize();
	virtual void register_permissions();

	Ref<RBACRank> get_rank(int rank_id);

	int get_default_user_rank_id();
	Ref<RBACRank> get_default_user_rank();

	int get_default_rank_id();
	Ref<RBACRank> get_default_rank();

	String &get_redirect_url();

	bool continue_on_missing_default_rank();

	// db

	virtual std::map<int, Ref<RBACRank> > db_load_ranks();

	virtual void db_save(const Ref<RBACRank> &rank);
	virtual void db_save_rank(const Ref<RBACRank> &rank);
	virtual void db_save_permission(const Ref<RBACPermission> &permission);
	virtual int db_get_default_rank();
	virtual int db_get_default_user_rank();
	virtual String db_get_redirect_url();

	void create_table();
	void drop_table();
	void create_default_entries();

	static RBACController *get_singleton();

	RBACController();
	~RBACController();

protected:
	static RBACController *_self;

	int _default_rank_id;
	int _default_user_rank_id;
	std::map<int, Ref<RBACRank> > _ranks;
	String _redirect_url;

	struct PermissionEntry {
		String name;
		int value;

		PermissionEntry() {
		}

		PermissionEntry(const String &p_name, const int p_val) {
			name = p_name;
			value = p_val;
		}
	};

	String _rbac_ranks_table;
	String _rbac_permissions_table;

	Vector<PermissionEntry> _registered_permissions;
	Vector<PermissionEntry> _registered_rank_permissions;
};

#endif