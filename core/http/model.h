#ifndef HTTP_MODEL_H
#define HTTP_MODEL_H

#include "core/object.h"

class Model : public Object {
public:
	virtual void create_table();
	virtual void drop_table();
	virtual void migrate();
	
	Model();
	~Model();

protected:
};

#endif