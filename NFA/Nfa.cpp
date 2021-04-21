//
// Created by andreas on 08.04.21.
//

#include "Nfa.h"
#include <algorithm>


Nfa::Nfa(std::string regex) {
    head = new NfaNode();
    auto last = head;
    NfaNode *beforeLast = nullptr;
    for (auto c : regex) {
        switch (c) {
            case '|':
                last->setFinal(true);
                last = head;
                beforeLast = nullptr;
                break;
            case '*':
                // TODO: error handling for cases like *a
                last->insertEpsilonTransition(beforeLast);
                last = beforeLast;
                beforeLast = nullptr;
                break;
            case '+':
                last->insertEpsilonTransition(beforeLast);
                break;
            case '?':
                beforeLast->insertEpsilonTransition(last);
                break;
            case '\\':
                break;
            default:
                auto epsilonNode = new NfaNode();
                last->insertEpsilonTransition(epsilonNode);
                auto newNode = new NfaNode();
                epsilonNode->insertTransition(c, false, newNode);
                beforeLast = last;
                last = newNode;
                break;
        }

    }
    last->setFinal(true);
    eliminateEpsilonTransitions();
}

bool Nfa::run(std::string runString) {
    std::set<NfaNode *> current = {head};
    for (auto c : runString) {
        std::set<NfaNode *> next = {};
        for (auto cur : current) {
            for (auto node : cur->next(c)) {
                next.insert(node);
            }
        }
        current = next;
        if (current.empty()) {
            return false;
        }
    }
    for (auto c : current) {
        if (c->isFinal()) {
            return true;
        }
    }
    return false;
}

void Nfa::eliminateEpsilonTransitions() {
    auto allNodes = allReachable();
    auto unprocessedNodes = allNodes;
    while (!unprocessedNodes.empty()) {
        auto node = *unprocessedNodes.begin();
        std::set<NfaNode *> epsNext;

        while (!(epsNext = node->getDirectlyEpsilonReachable()).empty()) {
            auto nextNode = *epsNext.begin();
            if (nextNode != node) {
                node->insertTransitions(nextNode);

                if (nextNode->isFinal()) {
                    node->setFinal(true);
                }
            }
            node->removeEpsilonTransition(nextNode);
        }
        unprocessedNodes.erase(node);
    }
}

std::set<NfaNode *> Nfa::allReachable() {
    std::set<NfaNode *> pendingNodes = {head};
    std::set<NfaNode *> reachableNodes;
    while (!pendingNodes.empty()) {
        auto node = *pendingNodes.begin();
        auto reachable = node->getDirectlyReachable();
        reachableNodes.insert(node);

        std::set_difference(reachable.begin(), reachable.end(), reachableNodes.begin(), reachableNodes.end(),
                            std::inserter(pendingNodes, pendingNodes.end()));
        pendingNodes.erase(node);
    }
    return reachableNodes;
}
