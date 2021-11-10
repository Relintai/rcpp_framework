#ifndef RBAC_MODEL_H
#define RBAC_MODEL_H

#include "core/http/model.h"

#include <map>
#include "core/containers/vector.h"
#include "core/string.h"

#include "rbac_permission.h"
#include "rbac_rank.h"

class RBACModel : public Model {
	RCPP_OBJECT(RBACModel, Model);

public:
	virtual std::map<int, Ref<RBACRank> > load_ranks();

	virtual void save(const Ref<RBACRank> &rank);
	virtual void save_rank(const Ref<RBACRank> &rank);
	virtual void save_permission(const Ref<RBACPermission> &permission);
	virtual int get_default_rank();
	virtual String get_redirect_url();

	void create_table();
	void drop_table();
	void migrate();
	virtual void create_default_entries();

	static RBACModel *get_singleton();

	RBACModel();
	~RBACModel();

protected:
	static RBACModel *_self;
};

#endif