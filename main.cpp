#include <iostream>
#include <chrono>
#include <regex>
#include "NFA/Nfa.h"

int main() {
    std::string regex = "a(b|c|\\?)*d";
    std::string testString = "abbbbbcccbcbbc????bbc????cccd";
    constexpr uint32_t runs = 100000;

    Nfa nfa(regex);
    std::cout << "done" << std::endl;


    std::regex test_regex(regex);


    double totalDiffs = 0;
    for (uint32_t i = 0; i < runs; i++) {
        auto t1 = std::chrono::high_resolution_clock::now();
        auto res = nfa.run(testString);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> nfaRunTimeUs = t2 - t1;


        t1 = std::chrono::high_resolution_clock::now();
        res = std::regex_search(testString, test_regex);
        t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> diffUs = nfaRunTimeUs - (t2 - t1);
        totalDiffs += diffUs.count();
    }
    totalDiffs /= runs;
    std::cout << "avg time diff: " << totalDiffs << "us" << std::endl;


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
