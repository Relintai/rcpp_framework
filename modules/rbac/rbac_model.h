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
	virtual std::map<int, Ref<RBACRank> > load_permissions();

	virtual void save(const Ref<RBACRank> &rank);
	virtual void save_rank(const Ref<RBACRank> &rank);
	virtual void save_permission(const Ref<RBACPermission> &permission);

	void create_table();
	void drop_table();
	void migrate();

	static RBACModel *get_singleton();

	RBACModel();
	~RBACModel();

protected:
	static RBACModel *_self;
};

#endif