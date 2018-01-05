/**
* class: A basic version of a binary tree
* @author Diederick Vermetten
* @file tree.h
* @date 20/11/2017
**/
#ifndef TREE_H
#define TREE_H
#include "node.h"
#include "expr.h"

template<class T>
class binarytree{
public:
    // *
    // * @function insert
    // * @abstract adds a new node to the tree
    // * @param info: the data to be put in the new node
    // * @post the tree now has an extra node
    // *
    // void insert(const T info);
    /**
    * @function contains
    * @abstract searches the tree to find wether a node containing some specific
        data in present
    * @param info: the data we are looking for
    * @return true if the data is present in the tree, false otherwise
    **/
    bool contains(const T info) const;
    /**
    * @function showPreOrder
    * @abstract outputs the data of the tree in preorder (root, left, right)
    * @pre the tree is not empty
    **/
    void showPreOrder() const;
    /**
    * @function showInOrder
    * @abstract outputs the data of the tree in inorder (left, root, right)
    * @pre the tree is not empty
    **/
    void showInOrder() const;
    /**
    * @function showPostOrder
    * @abstract outputs the data of the tree in postorder (left, right, root)
    * @pre the tree is not empty
    **/
    void showPostOrder() const;
    /**
    * @function saveAsDot
    * @abstract saves the tree to a file (in DOT-notation)
    * @return a file containing the DOT-notation of the tree
    * @pre the tree is not empty
    **/
    void saveAsDot(const string filename) const;
    /**
    * @function isEmpty
    * @abstract checks whether or not the tree is empty
    * @return true if the tree is empty, false otherwise
    **/
    bool isEmpty() const;
    ~binarytree();
    binarytree();
    /**
    * @function getRoot
    * @abstract returns the root of the tree
    * @return the root of the tree
    **/
    node<T>* getRoot() const;
    /**
    * @function setRoot
    * @abstract changes the root of the tree
    * @param extra: the new root
    * @post the tree now has a different root
    **/
    void setRoot(node<T>* extra);


protected:
    node<T>* root;

    /**
    * @function deletetree
    * @abstract deletes the (sub)tree starting at extra
    * @param extra: the node from where we start deleting the (sub)tree
    * @post the (sub)tree has been deleted
    **/
    void deletetree(node<T>* extra);
};

template<class T>
node<T>* binarytree<T>::getRoot() const{
    return root;
}

template<class T>
void binarytree<T>::setRoot(node<T>* extra){
    root=extra;
}


template<class T>
binarytree<T>::binarytree(){
    root=NULL;
}

template<class T>
binarytree<T>::~binarytree(){
    if(root != NULL){
        delete root;
        root = NULL;
    }
}

template<class T>
void binarytree<T>::deletetree(node<T>* extra){
    if(extra == NULL){
        return;
    }
    deletetree(extra->getRight());
    deletetree(extra->getLeft());
    delete extra;
    extra = NULL;
    return;
}

template<class T>
bool binarytree<T>::isEmpty() const{
    return(root==NULL);
}

// template<class T>
// void binarytree<T>::insert(T info){
// 	node<T>* extra = new node<T>;
// 	extra->setData(info);
// 	extra->setLeft(root);
// 	root=extra;
// }

template<class T>
bool binarytree<T>::contains(T info) const{
    node<T>* extra=root;
    while(extra!=NULL){
        if(extra->getData()==info)
            return true;
        extra=extra->getLeft();
    }
    return false;
}

template<class T>
void binarytree<T>::showPreOrder() const{
    if(root!=NULL)
        preOrder(root);
}

template<class T>
void preOrder(node<T>* root){
    root->printData();
    if(root->getLeft()!=NULL)
        preOrder(root->getLeft());
    if(root->getRight()!=NULL)
        preOrder(root->getRight());
    return;
}

template<class T>
void binarytree<T>::showInOrder() const{
    if(root!=NULL)
        inOrder(root);
    cout <<endl;
}

template<class T>
void inOrder(node<T>* root){
    if(root->getData().type == expr::EXISTS){
        root->printData();
        cout << " ";
        root->getLeft()->printData();
        cout << ": ";
        inOrder(root->getRight());
    }
    else if(isBinaryOperator(root->getData())){
        if(root->getData().type == expr::TIMES){
            inOrder(root->getLeft());
            root->printData();
            inOrder(root->getRight());
        }
        else{
            cout << "(";
            inOrder(root->getLeft());
            root->printData();
            inOrder(root->getRight());
            cout << ")";
            return;
        }
    }
    else
        root->printData();
}

template<class T>
void binarytree<T>::showPostOrder() const{
    if(root!=NULL)
        postOrder(root);
}

template<class T>
void postOrder(node<T>* root){
    if(root->getLeft()!=NULL)
        postOrder(root->getLeft());
    if(root->getRight()!=NULL)
        postOrder(root->getRight());
    root->printData();
    return;
}

template<class T>
void writeDot(node<T>* root, int i, ofstream & output){
    if(root->getLeft()!= NULL){
        output << 2*i << "[label=\"" << (root->getLeft())->getData() << "\"]"<<endl;
        output << i << " -> " << 2*i << endl;
        writeDot(root->getLeft(), 2*i,output);
    }
    if(root->getRight()!= NULL){
        output << 2*i+1 << "[label=\"" << (root->getRight())->getData() << "\"]" <<endl;
        output << i << " -> " << 2*i+1 << endl;
        writeDot(root->getRight(), 2*i+1,output);
    }
}

template<class T>
void binarytree<T>::saveAsDot(string filename) const{
    node<T>* extra = root;
    ofstream output;
    output.open(filename.c_str());
    output<<"digraph g { ";
    output.put('\n');
    int i=1;
    if(extra != NULL){
        output << i << "[label=\"" << extra->getData() << "\"]" <<endl;
        writeDot(extra, i, output);
    }
    output<< "}";
    output.close();
}
#endif