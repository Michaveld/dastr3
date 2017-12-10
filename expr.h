/**
 * class: expressions for finite automata
 * @author Diederick Vermetten
 * @file expr.h
 * @date 20/11/2017
 **/
#ifndef expr_H
#define expr_H

#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

struct expr {
    enum { EXISTS, PLUS, NOT, AND, OR, EQUALS, VARIABLE, MINUS, TIMES, NUMBER } type;
    union {
        unsigned variable;
        int number;
    };
};

/**
 * @function isBinaryOperator
 * @abstract determines whether an expression is a binary operator or not
 * @param theExpr: the expression to check
 * @return whether or not the expression is a binary operator
 **/
bool isBinaryOperator(expr theExpr){
    switch(theExpr.type){
        case expr::NOT:
            return false;
        case expr::VARIABLE:
            return false;
        case expr::NUMBER:
            return false;
        default:
            return true;
    }
    return true;
}

/**
 * @function isUnaryOerator
 * @abstract determines whether an expression is a binary operator or not
 * @param theExpr: the expression to check
 * @return whether or not the expression is a binary operator
 **/
bool isUnaryOperator(expr theExpr){
    switch(theExpr.type){
        case expr::NOT:
            return true;
        default:
            return false;
    }
    return false;
}


/**
 * @function std::ostream &operator<<
 * @abstract detailes how an expression should be printed
 * @param s: the output
 *                      theExpr: the expression to output
 * @return the output
 **/
std::ostream &operator<<(std::ostream &s, const expr &theExpr){
    switch(theExpr.type){
        case expr::PLUS:
            s << "+";
            break;
        case expr::EQUALS:
            s << "=";
            break;
        case expr::TIMES:
            s << "*";
            break;
        case expr::MINUS:
            s << "-";
            break;
        case expr::NUMBER:
            s << theExpr.number;
            break;
        case expr::VARIABLE:
            s << "x"  << theExpr.variable;
            break;
        case expr::NOT:
            s << "!";
            break;
        case expr::AND:
            s << "&";
            break;
        case expr::OR:
            s << "|";
            break;
        case expr::EXISTS:
            s << "E";
            break;
    } // switch
    return s;
} // <<

#endif // ifndef expr_H
