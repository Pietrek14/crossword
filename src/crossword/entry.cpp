#include "crossword.hpp"

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
