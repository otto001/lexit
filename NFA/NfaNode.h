//
// Created by andreas on 08.04.21.
//

#ifndef LEXIT_NFANODE_H
#define LEXIT_NFANODE_H

#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <memory>
#include "../types.h"

class NfaNode {
protected:
    std::unordered_map<Symbol, std::set<NfaNode *>> transitions;
    std::set<NfaNode *> epsilonTransitions;
    std::set<NfaNode *> wildCardTransitions;
    std::set<NfaNode *> wordCharacterTransitions;
    std::set<NfaNode *> nonWordCharacterTransitions;
    std::set<NfaNode *> whitespaceCharacterTransitions;
    std::set<NfaNode *> nonWhitespaceCharacterTransitions;
    std::set<NfaNode *> digitCharacterTransitions;
    std::set<NfaNode *> nonDigitCharacterTransitions;

    bool final;

    bool hasSpecialTransitions;
    bool epsilonInsertionLock;
public:
    void insertTransition(Symbol, bool, NfaNode *);
    void insertTransitions(NfaNode *);

    void insertEpsilonTransition(NfaNode *);
    void removeEpsilonTransition(NfaNode *);

    void optimize();

    std::set<NfaNode *> next(Symbol symbol);

    bool isFinal() const { return final; };
    void setFinal(bool final) {this->final = final;}

    std::set<NfaNode *> getDirectlyReachable();
    const std::set<NfaNode *> &getDirectlyEpsilonReachable() const;
};


#endif //LEXIT_NFANODE_H
