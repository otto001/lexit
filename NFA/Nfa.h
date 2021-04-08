//
// Created by andreas on 08.04.21.
//

#ifndef LEXIT_NFA_H
#define LEXIT_NFA_H


#include "NfaNode.h"
#include <vector>
#include <string>
#include "../types.h"


class Nfa {
public:
    Nfa() = default;
    Nfa(std::string regex);
protected:
    NfaNode *head{};


public:
    bool run(std::string runString);
};


#endif //LEXIT_NFA_H
