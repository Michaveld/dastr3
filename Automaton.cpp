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
        std::map<BitVector, std::set<State> > temp2;
        temp2[label] = temp;
        std::pair<State, std::map<BitVector, std::set<State> > > temp3 = {from, temp2};
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
    currentStates = initialStates;

    for (auto v : input) {
        next(v);
    }
}

bool Automaton::inFinalState() const {
    for (auto state : currentStates) {
        if (finalStates.find(state) != finalStates.end()) {
            return true;
        }
    }

    return false;
}

void Automaton::next(const BitVector input) {
    std::set<State> newStates;
    for (auto state : currentStates) {
        for (auto newState : transitions[state][input]) {
            newStates.insert(newState);
        }
    }

    currentStates = newStates;
}

void Automaton::intersect(Automaton &fa1, Automaton &fa2) {
    for (auto varnr : fa1.alphabet) {
        fa2.addToAlphabet(varnr);
    }

    for (auto varnr : fa2.alphabet) {
        fa1.addToAlphabet(varnr);
    }

    alphabet = fa1.alphabet;

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

        std::map<BitVector, std::set<State>> transitionsFa1 = fa1.transitions.find(current.first)->second;
        std::map<BitVector, std::set<State>> transitionsFa2 = fa2.transitions.find(current.second)->second;

        for (auto transition1 : transitionsFa1) {
            auto transition2 = transitionsFa2.find(transition1.first);
            if (transition2 != transitionsFa2.end()) {
                for (auto state1 : transition1.second) {
                    for (auto state2 : transition2->second) {
                        auto newState = cantorPairingFunction(state1, state2);
                        if (states.find(newState) == states.end()) {
                            remain.push(std::make_pair(state1, state2));
                        }

                        addTransition(state, transition1.first, newState);
                    }
                }
            }
        }
    }
}

int Automaton::cantorPairingFunction(int i, int j) {
    if (i >= 0) {
        i *= 2;
    }
    else {
        i = i * -2 - 1;
    }
    if (j >= 0) {
        j *= 2;
    }
    else {
        j = j * -2 - 1;
    }
    return ((i+j)*(i+j+1)/2 + j);
}

/*void Automaton::addToAlphabet(unsigned varnr) {
    if (alphabet.find(varnr) == alphabet.end()) {
        alphabet.insert(varnr);
        for (auto transitionsPerState : transitions) {
            for (std::pair<BitVector, std::set<State> > transition : transitionsPerState.second) {
                auto copy = transition;
                transitions[transitionsPerState.first]
                transition.first.emplace(varnr, false);
                copy.first.emplace(varnr, true);
                transitionsPerState.second.insert(copy);
            }
        }
    }
}*/

void Automaton::addToAlphabet(unsigned varnr) {
    if (alphabet.find(varnr) == alphabet.end()) {
        alphabet.insert(varnr);

        auto oldTransitions = transitions;
        transitions.clear();

        for (auto trans : oldTransitions) {
            std::map<BitVector, std::set<State> > toAdd;
            transitions.emplace(trans.first, toAdd);

            for (auto bitVector : trans.second) {
                BitVector v = bitVector.first;
                BitVector v2 = v;

                v.emplace(varnr, 0);
                v2.emplace(varnr, 1);

                for (auto toState : bitVector.second) {
                    addTransition(trans.first, v, toState);
                    addTransition(trans.first, v2, toState);
                }
            }
        }
    }
}

void Automaton::removeVariable(unsigned variable) {
    auto oldTransitions = transitions;
    transitions.clear();

    for (auto trans : oldTransitions) {
        std::map<BitVector, std::set<State> > toAdd;
        transitions.emplace(trans.first, toAdd);

        for (auto bitVector : trans.second) {
            BitVector v = bitVector.first;
            v.erase(variable);

            for (auto toState : bitVector.second) {
                addTransition(trans.first, v, toState);
            }
        }
    }
}
