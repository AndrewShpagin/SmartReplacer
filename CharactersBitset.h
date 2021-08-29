#pragma once

/**
 * \brief The class to check quickly if character is in defined characters set
 */
class CharactersBitset {
	///bitfield with characters
	unsigned char field[32]{};
public:
	CharactersBitset();
	CharactersBitset(const char* initial);
	///add charset to bitfield
	void add(const char* set);
	///add characters range to bitfield
	void range(int start, int end);
	///add sequences like "A..Z0..9_." 
	void smart(const char* set);
	///check if character is in bitfield
	bool check(char c) const;
	///add character to the bitfield
	void set(char c);
	///remove character from the list
	void remove(char c);
	///allow all characters
	void allow_all();
	///allow nothing
	void nothing();
};