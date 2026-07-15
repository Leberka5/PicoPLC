#pragma once

#include <vector>
#include <cstdint>
#include <optional>

template<typename T>
class tree
{
public:
    class node
    {
    public:
        node(node* parent, T data);
        ~node();
        node* addNode(T dataChild);
        T getData();
        T* getDataPtr();
        uint16_t getNumChildren();
        std::vector<node*> getChildren();
        node* getChild(uint16_t iNode);
        node* getParent();
        bool isLeaf();
    private:
        T data;
        std::vector<node*> children;
        node* parent;
        bool leaf = true; // i.e. no nodes
    };

    tree();
    ~tree();
    node* setRoot(T node);
    node* getRoot();
    std::vector<node*> getRootNodes();
    using nptr = tree<T>::node*;
private:
    node* root = nullptr;
};

template<typename T>
using nptr = typename tree<T>::nptr;

// include impl (weird C++ style)
#include "tree.tpp"
