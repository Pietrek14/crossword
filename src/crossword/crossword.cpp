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

    std::vector<std::vector<std::shared_ptr<char>>> crossword_fields;

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
                crossword_fields[i][j] = std::make_shared<char>(' ');
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
            throw std::runtime_error("Invalid direction");
        }

        std::string clue;

        std::getline(entry_stream, clue);

        std::string correct;
        std::vector<std::shared_ptr<char>> entry_characters;

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

Crossword::Display::Display(const Crossword& crossword) : crossword(crossword) {}

void Crossword::Display::display(std::ostream &stream) const
{
    using Entry = Crossword::Entry;

    char **crossword_content = new char *[crossword.height + 1];

    for (auto i = 0; i < crossword.height; i++)
    {
        crossword_content[i] = new char[crossword.width + 1];

        for (auto j = 0; j < crossword.width; j++)
        {
            crossword_content[i][j] = '#';
        }
    }

    for (const auto &entry : crossword.entries)
    {
        auto entry_position = entry.get_position();
        auto entry_direction = entry.get_direction();
        auto entry_length = entry.get_length();

        if (entry_direction == Entry::Direction::VERTICAL)
        {
            for (auto i = 0; i < entry_length; i++)
            {
                crossword_content[entry_position.second + i][entry_position.first] = entry.get_character(i);
            }
        }
        else if (entry_direction == Entry::Direction::HORIZONTAL)
        {
            for (auto i = 0; i < entry_length; i++)
            {
                crossword_content[entry_position.second][entry_position.first + i] = entry.get_character(i);
            }
        }
    }

    std::pair<uint8_t, uint8_t> scale_factor(5, 3);

    // Display the column numbers
    for (auto i = 0; i < scale_factor.second; i++)
    {
        if (i % scale_factor.second != scale_factor.second / 2)
        {
            for (auto j = 0; j < crossword.width * scale_factor.first; j++)
                stream << ' ';

            stream << std::endl;

            continue;
        }

        for (auto j = 0; j < crossword.width + 1; j++)
        {
            int num_length;

            if (j == 0)
                num_length = 0;
            else
                num_length = std::log10(j) + 1;

            auto pad_left = (scale_factor.first - num_length) / 2;
            auto pad_right = scale_factor.first - num_length - pad_left;

            for (auto k = 0; k < pad_left; k++)
                stream << ' ';

            if (j != 0)
            {
                stream << j;
            }

            for (auto k = 0; k < pad_right; k++)
                stream << ' ';
        }

        stream << std::endl;
    }

    for (auto i = 0; i < crossword.height * scale_factor.second; i++)
    {
        for (auto j = 0; j < (crossword.width + 1) * scale_factor.first; j++)
        {
            // Display the row numbers
            if (j < scale_factor.first + 1)
            {
                if (i % scale_factor.second != scale_factor.second / 2 && j != scale_factor.first)
                {
                    stream << ' ';
                    continue;
                }

                if (j != 0)
                    continue;

                int row_number = i / scale_factor.second + 1;

                int num_length;

                num_length = std::log10(row_number) + 1;

                auto pad_left = (scale_factor.first - num_length) / 2;
                auto pad_right = scale_factor.first - num_length - pad_left;

                for (auto k = 0; k < pad_left; k++)
                    stream << ' ';

                stream << row_number;

                for (auto k = 0; k < pad_right; k++)
                    stream << ' ';

                continue;
            }

            auto &current = crossword_content[i / scale_factor.second][(j - 1) / scale_factor.first - 1];
            if (current == '#')
                stream << '#';
            else
            {
                if (i % scale_factor.second == scale_factor.second / 2 && (j - 1) % scale_factor.first == scale_factor.first / 2)
                    stream << current;
                else
                    stream << ' ';
            }
        }

        stream << std::endl;
    }

    for (auto i = 0; i < crossword.height; i++)
    {
        delete[] crossword_content[i];
    }

    delete[] crossword_content;
}

void Crossword::Display::display_clues(std::ostream &stream) const
{
    for (auto i = 0; i < crossword.entries.size(); i++)
    {
        stream << i + 1 << ". " << crossword.entries[i] << std::endl;
    }
}

auto Crossword::operator<<(std::ostream &stream, const Crossword::Crossword::Entry &entry) -> std::ostream &
{
    auto position = entry.get_position();
    auto direction = entry.get_direction();

    stream
        << position.first << ", " << position.second
        << " (" << (direction == Crossword::Crossword::Entry::Direction::HORIZONTAL ? "poziom" : "pion") << ")"
        << entry.clue;

    return stream;
}

Crossword::Crossword::Entry::Entry(std::vector<std::shared_ptr<char>> characters, Position position, Direction direction, std::string correct, std::string clue) : characters(characters), position(position), direction(direction), correct(correct), clue(clue)
{
    if (characters.size() != correct.size())
    {
        throw std::runtime_error("Characters and correct answer must have the same length");
    }
}

auto Crossword::Crossword::Entry::get_length() const -> size_t
{
    return characters.size();
}

auto Crossword::Crossword::Entry::get_direction() const -> Direction
{
    return direction;
}

auto Crossword::Crossword::Entry::get_position() const -> Entry::Position
{
    return position;
}

auto Crossword::Crossword::Entry::get_character(size_t index) const -> char
{
    return *characters[index];
}
