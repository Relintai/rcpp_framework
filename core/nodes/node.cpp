
#include "node.h"

#include "node_tree.h"

Node *Node::get_parent() {
	return _parent;
}
void Node::set_parent(Node *parent) {
    if (_parent == parent) {
        return;
    }

    if (_parent) {
        notification(NOTIFICATION_UNPARENTED);
    }

	_parent = parent;

    if (_parent) {
        notification(NOTIFICATION_PARENTED);
    }
}

int Node::get_child_count() {
	return _children.size();
}
Node *Node::get_child(int index) {
	return _children[index];
}
void Node::add_child(Node *child) {
	ERR_FAIL_COND(!child);
	ERR_FAIL_COND(child->get_parent());

	_children.push_back(child);
	child->set_parent(this);

    notification(NOTIFICATION_CHILD_ADDED);
}
void Node::remove_child_index(int index) {
    Node *c = _children[index];

	_children.remove_keep_order(index);
	c->set_parent(nullptr);

    notification(NOTIFICATION_CHILD_REMOVED);
}
void Node::remove_child(Node *child) {
	ERR_FAIL_COND(!child);

	for (int i = 0; i < _children.size(); ++i) {
		Node *c = _children[i];

		if (c == child) {
			_children.remove_keep_order(i);
			child->set_parent(nullptr);
            notification(NOTIFICATION_CHILD_REMOVED);
			return;
		}
	}
}

NodeTree *Node::get_tree() {
	return _tree;
}

void Node::set_tree(NodeTree *tree) {
	if (_tree) {
		notification(NOTIFICATION_EXIT_TREE);
	}

	_tree = tree;

	if (_tree) {
		notification(NOTIFICATION_ENTER_TREE);
	}
}

void Node::notification(int what) {
    _notification(what);

	for (int i = 0; i < _children.size(); ++i) {
		_children[i]->notification(what);
	}
}
void Node::_notification(int what) {
}

Node::Node() :
		Object() {

	_parent = nullptr;
	_tree = nullptr;
}

Node::~Node() {
}