//
// Created by andreas on 08.04.21.
//

#include "NfaNode.h"
#include <stdexcept>
#include <memory>

void NfaNode::insertTransition(Symbol symbol, bool escape, NfaNode *nfaNode) {
    if (symbol == '.') {
        escape = !escape;
    }
    if (escape) {
        switch (symbol) {
            case 'w':
                wordCharacterTransitions.insert(nfaNode);
                break;
            case 'W':
                nonWordCharacterTransitions.insert(nfaNode);
                break;
            case 'd':
                digitCharacterTransitions.insert(nfaNode);
                break;
            case 'D':
                nonDigitCharacterTransitions.insert(nfaNode);
                break;
            case 's':
                whitespaceCharacterTransitions.insert(nfaNode);
                break;
            case 'S':
                nonWhitespaceCharacterTransitions.insert(nfaNode);
                break;
            case '.':
                wildCardTransitions.insert(nfaNode);
                break;
            default:
                throw std::invalid_argument("wrong escapable character");
        }
    } else {
        auto it = transitions.find(symbol);
        if (it == transitions.end()) {
            transitions[symbol] = {nfaNode};
        } else {
            it->second.insert(nfaNode);
        }
    }
}

void NfaNode::insertEpsilonTransition(NfaNode *nfaNode) {
    epsilonTransitions.insert(nfaNode);
}

void NfaNode::removeEpsilonTransition(NfaNode *nfaNode) {
    epsilonTransitions.erase(nfaNode);
}

std::set<NfaNode *> NfaNode::next(Symbol symbol) {
    if (!epsilonTransitions.empty()) {
        throw std::domain_error("calling next on a node with epsilon transitions is not supported");
    }
    std::set<NfaNode *> result;

    if ('0' <= symbol && symbol <= '9') {
        result.insert(digitCharacterTransitions.begin(), digitCharacterTransitions.end());
        result.insert(wordCharacterTransitions.begin(), wordCharacterTransitions.end());
    } else {
        result.insert(nonDigitCharacterTransitions.begin(), nonDigitCharacterTransitions.end());
        if (isalpha(symbol) || symbol == '_') {
            result.insert(wordCharacterTransitions.begin(), wordCharacterTransitions.end());
        } else {
            result.insert(nonWordCharacterTransitions.begin(), nonWordCharacterTransitions.end());
            if (isspace(symbol)) {
                result.insert(whitespaceCharacterTransitions.begin(), whitespaceCharacterTransitions.end());
            } else {
                result.insert(nonWhitespaceCharacterTransitions.begin(), nonWhitespaceCharacterTransitions.end());
            }
        }
    }

    if (symbol != '\n') {
        result.insert(wildCardTransitions.begin(), wildCardTransitions.end());
    }

    auto it = transitions.find(symbol);
    if (it != transitions.end()) {
        result.insert(it->second.begin(), it->second.end());
    }

    return result;
}

std::set<NfaNode *> NfaNode::getDirectlyReachable() {
    std::set<NfaNode *> result;
    result.insert(wordCharacterTransitions.begin(), wordCharacterTransitions.end());
    result.insert(nonWordCharacterTransitions.begin(), nonWordCharacterTransitions.end());

    result.insert(whitespaceCharacterTransitions.begin(), whitespaceCharacterTransitions.end());
    result.insert(nonWhitespaceCharacterTransitions.begin(), nonWhitespaceCharacterTransitions.end());

    result.insert(digitCharacterTransitions.begin(), digitCharacterTransitions.end());
    result.insert(nonDigitCharacterTransitions.begin(), nonDigitCharacterTransitions.end());

    result.insert(wildCardTransitions.begin(), wildCardTransitions.end());

    result.insert(epsilonTransitions.begin(), epsilonTransitions.end());

    for (const auto &pair: transitions) {
        result.insert(pair.second.begin(), pair.second.end());
    }

    return result;
}

const std::set<NfaNode *> &NfaNode::getDirectlyEpsilonReachable() const {
    return epsilonTransitions;
}

void NfaNode::insertTransitions(NfaNode *other) {
    wordCharacterTransitions.insert(other->wordCharacterTransitions.begin(), other->wordCharacterTransitions.end());
    nonWordCharacterTransitions.insert(other->nonWordCharacterTransitions.begin(), other->nonWordCharacterTransitions.end());

    whitespaceCharacterTransitions.insert(other->whitespaceCharacterTransitions.begin(), other->whitespaceCharacterTransitions.end());
    nonWhitespaceCharacterTransitions.insert(other->nonWhitespaceCharacterTransitions.begin(), other->nonWhitespaceCharacterTransitions.end());

    digitCharacterTransitions.insert(other->digitCharacterTransitions.begin(), other->digitCharacterTransitions.end());
    nonDigitCharacterTransitions.insert(other->nonDigitCharacterTransitions.begin(), other->nonDigitCharacterTransitions.end());

    wildCardTransitions.insert(other->wildCardTransitions.begin(), other->wildCardTransitions.end());

    epsilonTransitions.insert(other->epsilonTransitions.begin(), other->epsilonTransitions.end());

    for (const auto &pair: other->transitions) {

        auto it = transitions.find(pair.first);
        if (it == transitions.end()) {
            transitions[pair.first] = pair.second;
        } else {
            transitions[pair.first].insert(pair.second.begin(), pair.second.end());

        }

    }

}

