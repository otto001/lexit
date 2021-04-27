#include <iostream>
#include <chrono>
#include <regex>
#include "NFA/Nfa.h"

int main() {
    std::string regex = "a(b|c|\\?)*d";
    Nfa nfa(regex);
    std::regex test_regex(regex);

    std::cout << "done" << std::endl;

    while (true) {
        std::string input;
        std::cin >> input;

        auto t1 = std::chrono::high_resolution_clock::now();
        auto res = nfa.run(input);
        auto t2 = std::chrono::high_resolution_clock::now();
        /* Getting number of milliseconds as a double. */
        std::chrono::duration<double, std::micro> us_double = t2 - t1;
        std::cout << us_double.count() << "us" << std::endl;
        std::cout << res << std::endl;



        t1 = std::chrono::high_resolution_clock::now();
        res = std::regex_search(input, test_regex);
        t2 = std::chrono::high_resolution_clock::now();
        /* Getting number of milliseconds as a double. */
        us_double = t2 - t1;
        std::cout << us_double.count() << "us" << std::endl;
        std::cout << res << std::endl;

    }

}
