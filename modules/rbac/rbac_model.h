#ifndef RBAC_MODEL_H
#define RBAC_MODEL_H

#include "core/http/model.h"

#include <string>
#include <vector>

class RBACModel : public Model {
	RCPP_OBJECT(RBACModel, Model);

public:

	void create_table();
	void drop_table();
	void migrate();

	static RBACModel *get_singleton();

	RBACModel();
	~RBACModel();

protected:
	static RBACModel *_self;

	std::string _table_name;
};

#endif