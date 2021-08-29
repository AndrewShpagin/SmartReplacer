#include "SmartReplacer.h"

CharactersBitset::CharactersBitset() { for (int i = 0; i < 32; i++)field[i] = 0; }

CharactersBitset::CharactersBitset(const char* initial) { smart(initial); }

void CharactersBitset::add(const char* charset) { for (int i = 0; charset[i]; i++)set(charset[i]); }

void CharactersBitset::range(int start, int end) {
	auto s = static_cast<unsigned char>(start);
	auto e = static_cast<unsigned char>(end);
	for (int i = s; i <= e; i++) { set(static_cast<char>(i)); }
}

void CharactersBitset::smart(const char* charset) {
	for (int i = 0; charset[i]; i++) {
		char c = charset[i];
		if (i > 0 && c == '.' && charset[i + 1] == '.') {
			range(charset[i - 1], charset[i + 2]);
			i += 2;
		}
		else
			set(charset[i]);
	}
}

bool CharactersBitset::check(char c) const {
	unsigned char u = (unsigned char)c;
	return (field[u >> 3] & (1 << (u & 7))) != 0;
}

void CharactersBitset::set(char c) {
	unsigned char u = (unsigned char)c;
	field[u >> 3] |= 1 << (u & 7);
}

void CharactersBitset::remove(char c) {
	unsigned char u = (unsigned char)c;
	field[u >> 3] &= ~(1 << (u & 7));
}

void CharactersBitset::allow_all() { for (int i = 0; i < 32; i++) { field[i] = 255; } }

void CharactersBitset::nothing() { for (int i = 0; i < 32; i++) { field[i] = 0; } }
