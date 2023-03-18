#include "crossword.hpp"

Crossword::InvalidDirectionStringError::InvalidDirectionStringError(std::string direction_string)
	: std::runtime_error("Invalid direction string: " + direction_string), direction_string(direction_string) {}

auto Crossword::InvalidDirectionStringError::get_direction_string() const -> std::string {
	return direction_string;
}

Crossword::IncorrectAnswerLengthError::IncorrectAnswerLengthError(uint16_t expected, uint16_t actual)
	: std::runtime_error("Incorrect answer length: expected " + std::to_string(expected) + ", got " + std::to_string(actual)), expected(expected), actual(actual) {}

auto Crossword::IncorrectAnswerLengthError::get_expected() const -> uint16_t {
	return expected;
}

auto Crossword::IncorrectAnswerLengthError::get_actual() const -> uint16_t {
	return actual;
}