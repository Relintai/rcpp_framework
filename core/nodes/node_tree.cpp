
#include "node_tree.h"

#include "node.h"

Node *NodeTree::get_root() {
	return _root_node;
}

void NodeTree::set_root(Node *root) {
	if (_root_node) {
		_root_node->set_tree(nullptr);
	}

	_root_node = root;

	if (_root_node) {
		_root_node->set_tree(this);
	}
}

void NodeTree::send_update(float delta) {
	// todo
}

NodeTree::NodeTree() :
		Object() {

	_root_node = nullptr;
	_update_interval = 0;
}

NodeTree::~NodeTree() {
	if (_root_node) {
		delete _root_node;
		_root_node = nullptr;
	}
}