// do not include this file!

#pragma once

/**********************************
 * node methods
 **********************************/
template<typename T>
tree<T>::node::node(node *parent) :
    parent(parent) { }

template<typename T>
tree<T>::node::node(node *parent, T data) :
    parent(parent),
    data(data) { }

template<typename T>
tree<T>::node::node(node *parent, std::vector<node *> children) :
    parent(parent),
    nodes(children) { }

template<typename T>
tree<T>::node::~node()
{
    for (auto& nodeCurr : nodes)
        delete nodeCurr;
}

template<typename T>
typename tree<T>::node* tree<T>::node::addNode(T dataChild)
{
    node* nodeNew = new node(this, dataChild);
    nodes.push_back(nodeNew);
    leaf = false;
    return nodeNew;
}

template<typename T>
typename tree<T>::node* tree<T>::node::getNode(uint16_t iNode) { return nodes.at(iNode); }

template<typename T>
std::vector<class tree<T>::node *> tree<T>::node::getAllNodes() { return nodes; }

template<typename T>
uint16_t tree<T>::node::getNumNodes() { return nodes.size(); }

template<typename T>
T tree<T>::node::getData() { return data; }

template<typename T>
T * tree<T>::node::getDataPtr()
{
    return &data;
}

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
typename tree<T>::node* tree<T>::addNode(T nodeData)
{
    if (root == nullptr)
    {
        // create root node
        root = new node(nullptr, nodeData);
        return root;
    }
    else
    {
        // add node to root node
        return root->addNode(nodeData);
    }
}

template<typename T>
typename tree<T>::node * tree<T>::getNode(uint16_t iNode) { return root->getAllNodes().at(iNode); }

template<typename T>
std::vector<class tree<T>::node *> tree<T>::getAllNodes() { return root->getNodes(); }

template<typename T>
uint16_t tree<T>::getNumNodes() { return root->getNumNodes(); }
