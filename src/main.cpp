#include <iostream>
#include <fstream>

#include "crossword/crossword.hpp"

class Action
{
public:
    enum
    {
        ANSWER,
        HELP,
        CHECK,
        FINISH,
        QUIT,
        UNKNOWN
    };

    static auto from_string(std::string);
};

auto Action::from_string(std::string action)
{
    if (action == "answer")
    {
        return Action::ANSWER;
    }
    else if (action == "help")
    {
        return Action::HELP;
    }
    else if (action == "check")
    {
        return Action::CHECK;
    }
    else if (action == "finish")
    {
        return Action::FINISH;
    }
    else if (action == "quit")
    {
        return Action::QUIT;
    }
    else
    {
        return Action::UNKNOWN;
    }
}

auto main() -> int
{
    std::cout << "  -- CROSSWORD -- " << std::endl;

    std::ifstream input("crossword.cwd");

    if (input.is_open())
    {
        try
        {
            auto crossword = Crossword::Crossword::load_from_stream(input);

            auto crossword_display = Crossword::Display(crossword);

            input.close();

            bool playing = true;
            uint16_t checks_count = 0;

            while (playing)
            {
                crossword_display.display(std::cout);
                std::cout << std::endl;
                crossword_display.display_clues(std::cout);
                std::cout << std::endl;

                std::string input;

                std::cout << "Perform an action (input 'help' to learn about available actions): ";

                std::cin >> input;

                auto action = Action::from_string(input);

                switch (action)
                {
                case Action::ANSWER:
                {
                    std::cout << "Enter the index of the entry you want to answer: ";
                    std::cin >> input;

                    auto entry_index = std::stoi(input) - 1;

                    std::cout << "Enter the answer: ";
                    std::cin >> input;

                    try
                    {
                        crossword.answer(entry_index, input);
                    }
                    catch (Crossword::IncorrectAnswerLengthError &e)
                    {
                        std::cout << "Your answer was " << e.get_actual() << " characters long, while the entry is " << e.get_expected() << " characters long." << std::endl;
                    }

                    break;
                }
                case Action::CHECK:
                {
                    crossword.check();
                    checks_count++;

                    std::cout << "Checked " << checks_count << " times." << std::endl;

                    break;
                }
                case Action::HELP:
                {
                    std::cout << "Available actions:" << std::endl;
                    std::cout << "  answer - answer an entry" << std::endl;
                    std::cout << "  help - display this help message" << std::endl;
                    std::cout << "  check - check which squares are incorrect" << std::endl;
                    std::cout << "  finish - finish the crossword" << std::endl;
                    std::cout << "  quit - quit the program" << std::endl;

                    break;
                }
                case Action::QUIT:
                {
                    playing = false;
                    break;
                }
                default:
                {
                    std::cout << "Unknown action '" << input << "'" << std::endl;
                    break;
                }
                }
            }
        }
        catch (std::exception &e)
        {
            std::cerr << "Unexpected error: " << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Could not open file 'crossword.cwd'" << std::endl;
    }

    return 0;
}