/**
* class: Nodes for making a binary tree
* @author Diederick Vermetten
* @file node.h
* @date 20/11/2017
**/
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

template<class T>
class node{
	public:
		/**
		* @function getData
		* @abstract returns the data stored in the node
		* @return The data that is stored in the node
		* @pre the node is not NULL
		**/
		const T getData();
		/**
		* @function setData
		* @abstract Chsnges the data stored in the node
		* @param info : data to be stored
		* @pre the node in not NULL
		* @post the node now contains different data
		**/
		void setData(const T info);
		/**
		* @function getLeft
		* @abstract returns a pointer to the left child of the node
		* @return a pointer to the left child of the node
		* @pre the node is not NULL
		**/
		 node<T>* getLeft();
		/**
		* @function getRight
		* @abstract returns a pointer to the right child of the node
		* @return a pointer to the right child of the node
		* @pre the node is not NULL
		**/
		 node<T>* getRight();
		/**
		* @function setLeft
		* @abstract changes the left child of the node
		* @param a pointer to the new left cild
		* @pre the node is not NULL
		* @post the node has a new left child
		**/
		void setLeft( node<T>* toLeft);
		/**
		* @function setRight
		* @abstract changes the right child of the node
		* @param a pointer to the new right cild
		* @pre the node is not NULL
		* @post the node has a new right child
		**/
		void setRight( node<T>* toRight);
		/**
		* @function printData
		* @abstract outputs the data saved in the node
		* @pre the node is not NULL
		**/
		void printData() const;
		/**
		* @function node
		* @abstract creates a new node
		* @post a new node is created
		**/
		node();

		~node();

	private: //TODO: private or protected?
		node<T>* leftChild;
		node<T>* rightChild;
		T data;
};

template<class T>
node<T>::node(){
	leftChild=nullptr;
	rightChild=nullptr;
}

template<class T>
node<T>::~node(){
    if(leftChild != nullptr) delete leftChild;
    if(rightChild != nullptr) delete rightChild;
}

template<class T>
void node<T>::setData(const T info){
	data=info;
}

template<class T>
 node<T>* node<T>::getLeft(){
	return leftChild;
}

template<class T>
 node<T>* node<T>::getRight(){
	return rightChild;
}

template<class T>
void node<T>::setLeft( node<T>* toLeft){
	leftChild = toLeft;
}

template<class T>
void node<T>::setRight( node<T>* toRight){
	rightChild = toRight;
}

template<class T>
void node<T>::printData() const{
 	cout<<data;
 }

template<class T>
const T node<T>::getData(){
 	return data;
}
 #endif
