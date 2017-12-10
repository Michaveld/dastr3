#include "Automaton.h"



// TODO (voor studenten - deel 1): VOEG HIER DE IMPLEMENTATIES VAN DE OPERATIES IN Automato.h TOE




void Automaton::printStates(std::ostream &str, const std::set<State> s) {
    str << "{";
    for(auto& st : s) {
        str << st << ", ";
    }
    str << "}";
    
}

void Automaton::printTransitionLabel(std::ostream &str, const BitVector t) {
    str << "{";
    for(auto& bv : t) {
        str << bv.first << "->" << bv.second << ", ";
    }
    str << "}";
}

void Automaton::print(std::ostream &str) const {
    str << "Initial States: ";
    printStates(str, initialStates);
    str << "Final States: ";
    printStates(str, finalStates);
    str << "Current States: ";
    printStates(str, currentStates);
    str <<"\nTransitions: \n";
    
    for (auto& trans : transitions) {
        for(auto& m : trans.second) {
            str << "(";
            
            // print label of source state
            str << trans.first;
            str << ", ";
            
            // print label of transition
            printTransitionLabel(str, m.first);
            str << ", ";
            
            // print target states
            printStates(str, m.second);
            str << ")\n";
        }
    }
}


