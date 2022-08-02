/*
Description: .h file that declares and defines the templated BST and all of its methods.
             It also contains the WordEntry class to hold the words and their frequencies
*/

// standard safety for .h files
#pragma once
#ifndef BST_H
#define BST_H
#include <string>

// declare the templated BST class
template <class type>
class BST
{
    // create a node struct to hold the data
    struct Node
    {
        // node contains the data and pointers to left and right nodes
        type data;
        struct Node* leftNode;
        struct Node* rightNode;
    };

    // declare private variables and helper methods
    private:
        struct Node* root;
        int numNodes;
        int toArrayIndex;

        void insertHelper(struct Node* parentNode, struct Node* node, type data);
        void removeHelper(struct Node* node, type data);
        void deleteNode(struct Node* node);
        void toArrayHelper(struct Node* node, type* array);
        void emptyHelper(struct Node* node);

    // declare constructor, destructor, and public methods
    public:
        BST();
        ~BST();
        void insert(type data);
        void remove(type data);
        bool elementExists(type data);
        struct Node* find(type data);
        type* toArray();
        void empty();
        int count();

};

// declare WordEntry class to represent each word
class WordEntry
{
    // declare public variables and methods
    public:
        std::string word;
        int frequency;
        bool operator<(WordEntry object);
        bool operator>(WordEntry object);
        void operator+=(int num);
};

// define BST constructor
template <class type>
BST<type>::BST()
{
    // initialize some variables
    this->root = NULL;
    this->numNodes = 0;
    this->toArrayIndex = 0;
}

// define BST destructor
template <class type>
BST<type>::~BST()
{
    // delete all nodes of the tree
    empty();
}

// define the insert method
template <class type>
void BST<type>::insert(type data)
{
    // pass in the root and parentNode to the helper method
    struct Node* node = this->root;
    struct Node* parentNode = this->root;
    insertHelper(parentNode, node, data);
}

// helper method that does the actual inserting.
// this is needed because we need to keep track of the root and parentNode while inserting
template <class type>
void BST<type>::insertHelper(struct Node* parentNode, struct Node* node, type data)
{
    // if node is null
    if (node == NULL)
    {
        // create new Node object and fill in the data
        node = new struct Node;
        node->data = data;
        node->leftNode = NULL;
        node->rightNode = NULL;
        this->numNodes += 1;
        // this root is also null, this means this is the first insert, so make root equal to this
        if (this->root == NULL)
        {
            this->root = node;
            return;
        }
        // connect parent node to this node
        if (parentNode->data < node->data)
            parentNode->rightNode = node;
        else if (parentNode->data > node->data)
            parentNode->leftNode = node;
        return;
    }

    // recursively call on right side based on condition
    else if (data > node->data)
        insertHelper(node, node->rightNode, data);
    // recursively call on left side based on condition
    else if (data < node->data)
        insertHelper(node, node->leftNode, data);
    // if nodes are the same, increase data by 1 and return
    else
    {
        node->data += 1;
        return;
    }
}

// define BST remove method
template <class type>
void BST<type>::remove(type data)
{
    // pass in the root to the helper method
    struct Node* node = this->root;
    removeHelper(node, data);
}

// helper method that goes to the node that needs to be deleted
// this is needed because we need to keep track of the start node
template <class type>
void BST<type>::removeHelper(struct Node* node, type data)
{
    // recursively call on left side based on condition
    if (data < node->data)
        removeHelper(node->leftNode, data);
    // recursively call on right side based on condition
    else if (data > node->data)
        removeHelper(node->rightNode, data);
    // if node that needs to be deleted is reached, call another method that does the deleting
    else
        return deleteNode(node);
}

// helper method that does the actual deleting of the node
template <class type>
void BST<type>::deleteNode(struct Node* node)
{
    // create temp node
    struct Node* tempNode;
    if (node == NULL)
        return;
    // if this node only has right child
    else if (node->leftNode == NULL)
    {
        tempNode = node;
        node = node->rightNode;
    }
    // if this node only has left child
    else if (node->rightNode == NULL)
    {
        tempNode = node;
        node = node->leftNode;
    }
    // if left and right children exist, it becomes a little bit hard to delete
    // but we have to accurately attach the right nodes to the right parents
    else
    {
        tempNode = node->rightNode;
        while (tempNode->leftNode)
            tempNode = tempNode->leftNode;
        tempNode->leftNode = node->leftNode;
        tempNode = node;
        node = node->rightNode;
    }
    // delete the temp node
    delete tempNode;
}

// define method to find a node and return its pointer
template<class type> 
struct BST<type>::Node* BST<type>::find(type data)
{
    // get root node reference
    struct Node* node = this->root;
    while (node != NULL)
    {
        // until desired node is reached, keep going deeper into the tree
        if (node->data == data)
            return node;
        else if (node->data > data)
            node = node->leftNode;
        else if (node->data < data)
            node = node->rightNode;
    }
    // if node not found, return null
    return NULL;
}

// define method that tells whether a node already exists in the tree
template <class type>
bool BST<type>::elementExists(type data)
{
    // instead of doing double work, simply use the method defined above
    if (find(data) != NULL)
        return true;
    return false;
}

// define method to convert inorder traversal of tree into an array
template <class type>
type* BST<type>::toArray()
{
    // create an array on the heap, and pass it to the helper method
    type* array = new type[this->numNodes];
    struct Node* node = this->root;
    toArrayHelper(node, array);
    toArrayIndex = 0;
    // return array
    return array;
}

// helper method that does the inorder traversal
template <class type>
void BST<type>::toArrayHelper(struct Node* node, type* array)
{
    // until null node is reached, keep recursively calling the method on left and right sides
    // continually update the array and the array index
    if (node != NULL)
    {
        toArrayHelper(node->leftNode, array);
        array[toArrayIndex] = node->data;
        toArrayIndex++;
        toArrayHelper(node->rightNode, array);
    }
}

// define method to delete all nodes in the tree
template <class type>
void BST<type>::empty()
{
    // pass in the root to the helper method
    struct Node* node = this->root;
    return emptyHelper(node);
}

// helper method to delete the entire tree
template <class type>
void BST<type>::emptyHelper(struct Node* node)
{
    // until all nodes are null, keep recursively calling the left and right nodes and delete them
    if (node != NULL)
    {
        emptyHelper(node->leftNode);
        emptyHelper(node->rightNode);
        delete node;
    }
    return;
}

// return the number of nodes in the tree
template <class type>
int BST<type>::count()
{
    return this->numNodes;
}

// override the < operator for a WordEntry object
bool WordEntry::operator<(WordEntry object)
{
    return this->word < object.word;
}

// override the > operator for a WordEntry object
bool WordEntry::operator>(WordEntry object)
{
    return this->word > object.word;
}

// override the += operator for a WordEntry object
void WordEntry::operator+=(int num)
{
    this->frequency += num;
}

#endif