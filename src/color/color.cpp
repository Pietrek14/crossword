#include "color.hpp"

Color::Modifier::Modifier(Code code) : code(code) {}

std::ostream &Color::operator<<(std::ostream &os, const Modifier &mod) {
	return os << "\033[" << mod.code << "m";
}

const Color::Modifier Color::Modifier::FG_RESET = Modifier(Color::Code::FG_DEFAULT);
const Color::Modifier bg_reset = Color::Modifier(Color::Code::BG_DEFAULT);