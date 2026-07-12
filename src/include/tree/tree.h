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
        node(node* parent);
        node(node* parent, T data);
        node(node* parent, std::vector<node*> children);
        ~node();
        T getData();
        T* getDataPtr();
        node* addNode(T dataChild);
        node* getNode(uint16_t iNode);
        std::vector<node*> getAllNodes();
        uint16_t getNumNodes();
        bool isLeaf();
    private:
        T data;
        std::vector<node*> nodes;
        node* parent;
        bool leaf = true; // i.e. no nodes
    };

    tree();
    ~tree();
    node* addNode(T node);
    node* getNode(uint16_t iNode);
    std::vector<node*> getAllNodes();
    uint16_t getNumNodes();
    using nptr = tree<T>::node*;
private:
    node* root = nullptr;
};

template<typename T>
using nptr = typename tree<T>::nptr;
// include impl (weird C++ style)
#include "tree.tpp"
