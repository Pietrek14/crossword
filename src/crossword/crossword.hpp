#pragma once

#include <string>
#include <vector>
#include <memory>
#include <vector>
#include <utility>
#include <cstdint>
#include <istream>
#include <fstream>
#include <stdexcept>

namespace Crossword
{
	const size_t MAX_WIDTH = 255;
	const size_t MAX_HEIGHT = 255;

	class InvalidDirectionStringError : public std::runtime_error
	{
		std::string direction_string;
	public:
		InvalidDirectionStringError(std::string direction_string);

		auto get_direction_string() const -> std::string;
	};

	class IncorrectAnswerLengthError : public std::runtime_error
	{
		uint16_t expected;
		uint16_t actual;
	public:
		IncorrectAnswerLengthError(uint16_t expected, uint16_t actual);

		auto get_expected() const -> uint16_t;
		auto get_actual() const -> uint16_t;
	};

	class Crossword
	{
		friend class Display;

		size_t width;
		size_t height;

	public:
		class Entry
		{
		public:
			enum class Direction
			{
				VERTICAL,
				HORIZONTAL
			};

			using Position = std::pair<uint16_t, uint16_t>;

		private:
			std::vector<std::shared_ptr<char>> answer;
			Position position;
			Direction direction;

		public:
			std::string correct_answer;
			std::string clue;

			auto get_length() const -> size_t;
			auto get_direction() const -> Direction;
			auto get_position() const -> Position;
			auto get_character(size_t index) const -> char;

			Entry(std::vector<std::shared_ptr<char>> characters, Position position, Direction direction, std::string correct, std::string clue);

			void set_answer(std::string new_answer);
		};

		std::vector<Entry> entries;

		static auto load_from_stream(std::ifstream &stream) -> Crossword;

		auto answer(size_t entry_index, std::string answer) -> void;
	};

	class Display
	{
		const Crossword &crossword;

	public:
		Display(const Crossword &crossword);

		void display(std::ostream &stream) const;
		void display_clues(std::ostream &stream) const;
	};

	auto operator<<(std::ostream &stream, const Crossword::Entry &entry) -> std::ostream &;
}
