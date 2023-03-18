#pragma once

#include <string>
#include <vector>
#include <memory>
#include <vector>
#include <utility>
#include <cstdint>
#include <istream>
#include <fstream>

namespace Crossword {
	const size_t MAX_WIDTH = 255;
	const size_t MAX_HEIGHT = 255;

    class Crossword {
		friend class Display;

		size_t width;
		size_t height;
    public:
        class Entry {
		public:
            enum class Direction {
                VERTICAL,
                HORIZONTAL
            };

			using Position = std::pair<uint16_t, uint16_t>;

		private:
            std::vector<std::shared_ptr<char>> characters;
            Position position;
            Direction direction;

		public:
			auto get_length() const -> size_t;
			auto get_direction() const -> Direction;
			auto get_position() const -> Position;
			auto get_character(size_t index) const -> char;

			Entry(std::vector<std::shared_ptr<char>> characters, Position position, Direction direction, std::string correct, std::string clue);

            std::string correct;
			std::string clue;
        };

		std::vector<Entry> entries;

        static auto load_from_stream(std::ifstream& stream) -> Crossword;
    };

	class Display {
		const Crossword& crossword;

	public:
		Display(const Crossword& crossword);

		void display(std::ostream& stream) const;
		void display_clues(std::ostream& stream) const;
	};

	auto operator<<(std::ostream& stream, const Crossword::Entry& entry) -> std::ostream&;
}

