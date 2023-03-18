#include "crossword.hpp"

#include <iostream>
#include <cstring>
#include <cmath>
#include <sstream>

auto Crossword::Crossword::load_from_stream(std::ifstream &stream) -> Crossword
{
    // I'm using a C-style array here because it was our teacher's requirement.
    char crossword_content[MAX_HEIGHT][MAX_WIDTH];

    auto crossword_height = 0;

    do
    {
        stream.getline(crossword_content[crossword_height], MAX_WIDTH);

        crossword_height++;
    } while (strcmp(crossword_content[crossword_height - 1], "") != 0);

    crossword_height--;

    auto crossword_width = strlen(crossword_content[0]);

    std::vector<std::vector<std::shared_ptr<Entry::AnswerCharacter>>> crossword_fields;

    crossword_fields.resize(crossword_height);

    for (auto i = 0; i < crossword_height; i++)
    {
        crossword_fields[i].resize(crossword_width);

        for (auto j = 0; j < crossword_width; j++)
        {
            if (crossword_content[i][j] == '#')
            {
                crossword_fields[i][j] = nullptr;
            }
            else
            {
                crossword_fields[i][j] = std::make_shared<Entry::AnswerCharacter>(' ');
            }
        }
    }

    Crossword crossword;

    crossword.width = crossword_width;
    crossword.height = crossword_height;

    // Read crossword entries
    while (!stream.eof())
    {
        std::string entry;

        std::getline(stream, entry);

        std::istringstream entry_stream(entry);

        std::string location_x_str;

        std::getline(entry_stream, location_x_str, ' ');

        auto location_x = std::stoi(location_x_str);

        std::string location_y_str;

        std::getline(entry_stream, location_y_str, ' ');

        auto location_y = std::stoi(location_y_str);

        std::string direction_str;

        std::getline(entry_stream, direction_str, ' ');

        Entry::Direction direction;

        if (direction_str == "pion")
        {
            direction = Entry::Direction::VERTICAL;
        }
        else if (direction_str == "poziom")
        {
            direction = Entry::Direction::HORIZONTAL;
        }
        else
        {
            throw InvalidDirectionStringError(InvalidDirectionStringError(direction_str));
        }

        std::string clue;

        std::getline(entry_stream, clue);

        std::string correct;
        std::vector<std::shared_ptr<Entry::AnswerCharacter>> entry_characters;

        if (direction == Entry::Direction::VERTICAL)
        {
            for (auto i = 0; location_y + i < crossword_height && crossword_content[location_y + i][location_x] != '#'; i++)
            {
                correct += crossword_content[location_y + i][location_x];
                entry_characters.push_back(crossword_fields[location_y + i][location_x]);
            }
        }
        else if (direction == Entry::Direction::HORIZONTAL)
        {
            for (auto i = 0; location_x + i < crossword_width && crossword_content[location_y][location_x + i] != '#'; i++)
            {
                correct += crossword_content[location_y][location_x + i];
                entry_characters.push_back(crossword_fields[location_y][location_x + i]);
            }
        }

        crossword.entries.emplace_back(entry_characters, std::make_pair(location_x, location_y), direction, correct, clue);
    }

    return crossword;
}

auto Crossword::Crossword::answer(size_t entry_index, std::string answer) -> void {
    entries[entry_index].set_answer(answer);
}

auto Crossword::Crossword::check() -> void {
    for (auto &entry : entries) {
        entry.check();
    }
}