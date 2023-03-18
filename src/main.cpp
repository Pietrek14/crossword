#include <iostream>
#include <fstream>

#include "crossword/crossword.hpp"

auto main() -> int {
    std::cout << "  -- CROSSWORD -- " << std::endl;

    std::ifstream input("crossword.cwd");

    if(input.is_open()) {
        try {
            auto crossword = Crossword::Crossword::load_from_stream(input);

            auto crossword_display = Crossword::Display(crossword);

            input.close();

            crossword_display.display(std::cout);
            std::cout << std::endl;
            crossword_display.display_clues(std::cout);
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