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

void Automaton::addState(const State state) {
    states.insert(state);
}

void Automaton::addTransition(const State from, const BitVector label, const State to) {
    std::map<State, std::map<BitVector, std::set<State> > >::iterator it;
    it = transitions.find(from);
    if (it != transitions.end()) {
        std::map<BitVector, std::set<State> >::iterator it2;
        it2 = it->second.find(label);
        if (it2 != it->second.end()) {
            it2->second.insert(to);
        }
        else {
            std::set<State> temp = {to};
            it->second.insert(std::pair<BitVector, std::set<State> >(label, temp));
        }
    }
    else {
        std::set<State> temp = {to};
        std::pair<BitVector, std::set<State> > temp2 = {label, temp};
        std::pair<State, std::pair<BitVector, std::set<State> > > temp3 = {from, temp2};
        transitions.insert(temp3);
    }
}

void Automaton::markInitial(const State state) {
    initialStates.insert(state);
}

void Automaton::markFinal(const State state) {
    finalStates.insert(state);
}

void Automaton::parseInput(const std::list<BitVector> input) {
    for (auto itInit = initialStates.begin(); itInit != initialStates.end(); itInit++) {
        std::set<State> currentState = {*itInit};
        for (auto itInput = input.begin(); itInput != input.end(); itInput++) {
            currentState = validTransitions(*itInput, currentState);
        }
        currentStates.insert(currentState.begin(), currentState.end());
    }
}

std::set<State> Automaton::validTransitions(const BitVector input, const std::set<State> state) {
    std::set<State> validStates;
    for (auto it = state.begin(); it != state.end(); it++) {
        auto temp = transitions.find(*it);
        if (temp != transitions.end() && temp->second.find(input) != temp->second.end()) {
            validStates.insert(*it);
        }
    }
    return validStates;
}

bool Automaton::inFinalState() const {
    for (auto it = currentStates.begin(); it != currentStates.end(); it++) {
        if (finalStates.find(*it) != finalStates.end()) {
            return true;
        }
    }
    return false;
}

void Automaton::next(const BitVector input) {
    currentStates = validTransitions(input, currentStates);
}

void Automaton::intersect(Automaton &fa1, Automaton &fa2) {
    std::queue<std::pair<State, State> > remain;
    for (State state : fa1.initialStates) {
        for (State state2 : fa2.initialStates) {
            initialStates.insert(cantorPairingFunction(state, state2));
            remain.push(std::pair<State, State>(state, state2));
        }
    }
    while (!remain.empty()) {
        std::pair<State, State> current = remain.front(); // false error by CLion TODO: remove comment
        remain.pop();
        State state = cantorPairingFunction(current.first, current.second);
        states.insert(state);
        if (fa1.finalStates.count(current.first) && fa2.finalStates.count(current.second)) {
            markFinal(state);
        }
        for (auto a : alphabet) {

        }
    }
}

int Automaton::cantorPairingFunction(int i, int j) {
    return ((i+j)*(i+j+1)/2 + j);
}
