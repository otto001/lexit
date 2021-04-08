//
// Created by andreas on 08.04.21.
//

#ifndef LEXIT_NFANODE_H
#define LEXIT_NFANODE_H

#include <unordered_map>
#include <set>
#include <vector>
#include "../types.h"

class NfaNode {
protected:
    std::unordered_map<Symbol, std::set<NfaNode *>> transitions;
    bool final;
public:
    void insertTransition(Symbol, NfaNode *);
    const std::set<NfaNode *> & next(Symbol symbol);
    bool isFinal() const { return final; };
    void setFinal(bool final) {this->final = final;}
};


#endif //LEXIT_NFANODE_H
