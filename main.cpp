#include <iostream>
#include "NFA/Nfa.h"

int main() {
    Nfa nfa("abc");
    while (true) {
        std::string input;
        std::cin >> input;
        auto res = nfa.run(input);
        std::cout << res << std::endl;
    }

}
