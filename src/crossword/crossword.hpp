#pragma once

#include <string>
#include <vector>
#include <memory>
#include <vcruntime.h>
#include <vector>
#include <utility>
#include <cstdint>
#include <istream>

namespace Crossword {
	const size_t MAX_WIDTH = 255;
	const size_t MAX_HEIGHT = 255;

    class Crossword {
    public:
        class Entry {
		public:
            enum class Direction {
                VERTICAL,
                HORIZONTAL
            };

		private:
            std::vector<std::shared_ptr<char>> characters;
            std::pair<uint16_t, uint16_t> position;
            Direction direction;

		public:
			auto get_length() -> size_t const;
			auto get_direction() -> Direction const;

			Entry(std::vector<std::shared_ptr<char>> characters, std::pair<uint16_t, uint16_t> position, Direction direction, std::string correct, std::string clue);

            std::string correct;
			std::string clue;
        };

		std::vector<Entry> entries;

        static auto load_from_stream(std::istream& stream) -> Crossword;
    };
}

