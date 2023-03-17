#include "crossword.hpp"

#include <iostream>
#include <cstring>
#include <vcruntime.h>

auto Crossword::Crossword::load_from_stream(std::istream &stream) -> Crossword
{
    // I'm using a C-style array here because it was our teacher's requirement.
    char crossword_content[MAX_HEIGHT][MAX_WIDTH];

    auto crossword_height = 0;

    do
    {
        stream.getline(crossword_content[crossword_height], MAX_WIDTH);

        crossword_height++;
    } while (strcmp(crossword_content[crossword_height], "") != 0);

    crossword_height--;

    auto crossword_width = strlen(crossword_content[0]);

    std::shared_ptr<char> crossword_fields[MAX_HEIGHT][MAX_WIDTH];

    for (auto i = 0; i < crossword_height; i++)
    {
        for (auto j = 0; j < crossword_width; j++)
        {
            if (crossword_content[i][j] == '#')
            {
                crossword_fields[i][j] = nullptr;
            }
            else
            {
                crossword_fields[i][j] = std::make_shared<char>(' ');
            }
        }
    }

    Crossword crossword;

    std::string entry;

    // Read crossword entries
    while (!stream.eof())
    {
        std::getline(stream, entry);

        auto separator = entry.find(" - ");

        auto entry_location = entry.substr(0, separator);
        auto entry_clue = entry.substr(separator + 3);

        auto entry_location_x_separator = entry_location.find(" ");

        auto entry_location_x = std::stoi(entry_location.substr(0, entry_location_x_separator));

        auto entry_location_y_separator = entry_location.find(" ", entry_location_x_separator + 1);

        auto entry_location_y = std::stoi(entry_location.substr(entry_location_x_separator + 1, entry_location_y_separator));

        auto entry_location_direction = entry_location.substr(entry_location_y_separator + 1);

        Entry::Direction entry_direction;

        if (entry_location_direction == "pion")
        {
            entry_direction = Entry::Direction::VERTICAL;
        }
        else if (entry_location_direction == "poziom")
        {
            entry_direction = Entry::Direction::HORIZONTAL;
        }
        else
        {
            throw std::runtime_error("Invalid direction");
        }

        std::string entry_correct;
        std::vector<std::shared_ptr<char>> entry_characters;

        if (entry_direction == Entry::Direction::VERTICAL)
        {
            for (auto i = 0; entry_location_y + i < crossword_height && crossword_content[entry_location_y + i][entry_location_x] != '#'; i++)
            {
                entry_correct += crossword_content[entry_location_y + i][entry_location_x];
                entry_characters.push_back(crossword_fields[entry_location_y + i][entry_location_x]);
            }
        }
        else if (entry_direction == Entry::Direction::HORIZONTAL)
        {
            for (auto i = 0; entry_location_x + i < crossword_width && crossword_content[entry_location_y][entry_location_x + i] != '#'; i++)
            {
                entry_correct += crossword_content[entry_location_y][entry_location_x + i];
                entry_characters.push_back(crossword_fields[entry_location_y][entry_location_x + i]);
            }
        }

        crossword.entries.emplace_back(entry_characters, std::make_pair(entry_location_x, entry_location_y), entry_direction, entry_correct, entry_clue);
    }

    return crossword;
}

Crossword::Crossword::Entry::Entry(std::vector<std::shared_ptr<char>> characters,std::pair<uint16_t, uint16_t> position, Direction direction, std::string correct, std::string clue) : characters(characters), position(position), direction(direction), correct(correct), clue(clue)
{
    if(characters.size() != correct.size())
    {
        throw std::runtime_error("Characters and correct answer must have the same length");
    }
}

auto Crossword::Crossword::Entry::get_length() -> size_t const
{
    return characters.size();
}

auto Crossword::Crossword::Entry::get_direction() -> Direction const
{
    return direction;
}
