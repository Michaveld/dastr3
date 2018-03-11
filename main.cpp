#include <iostream>
#include "exprtree.h"
#include "Automaton.h"



void test(std::set<BitVector> &found, BitVector &initial, std::map<unsigned,int> pres, int bModTwo) {
    int solution = 0;
    for (auto variable : initial) {
        solution += variable.second * pres[variable.first];
    }
    if ((solution % 2 + 2) % 2 == bModTwo) {
        found.insert(initial);
    }

    for (auto variable : initial) {
        if (variable.second == 0) {
            initial.find(variable.first)->second = 1;
            test(found, initial, pres, bModTwo);
            break;
        }
        else {
            initial.find(variable.first)->second = 0;
        }
    }

}

std::set<BitVector> getSolutions(std::map<unsigned,int> pres, int b) {
    BitVector initial;
    for(auto entry : pres) {
        initial[entry.first] = false;
    }
    std::set<BitVector> found;
    test(found, initial, pres, b%2);
    return found;
}

Automaton createAutomaton(ExprTree * exptree){
    Automaton theAuto;

    if(exptree->getRoot()->getData().type == expr::AND) {
        ExprTree *newTreeLeft = new ExprTree;
        newTreeLeft->createFromNode(exptree->getRoot()->getLeft());

        ExprTree *newTreeRight = new ExprTree;
        newTreeRight->createFromNode(exptree->getRoot()->getRight());

        Automaton left = createAutomaton(newTreeLeft);
        Automaton right = createAutomaton(newTreeRight);

        left.print(std::cout);
        right.print(std::cout);

        theAuto.intersect(left, right);
        return theAuto;
    }
    else if (exptree->getRoot()->getData().type == expr::EXISTS) {
        ExprTree *newTree = new ExprTree;
        newTree->createFromNode(exptree->getRoot()->getRight());
        theAuto = createAutomaton(newTree);

        unsigned variable = exptree->getRoot()->getLeft()->getData().variable;
        theAuto.removeVariable(variable);

        return theAuto;
    }
    else if (exptree->getRoot()->getData().type == expr::EQUALS) {
        std::map<unsigned,int> pres;
        int b = 0;
        pres.emplace(9999, 0);

        exptree->getPresburgerMap(pres, b);

        pres.erase(9999);

        int initial = b;

        for (auto key : pres) {
            theAuto.addToAlphabet(key.first);
        }

        std::set<State> done;
        std::queue<State> remaining;
        remaining.push(b);

        while (!remaining.empty()) {
            State current = remaining.front();
            remaining.pop();

            theAuto.addState(current);
            done.insert(current);
            b = current;
            std::set<BitVector> solutions = getSolutions(pres, current);
            for (auto solution : solutions) {
                int newState = b;
                for (auto iets : solution) {
                    newState = newState - pres.find(iets.first)->second * iets.second;
                }

                newState /= 2;

                theAuto.addTransition(current, solution, newState);

                if (done.find(newState) == done.end()) {
                    remaining.push(newState);
                }
            }
        }

        theAuto.markInitial(initial);
        theAuto.markFinal(0);

        // TODO (voor studenten in deel 2): Bouw de Presburger automaat door de meegegeven syntaxtree exptree van de formule te doorlopen
        return theAuto;
    }
}

void addVarToBitVectors(std::list<BitVector> &l, const unsigned index, int val) {
    for(std::list<BitVector>::iterator it = l.begin(); it != l.end(); ++it) {
        bool bit = (val&1);
        (*it)[index] = bit;
        val>>=1;
    }

    // create BitVector in which all variables have a bit value of 0
    BitVector zeroVector;
    if(l.size() > 0)
        zeroVector = *(l.begin());
    for(BitVector::iterator it = zeroVector.begin(); it != zeroVector.end(); ++it) {
        it->second = 0;
    }

    // val requires more bits than l.size(): add new vectors at the end of l
    while(val) {
        BitVector b = zeroVector;
        bool bit = (val&1);
        b[index] = bit;
        l.push_back(b);
        val>>=1;
    }
}

/** Utility function for BitVectors. Prints the content all BitVectors in the list to the given output stream
*/
void printBitVectors(std::ostream &out, std::list<BitVector> l) {
    for(BitVector b: l) {
        out << "[";
        for(std::pair<unsigned, bool> elem : b) {
            out << elem.first << ": " << elem.second << ", ";
        }
        out << "]\n";
    }
}

std::list<BitVector> generateBitVectors(std::map<unsigned,unsigned> valueMap){
    std::list<BitVector> l;
    for(auto &var : valueMap){
        addVarToBitVectors(l, var.first,var.second);
    }
    return l;
}

bool verifyAutomaton(Automaton& theAuto, string formula, bool debug, std::ostream &out){
    std::stringstream ss(formula);
    string data;
    int variable;
    int value;
    std::map<unsigned,unsigned> valueMap;
    while(ss >> data){
        if(data[0] == 'x'){
            string substr = data.substr(1,data.find('=')-1);
            variable = std::atoi(substr.c_str());
            if(variable == 0)
                return false;
            string substr2 = data.substr(data.find('=')+1,string::npos);
            value =  std::atoi(substr2.c_str());
            valueMap[variable] = value;
        }
        else
            return false;
    }
    std::list<BitVector> l = generateBitVectors(valueMap);
    theAuto.parseInput(l);
    if(debug) {
        printBitVectors(out,l);
    }
    return theAuto.inFinalState();
}


void menu(bool debug, std::istream& inStr, std::ostream &out){
    Automaton theAuto;
    ExprTree * t = new ExprTree();
    while(true){
        string in;
        getline(inStr,in, '\n');
        if(in.substr(0,5) == "read "){
            if(t->create(in.substr(4,string::npos))) {
                theAuto = createAutomaton(t);
                if(debug) {
                    theAuto.print(out);
                }
            } else if(!debug) {
                out << "Error: invalid formula" <<std::endl;
            }
        }
        else if(in.substr(0,6) == "check "){
            if(verifyAutomaton(theAuto,in.substr(5,string::npos), debug, out))
                out << "valid" <<std::endl;
            else
                out << "invalid" <<std::endl;
        }
        else if(in.substr(0,3) == "end")
            return;
        else if(!debug)
            out << "Error: invalid input" <<std::endl;
    }
    delete t;
}

int main(int argc, char** argv)
{
    bool debug = false;
    if(argc >= 2 && string(argv[1]).find("d") != string::npos){
        debug = true;
    }
    menu(debug, std::cin, std::cout);
    return 0;
}