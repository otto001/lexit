//
// Created by andreas on 08.04.21.
//

#include "Nfa.h"
#include "../util.h"
#include <algorithm>

std::unordered_set<char> Nfa::escapableCharacter = {'(', ')', '[', ']', '{', '}', '?', '*', '+', '|', '\\'};


Nfa::Nfa(const std::string &regex) {
    auto result = buildEpsNfa(regex);
    head = result.first;
    finalState = result.second;

    eliminateEpsilonTransitions();
}

Nfa::~Nfa() {
    auto allNodes = allReachable();
    for (auto node : allNodes) {
        delete node;
    }
}

std::pair<NfaNode *, NfaNode *> Nfa::buildEpsNfa(const std::string &regex) {
    bool escape = false;

    auto head = new NfaNode();
    auto finalState = new NfaNode();
    finalState->setFinal(true);

    auto last = head;
    NfaNode *beforeLast = nullptr;

    auto appendSymbol = [&last, &beforeLast, &escape](char c) {
        auto epsilonNode = new NfaNode();
        last->insertEpsilonTransition(epsilonNode);
        auto newNode = new NfaNode();
        if (escape && escapableCharacter.find(c) != escapableCharacter.end()) {
            escape = false;
        }
        epsilonNode->insertTransition(c, !escape, newNode);
        beforeLast = last;
        last = newNode;
        escape = false;
    };

    for (auto it = regex.begin(); it != regex.end(); it++) {
        if (escape) {
            appendSymbol(*it);
        } else {
            switch (*it) {
                case '(': {
                    auto bracketContent = extractBracket(it, regex.end()).value();
                    auto newNfa = buildEpsNfa(bracketContent);
                    last->insertEpsilonTransition(newNfa.first);
                    newNfa.second->setFinal(false);
                    beforeLast = last;
                    last = newNfa.second;
                    it += (long) bracketContent.size() + 1;
                    break;
                }
                case '|':
                    last->insertEpsilonTransition(finalState);
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
                    escape = true;
                    break;
                default:
                    appendSymbol(*it);
                    break;
            }
        }
    }
    last->insertEpsilonTransition(finalState);
    return std::pair<NfaNode *, NfaNode *>(head, finalState);
}

bool Nfa::run(const std::string &runString) {
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

    auto remaining = allReachable();
    remaining.insert(finalState);
    std::vector<NfaNode *> unreachableNodes;
    std::set_difference(allNodes.begin(), allNodes.end(), remaining.begin(), remaining.end(),
                        std::inserter(unreachableNodes, unreachableNodes.end()));
    for (auto node : unreachableNodes) {
        delete node;
    }
    delete finalState;
    finalState = nullptr;
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



