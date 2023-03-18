#include "crossword.hpp"

#include "../color/color.hpp"

#include <optional>

Crossword::Display::Display(const Crossword& crossword) : crossword(crossword) {}

template<typename T>
void display_array(std::ostream &stream, std::optional<T>** array, std::pair<size_t, size_t> array_size, std::pair<uint8_t, uint8_t> scale_factor = { 5, 3 }) {
    // Display the column numbers
    for (auto i = 0; i < scale_factor.second; i++)
    {
        if (i % scale_factor.second != scale_factor.second / 2)
        {
            for (auto j = 0; j < array_size.first * scale_factor.first; j++)
                stream << ' ';

            stream << std::endl;

            continue;
        }

        for (auto j = 0; j < array_size.first + 1; j++)
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

    for (auto i = 0; i < array_size.second * scale_factor.second; i++)
    {
        for (auto j = 0; j < (array_size.first + 1) * scale_factor.first; j++)
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

            auto &current = array[i / scale_factor.second][(j - 1) / scale_factor.first - 1];
            if (current == std::nullopt)
                stream << '#';
            else
            {
                if (i % scale_factor.second == scale_factor.second / 2 && (j - 1) % scale_factor.first == scale_factor.first / 2) {
                    stream << current.value();
                }
                else
                    stream << ' ';
            }
        }

        stream << std::endl;
    }
}

void Crossword::Display::display(std::ostream &stream) const
{
    using Entry = Crossword::Entry;
    using OptAnswerCharacter = std::optional<Crossword::Entry::AnswerCharacter>;

    auto **crossword_content = new OptAnswerCharacter *[crossword.height];

    for (auto i = 0; i < crossword.height; i++)
    {
        crossword_content[i] = new OptAnswerCharacter[crossword.width];

        for (auto j = 0; j < crossword.width; j++)
        {
            crossword_content[i][j] = std::nullopt;
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

    display_array(stream, crossword_content, { crossword.width, crossword.height });

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

void Crossword::Display::display_correct(std::ostream &stream) const
{
    using Entry = Crossword::Entry;

    auto **crossword_content = new std::optional<char> *[crossword.height];

    for (auto i = 0; i < crossword.height; i++)
    {
        crossword_content[i] = new std::optional<char>[crossword.width];

        for (auto j = 0; j < crossword.width; j++)
        {
            crossword_content[i][j] = std::nullopt;
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
                crossword_content[entry_position.second + i][entry_position.first] = std::tolower(entry.correct_answer[i]);
            }
        }
        else if (entry_direction == Entry::Direction::HORIZONTAL)
        {
            for (auto i = 0; i < entry_length; i++)
            {
                crossword_content[entry_position.second][entry_position.first + i] = std::tolower(entry.correct_answer[i]);
            }
        }
    }

    display_array(stream, crossword_content, { crossword.width, crossword.height });

    for (auto i = 0; i < crossword.height; i++)
    {
        delete[] crossword_content[i];
    }

    delete[] crossword_content;
}