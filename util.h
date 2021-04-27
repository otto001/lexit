//
// Created by andreas on 27.04.21.
//

#ifndef LEXIT_UTIL_H
#define LEXIT_UTIL_H

#include <string>
#include <algorithm>

std::optional<std::string> extractBracket(std::string::const_iterator begin, std::string::const_iterator end) {
    size_t bracketCount = 0;

    begin = std::find(begin, end, '(');
    if (begin == end) {
        return {};
    }
    begin++;
    for (auto it = begin; it != end; it++) {
        switch (*it) {
            case '(':
                bracketCount++;
                break;
            case ')':
                if (bracketCount == 0) {
                    return std::string(begin,it);
                } else {
                    bracketCount--;
                }
                break;
            default:
                break;
        }
    }
    throw std::invalid_argument("more opening than closing Brackets");
}

#endif //LEXIT_UTIL_H
