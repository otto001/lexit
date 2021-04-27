//
// Created by andreas on 08.04.21.
//

#ifndef LEXIT_NFA_H
#define LEXIT_NFA_H


#include "NfaNode.h"
#include <vector>
#include <string>
#include <unordered_set>
#include "../types.h"


class Nfa {
public:
    Nfa() = default;

    Nfa(const std::string &regex);
    ~Nfa();

protected:
    static std::unordered_set<char> escapableCharacter;
    static std::pair<NfaNode*, NfaNode*> buildEpsNfa(const std::string &regex);

    NfaNode *head{};
    NfaNode *finalState{};

    void eliminateEpsilonTransitions();

    std::set<NfaNode *> allReachable();


public:
    bool run(const std::string &runString);
};


#endif //LEXIT_NFA_H
