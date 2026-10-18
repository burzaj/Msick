#pragma once

#include <string>
#include <vector>

namespace bb
{
namespace string
{

	/*===============| Modification |===============*/

	// returns "_strip_target" without the "_to_strip" character from fornt and back
	void strip(std::string& _strip_target, char _to_strip);
	std::string strip(const std::string& _strip_target, char _to_strip);

	// returns "_strip_target" without the "_to_strip" characters from fornt and back
	void strip(std::string& _strip_target, const std::string& _to_strip);
	std::string strip(const std::string& _strip_target, const std::string& _to_strip);


	// erases the "_to_erase" character from "_target"
	void erase(std::string& _target, char _to_erase);
	std::string erase(const std::string& _target, char _to_erase);

	// erases the "_to_erase" characters from "_target"
	void erase(std::string& _target, const std::string& _to_erase);
	std::string erase(const std::string& _target, const std::string& _to_erase);

	// erases the "_to_erase" substring from "_target"
	void erase_substr(std::string& _target, const std::string& _to_erase);
	std::string ererase_substrase(const std::string& _target, const std::string& _to_erase);


	// returns cleaned "_to_clean" (123.00000 -> 123) 
	// PS. check if "_to_clean" is a valid float before hand
	void clean_float(std::string& _to_clean);
	std::string clean_float(const std::string& _to_clean);


	// returns a vector of string separated by "_separator"
	std::vector<std::string> split(const std::string& _to_split, char _separator);

	// returns a vector of string separated by any char of "_separators"
	std::vector<std::string> split(const std::string& _to_split, const std::string& _separators);


	/*===============| Position |===============*/

	//returns the position of the first "_what" character
	size_t find_first(const std::string& _where, char _what);

	//returns the position of the first character from "_what"
	size_t find_first(const std::string& _where, const std::string& _what);

	//returns the position of the first character from "_what"
	size_t find_first_sequence(const std::string& _where, const std::string& _what);


	//returns the position of the first "_what" character
	size_t find_last(const std::string& _where, char _what);

	//returns the position of the first character from "_what"
	size_t find_last(const std::string& _where, const std::string& _what);

	//returns the position of the first character from "_what"
	size_t find_last_sequence(const std::string& _where, const std::string& _what);


	/*===============| Counting |===============*/

	// returns a number of instances of "_what" in "_where_to_count"
	size_t count(const std::string& _where_to_count, char _what);

	// returns a number of instances the characters in "_what" in "_where_to_count"
	size_t count(const std::string& _where_to_count, const std::string& _what);


	// returns a number of instances the characters in "_what" in "_where_to_count"
	size_t count_sequence(const std::string& _where_to_count, const std::string& _what);


	/*===============| Querry |===============*/

	// returns true if the "_to_check" is made of "_of"
	bool is_made_of(const std::string& _to_check, char _of);

	// returns true if the "_to_check" is made of "_of" characters
	bool is_made_of(const std::string& _to_check, const std::string& _of);


	// returns true if the "_to_check" contains a "_what"
	bool contains(const std::string& _to_check, char _what);

	// returns true if the "_to_check" contains characters of "_what"
	bool contains(const std::string& _to_check, const std::string& _what);


	// returns true if the "_to_check" contains a "_what" string sequence
	bool contains_sequence(const std::string& _to_check, const std::string& _what);

	// returns true if the "_to_check" contains a "_what" string sequence at "_at"
	bool contains_sequence(const std::string& _to_check, const std::string& _what, size_t _at);


	// returns true if "_to_check" is a valid arab numeral
	bool is_number(char _to_check);

	// returns true if "_to_check" is a valid arab number (float, int etc.)
	bool is_number(const std::string& _to_check);


	// returns true if "_to_check" is a valid HEX coded numeral
	bool is_hex(char _to_check);

	// returns true if "_to_check" is a valid HEX coded number
	bool is_hex(const std::string& _to_check);


	// returns true if "_to_check" is a valid float
	bool is_float(const std::string& _to_check);
	
	// returns true if "_to_check" is a valid int
	bool is_int(const std::string& _to_check);


	// returns true if the string coded path "_to_check" is structured correctly
	bool is_path(const std::string& _to_check);

	// returns true if the string coded path "_to_check" exists
	bool path_exists(const std::string& _to_check);



	/*===============| Hashing |===============*/

	//returns a hashed string
	size_t hash(const std::string& _to_hash);

} //namespace string
} //namespace bb