#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <iostream>

typedef std::map<unsigned, bool> BitVector;
typedef int State;


class Automaton {
public:
    /**
             * Add the given state (an integer) to the automaton
             */
    void addState(const State state);

    /**
             * Add transition with the given label (a bitvector) between State from and State to.
             * If this is the first transition added to the automaton:
             * initialize the field alphabet with all variables
     * in BitVector label (i.e., the domain of the BitVector map)
             */
    void addTransition(const State from, const BitVector label, const State to);

    /**
             * Mark the given state as an initial state
             */
    void markInitial(const State state);

    /**
             * Mark the given state as a final state
             */
    void markFinal(const State state);

    /**
             * Read the list of bitvectors and move to the next states according to the transitions,
             * starting from the initial states
             */
    void parseInput(const std::list<BitVector> input);

    /**
             * Return whether the current states contain a final state
             */
    bool inFinalState() const;

    /**
             * Compute intersection of automata fa1 and fa2 and store the result in the current automaton
             * (implementation calculates cartesian product of automata)
             */
    void intersect(Automaton& fa1, Automaton& fa2);

    /**
     * Replace every bitvector in the transitions with two new bitvectors that additionally contain the new variable varnr:
     * 1. a transition where variable varnr has value 0
     * 2. a transition where variable varnr has value 1
     * The values for the other variables are unchanged from the original bitvector.
     * Finally, add the given variable to the alphabet.
* If the variable was already part of the alphabet: do nothing.
     */
    void addToAlphabet(unsigned varnr);

    /**
             * OPTIONEEL (voor bonus)!!!
             * Compute the complement of the current automaton
             * (for non-deterministic automata: uses makeDeterministic() )
             */
    void complement(Automaton& fa);

    /**
             * OPTIONEEL (voor bonus)!!!
             * Remove the variable with the given index from all transitions of this automaton
             */
    void project(const unsigned variable);

    /**
             * OPTIONEEL (voor bonus)!!!
             * Make automata fa deterministic, eliminate epsilon transitions, and store the result in the current automaton
             */
    void makeDeterministic(Automaton& fa);

    /**
     * Prints the current automaton to the specified stream
     */
    void print(std::ostream &str) const;

    void removeVariable(unsigned variable);

private:
    /**
             * Read a single BitVector as input and move from the set of current states to the set of next states
             */
    void next(const BitVector input);

    /**
             * OPTIONEEL (voor bonus)!!!
             * Eliminate epsilon transitions, and store the result in the current automaton
             */
    void eliminateLambda(Automaton& fa);

    /**
     * Prints the set of states s to the specified stream
     */
    static void printStates(std::ostream &str, const std::set<State> s);

    /**
     * Prints the BitVector t to the specified stream
     */
    static void printTransitionLabel(std::ostream &str, const BitVector t);


    int cantorPairingFunction(int i, int j);

        // the set of all states
    std::set<State> states;

    // the set of initial states
    std::set<State> initialStates;

    // the set of final states
    std::set<State> finalStates;

    // the map containing all transitions of the automton
    std::map<State, std::map<BitVector, std::set<State> > > transitions;

    // the set of current states
    std::set<State> currentStates;

    // the set of variables used in transitions
    std::set<unsigned> alphabet;

};

#endif