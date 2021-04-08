//
// Created by andreas on 08.04.21.
//

#include "Nfa.h"
#include <algorithm>


Nfa::Nfa(std::string regex) {
    head = new NfaNode();
    auto last = head;
    for (auto c : regex) {
        auto newNode = new NfaNode();
        last->insertTransition(c, newNode);
        last = newNode;

    }
    last->setFinal(true);
}

bool Nfa::run(std::string runString) {
    std::set<NfaNode *> current = { head };
    for(auto c : runString) {
        std::set<NfaNode *> next = {};
        for(auto cur : current) {
            for (auto node : cur->next(c)) {
                next.insert(node);
            }
        }
        current = next;
        if(current.empty()) {
            return false;
        }
    }
    for (auto c : current){
        if(c->isFinal()){
            return true;
        }
    }
    return false;
}
