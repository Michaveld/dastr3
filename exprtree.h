/**
* class: expression tree for finite automata
* @author Diederick Vermetten
* @file exptree.h
* @date 20/11/2017
**/
#ifndef EXPRTREE_H
#define EXPRTREE_H

#include "expr.h"
#include "tree.h"
#include <map>
#include <set>
#include <list>
#include <queue>
#include <vector>
#include <cmath>
#include <stack>
#include <string>
#include <sstream>



class ExprTree : public binarytree<expr>{
	public:



		/**
		* @function create
		* @abstract reads the expression and saves it as an expressiontree
		* @param: expr: the expression to turn into a tree
		* @param: debug: display output for user or not
		* @pre the typed expression has to be complete
		* @post the expression is saved as a tree
		**/
		bool create(string expr);


		/**
		* @function createFromNode
		* @abstract used to turn a subtree into a real ExpTree
		* @param: root: root of the subtree
		* @pre none
		* @post the tree is now equal to the subtree starting at root
		**/
		void createFromNode(node<expr>* const root);


		/**
		* @function getPresburgerMap
		* @abstract maps the variables to their coefficient
		* @param: pres: empty map, will become the coefficient map
		* @param: b: 0, will become the constant coefficient
		* @pre the root of the tree is '=' (use createFromNode)
		* @return: the tree is a legal presburger formula
		**/
		bool getPresburgerMap(std::map<unsigned,int>& pres, int& b);

	private:
		/**
		* @function isComplete
		* @abstract checks if the tree is complelely filled
		* @return true if the tree is complelely filled
		**/
		bool isComplete();
		/**
		* @function createNode
		* @abstract Creates a node with the correct type of expression-token
		* @param data: the expression-token to be stored in the node
		* @return a node with the expression-token as its data
		**/
		node<expr>* createNode(const string data);
		/**
		* @function insert
		* @abstract insert the node into the tree
		* @post the node is saved in the tree
		**/
		bool insert(node<expr>* theNode);

		/**
		* @function recursiveInsert
		* @abstract recursive part of insertnode
		* @post theNode is saved in the tree as the child of startpoint
		**/
		bool recursiveInsert(node<expr>* theNode, node<expr>* startpoint);

		/** @function getPresburgerMapRecursive
		* @abstract recursive part of getPresburgerMap
		* @param: pres: map so far
		* @param: b: constant coefficient
		* @param: root: node from where to continue calculating
		* @param: mult: multiplier for the coefficients in this subtree
		* @return: this is a legal subtree of a presburget formula
		**/
		bool getPresburgerMapRecursive(node<expr>* root, int mult, std::map<unsigned,int>& pres,int& b);

		

		/** @function copy
		* @abstract copy a subtree to the root of this tree
		* @param: extra: root of the subtree to copy
		* @post: this tree now is a copy of the tree starting at extra
		**/
		node<expr>* copy(node<expr>* extra);
};


bool ExprTree::getPresburgerMap(std::map<unsigned,int>& pres, int& b){
	if(getRoot()->getData().type == expr::EQUALS)
		return getPresburgerMapRecursive(getRoot(), 1, pres,b);
	return false;
}

bool ExprTree::getPresburgerMapRecursive(node<expr>* root, int mult, std::map<unsigned,int>& pres, int& b){
	bool leftOK = true;
	bool rightOK = true;
	switch(root->getData().type){
		case expr::EQUALS:
			leftOK = getPresburgerMapRecursive(root->getLeft(), mult, pres,b);
			rightOK = getPresburgerMapRecursive(root->getRight(), mult*(-1), pres,b);
			break;
		case expr::NUMBER:
			b -= mult * root->getData().number;
			break;
		case expr::VARIABLE:
			pres[root->getData().variable] += mult;
			break;
		case expr::MINUS:
			leftOK = getPresburgerMapRecursive(root->getLeft(), mult, pres,b);
			rightOK = getPresburgerMapRecursive(root->getRight(), mult*(-1), pres,b);
			break;
		case expr::PLUS:
			leftOK = getPresburgerMapRecursive(root->getLeft(), mult, pres,b);
			rightOK = getPresburgerMapRecursive(root->getRight(), mult, pres,b);
			break;
		case expr::TIMES:
			if(root->getLeft()->getData().type == expr::NUMBER){
				leftOK = getPresburgerMapRecursive(root->getRight(), mult*root->getLeft()->getData().number, pres,b);
			}
			else if(root->getRight()->getData().type == expr::NUMBER){
				rightOK = getPresburgerMapRecursive(root->getLeft(), mult*root->getRight()->getData().number, pres,b);
			}
			else
				return false;
			break;
		default:
			return false;
	}
	return (leftOK && rightOK);
}

void ExprTree::createFromNode(node<expr>* const root){
	setRoot(copy(root));
}

bool ExprTree::isComplete(){
	return insert(NULL);
}

bool ExprTree::insert(node<expr>* theNode){
	if(getRoot() == NULL){
		setRoot(theNode);
		return true;
	}
	return recursiveInsert(theNode,getRoot());
}

bool ExprTree::recursiveInsert(node<expr>* theNode, node<expr>* startpoint){
	if(isUnaryOperator(startpoint->getData())){
		if(startpoint->getLeft() == NULL){
			startpoint->setLeft(theNode);
			return true;
		}
		return recursiveInsert(theNode, startpoint->getLeft());
	}
	else if(isBinaryOperator(startpoint->getData())){
		if(startpoint->getLeft() == NULL){
			startpoint->setLeft(theNode);
			return true;
		}
		if(recursiveInsert(theNode, startpoint->getLeft())){
			return true;
		}
		if(startpoint->getRight() == NULL){
			startpoint->setRight(theNode);
			return true;
		}
		return recursiveInsert(theNode, startpoint->getRight());
	}
	return false;
}

bool ExprTree::create(string expr){
	auto oldRoot = getRoot();
	setRoot(NULL);
	std::stringstream ss(expr);
	string data;

	while(ss >> data){
		auto newNode = createNode(data);
		if(newNode == NULL || !insert(newNode)){
			setRoot(oldRoot);
			return false;
		}
	}
	if(insert(NULL)){
		setRoot(oldRoot);
		return false;
	}
	return !isComplete();
}




node<expr>* ExprTree::createNode(const string data){
	node<expr>* assist = new node<expr>;
	expr extra;
	if(data == "*")
		extra.type = expr::TIMES;
	else if(data == "+")
		extra.type = expr::PLUS;
	else if(data == "-")
		extra.type = expr::MINUS;
	else if(data == "|")
		extra.type = expr::OR;
	else if(data == "&")
		extra.type = expr::AND;
	else if(data == "!")
		extra.type = expr::NOT;
	else if(data == "E")
		extra.type = expr::EXISTS;
	else if(data == "="){
		extra.type=expr::EQUALS;
	}
	else if(data[0] == 'x'){
		extra.type = expr::VARIABLE;
		string substr = data.substr(1);
		int nr = std::atoi(substr.c_str());
		if(nr == 0)
			return NULL;
		extra.variable = nr;
	}
	else {
		extra.type = expr::NUMBER;
		extra.number = std::atoi(data.c_str());
	}
	assist->setData(extra);
	return assist;
}

node<expr>* ExprTree::copy(node<expr>* extra){
	if(extra == NULL)
		return NULL;
	node<expr>* assist= new node<expr>;
	expr test=extra->getData();
	assist->setData(test);
	assist->setLeft(copy(extra->getLeft()));
	assist->setRight(copy(extra->getRight()));
	return assist;
}



#endif
