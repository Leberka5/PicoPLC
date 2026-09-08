// do not include this file!

#pragma once

/**********************************
 * node methods
 **********************************/
template<typename T>
tree<T>::node::node(node *parent, T data) :
    parent(parent),
    data(data) { }


template<typename T>
tree<T>::node::~node()
{
    for (auto& nodeCurr : children)
        delete nodeCurr;
}

template<typename T>
typename tree<T>::node* tree<T>::node::addNode(T dataChild)
{
    node* nodeNew = new node(this, dataChild);
    children.push_back(nodeNew);
    leaf = false;
    return nodeNew;
}

template<typename T>
typename tree<T>::node* tree<T>::node::getChild(uint16_t iNode)
{
    return children.at(iNode);
}

template<typename T>
typename tree<T>::node * tree<T>::node::getParent() { return parent; }

template<typename T>
std::vector<class tree<T>::node *> tree<T>::node::getChildren() { return children; }

template<typename T>
uint16_t tree<T>::node::getNumChildren() { return children.size(); }

template<typename T>
T tree<T>::node::getData() { return data; }

template<typename T>
T * tree<T>::node::getDataPtr() { return &data; }

template<typename T>
bool tree<T>::node::isLeaf() { return leaf; }


/**********************************
 * tree methods
 **********************************/
template<typename T>
tree<T>::tree()
{
    //root = new node(nullptr, data);
}

template<typename T>
tree<T>::~tree()
{
    delete root;
}

template<typename T>
typename tree<T>::node* tree<T>::setRoot(T nodeData)
{
    root = new node(nullptr, nodeData);
    return root;
}

template<typename T>
typename tree<T>::node * tree<T>::getRoot() { return root; }

template<typename T>
std::vector<class tree<T>::node *> tree<T>::getRootNodes() { return root->getNodes(); }
