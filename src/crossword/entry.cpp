#include "crossword.hpp"
#include <cctype>

Crossword::Crossword::Entry::Entry(std::vector<std::shared_ptr<AnswerCharacter>> answer, Position position, Direction direction, std::string correct, std::string clue) : answer(answer), position(position), direction(direction), correct_answer(correct), clue(clue)
{
    if (answer.size() != correct.size())
    {
        throw IncorrectAnswerLengthError(correct.size(), answer.size());
    }
}

auto Crossword::Crossword::Entry::get_length() const -> size_t
{
    return answer.size();
}

auto Crossword::Crossword::Entry::get_direction() const -> Direction
{
    return direction;
}

auto Crossword::Crossword::Entry::get_position() const -> Entry::Position
{
    return position;
}

auto Crossword::Crossword::Entry::get_character(size_t index) const -> AnswerCharacter
{
    return *answer[index];
}

auto Crossword::Crossword::Entry::set_answer(std::string new_answer) -> void
{
    if (new_answer.size() != correct_answer.size())
    {
        throw IncorrectAnswerLengthError(correct_answer.size(), new_answer.size());
    }

    for (size_t i = 0; i < new_answer.size(); i++)
    {
        // The AnswerCharacter constructor will automatically set the state to UNKNOWN
        *answer[i] = std::tolower(new_answer[i]);
    }
}

auto Crossword::Crossword::Entry::check() -> void
{
    for(size_t i = 0; i < answer.size(); i++)
    {
        if (std::tolower(answer[i]->character) == std::tolower(correct_answer[i]))
        {
            answer[i]->state = AnswerCharacter::State::CORRECT;
        }
        else
        {
            answer[i]->state = AnswerCharacter::State::INCORRECT;
        }
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

Crossword::Crossword::Entry::AnswerCharacter::AnswerCharacter(char character, State state) : character(character), state(state) {}