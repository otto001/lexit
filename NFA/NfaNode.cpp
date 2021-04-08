//
// Created by andreas on 08.04.21.
//

#include "NfaNode.h"

void NfaNode::insertTransition(Symbol symbol, NfaNode *nfaNode) {
    auto it = transitions.find(symbol);
    if(it == transitions.end()) {
        transitions[symbol] = {nfaNode};
    } else {
        it->second.insert(nfaNode);
    }
}

const std::set<NfaNode *> & NfaNode::next(Symbol symbol) {
    static const std::set<NfaNode *> emptySet = {};
    auto it = transitions.find(symbol);
    if(it == transitions.end()){
        return emptySet;
    }
    return it->second;
}