#include <iostream>
#include <fstream>

#include "crossword/crossword.hpp"

auto main() -> int {
    std::cout << "  -- CROSSWORD -- " << std::endl;

    using Crossword = Crossword::Crossword;

    std::ifstream input("crossword.cwd");

    auto crossword = Crossword::load_from_stream(input);

    input.close();

    return 0;
}