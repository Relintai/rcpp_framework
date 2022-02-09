#ifndef NODE_TREE_H
#define NODE_TREE_H

#include "core/object.h"
#include "core/threading/rw_lock.h"

class Node;

class NodeTree : public Object {
	RCPP_OBJECT(NodeTree, Object);

public:
	Node *get_root();
	virtual void set_root(Node *root);

	virtual void send_update(float delta);

	void request_write_lock();

	NodeTree();
	~NodeTree();

protected:
	Node *_root_node;
	float _update_interval;
	bool _write_lock_requested;
	RWLock _rw_lock;
};

#endif