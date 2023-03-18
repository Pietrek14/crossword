#include <iostream>
#include <fstream>

#include "crossword/crossword.hpp"

auto main() -> int {
    std::cout << "  -- CROSSWORD -- " << std::endl;

    using Crossword = Crossword::Crossword;

    std::ifstream input("crossword.cwd");

    if(input.is_open()) {
        try {
            auto crossword = Crossword::load_from_stream(input);

            input.close();

            std::cout << crossword << std::endl;
        } catch(std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    else {
        std::cerr << "Could not open file 'crossword.cwd'" << std::endl;
    }

    std::cout << "  -- END -- " << std::endl;

    return 0;
}