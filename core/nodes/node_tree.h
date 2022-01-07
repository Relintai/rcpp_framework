#ifndef NODE_TREE_H
#define NODE_TREE_H

#include "core/object.h"

//should be a template
//It should be inherited from
//Similar idea to godot's scenetree
//Should handle all functionality of base nodes
//refreshes etc -> good for web nodes aswell -> can use dirtying if there are updates, or clearing caches
//Should have a root node (templated)
//Nodes should have a tree set into them (not templated) -> set() should be virtual

//For the web:
//The webserver could be a webtree, the nodes themselves could handle routing with their hierarchy (easy to cache) would have the same speed
//Routing could be overridable per node
//MIddlewares can be handled by the tree before sending the request to the tree, also files (that could be a middleware aswell)
//WebTree : public NodeTree<WebNode>
//and then Server : public WebTree
//This way webapps could be build like guis in godot.

//WebTree
// - WebRoot   (Normal WebNode - if uri is '/news/...' if it has a news node, pushes the uri stack and forwards the request to that node. Else 404
// ---- news    (News node f.e. - handles request, stops forwarding)
// ---- projects   (Projects node, either forwards, or if /projects sends back a list etc)
// -------- p1
//etc

class NodeTree : public Object {
public:
	NodeTree();
	~NodeTree();

protected:
};

#endif