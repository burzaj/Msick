#include "String.hpp"

#include <cctype>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

#include "Macros.hpp"
#include "Vector.hpp"


#pragma region Modification

void bb::string::strip(std::string& _strip_target, char _to_strip)
{
    size_t front_pos = _strip_target.find_first_not_of(_to_strip);

    if (front_pos == std::string::npos)
    {
        _strip_target.clear();
        return;
    }

    size_t last_valid_pos = _strip_target.find_last_not_of(_to_strip);

    _strip_target.erase(last_valid_pos + 1);

    _strip_target.erase(0, front_pos);
}

std::string bb::string::strip(const std::string& _strip_target, char _to_strip)
{
    std::string str = _strip_target;
    bb::string::strip(str, _to_strip);
    return str;
}

void bb::string::strip(std::string& _strip_target, const std::string& _to_strip)
{
    size_t front_pos = _strip_target.find_first_not_of(_to_strip);

    if (front_pos == std::string::npos)
    {
        _strip_target.clear();
        return;
    }

    size_t last_valid_pos = _strip_target.find_last_not_of(_to_strip);

    _strip_target.erase(last_valid_pos + 1);

    _strip_target.erase(0, front_pos);
}

std::string bb::string::strip(const std::string& _strip_target, const std::string& _to_strip)
{
    std::string str = _strip_target;
    bb::string::strip(str, _to_strip);
    return str;
}

void bb::string::erase(std::string& _target, char _to_erase)
{
    _target.erase(
        std::remove(_target.begin(), _target.end(), _to_erase),
        _target.end()
    );
}

std::string bb::string::erase(const std::string& _target, char _to_erase)
{
    std::string new_str = _target;
    erase(new_str, _to_erase);
    return new_str;
}

void bb::string::erase(std::string& _target, const std::string& _to_erase)
{
    if (_to_erase.empty()) {
        throw bb::exception::Unexpected_empty(bb::errid::Critical + "_to_erase cannot be empty");
    }

    for (auto to_erase : _to_erase)
    {
        erase(_target, to_erase);
    }
}

std::string bb::string::erase(const std::string& _target, const std::string& _to_erase)
{
    std::string new_str = _target;
    erase(new_str, _to_erase);
    return new_str;
}

void bb::string::erase_substr(std::string& _target, const std::string& _to_erase)
{
    if (_to_erase.empty()) {
        return;
    }

    size_t pos = _target.find(_to_erase);

    while (pos != std::string::npos) {
        _target.erase(pos, _to_erase.length());

        pos = _target.find(_to_erase, pos);
    }
}

std::string bb::string::ererase_substrase(const std::string& _target, const std::string& _to_erase)
{
    std::string new_str = _target;
    erase_substr(new_str, _to_erase);
    return new_str;
}


void bb::string::clean_float(std::string& _to_clean)
{
    if (_to_clean.empty()) {
        return;
    }

    size_t dot_pos = _to_clean.find('.');
    if (dot_pos != std::string::npos)
    {
        size_t back_pos = _to_clean.find_last_not_of('0');

        if (back_pos <= dot_pos) {
            _to_clean.erase(dot_pos);
        }
        else {
            _to_clean.erase(back_pos + 1);
        }
    }

    size_t start = 0;
    if (_to_clean[0] == '-' || _to_clean[0] == '+') {
        start = 1;
    }

    size_t first_non_zero = _to_clean.find_first_not_of('0', start);

    if (first_non_zero == std::string::npos) {
        _to_clean = "0";
        return;
    }

    if (_to_clean[first_non_zero] == '.') {
        if (first_non_zero > start) {
            _to_clean.erase(start, first_non_zero - start - 1);
        }
    }
    else {
        if (first_non_zero > start) {
            _to_clean.erase(start, first_non_zero - start);
        }
    }
}

std::string bb::string::clean_float(const std::string& _to_clean)
{
    std::string new_string = _to_clean;
    clean_float(new_string);
    return new_string;
}


std::vector<std::string> bb::string::split(const std::string& _to_split, char _separator)
{
    return bb::vector::split(_to_split, _separator);
}



std::vector<std::string> bb::string::split(const std::string& _to_split, const std::string& _separators)
{
    return bb::vector::split(_to_split, _separators);
}

#pragma endregion

#pragma region Position

size_t bb::string::find_first(const std::string& _where, char _what)
{
    return _where.find_first_of(_what);
}



size_t bb::string::find_first(const std::string& _where, const std::string& _what)
{
    return _where.find_first_of(_what);
}



size_t bb::string::find_first_sequence(const std::string& _where, const std::string& _what)
{
    return _where.find(_what);
}



size_t bb::string::find_last(const std::string& _where, char _what)
{
    return _where.find_last_of(_what);
}



size_t bb::string::find_last(const std::string& _where, const std::string& _what)
{
    return _where.find_last_of(_what);
}



size_t bb::string::find_last_sequence(const std::string& _where, const std::string& _what)
{
    return _where.rfind(_what);
}


#pragma endregion

#pragma region Counting


size_t bb::string::count(const std::string& _where_to_count, char _what)
{
    return std::count(_where_to_count.begin(), _where_to_count.end(), _what);
}



size_t bb::string::count(const std::string& _where_to_count, const std::string& _what)
{
    size_t _count = 0;

    for (auto _char : _what)
        _count += count(_where_to_count, _char);

    return _count;
}



size_t bb::string::count_sequence(const std::string& _where_to_count, const std::string& _what)
{
    if (_what.empty() || _where_to_count.empty()) {
        return 0;
    }

    size_t count = 0;
    size_t pos = _where_to_count.find(_what, 0);

    while (pos != std::string::npos)
    {
        count++;
        pos = _where_to_count.find(_what, pos + _what.size());
    }

    return count;
}

#pragma endregion

#pragma region Querry


bool bb::string::is_made_of(const std::string& _to_check, char _of)
{
    if (_to_check.empty()) return false;

    for (auto checking : _to_check)
    {
        if (_of != checking)
            return false;
    }

    return true;
}



bool bb::string::is_made_of(const std::string& _to_check, const std::string& _of)
{
    if (_to_check.empty()) return false;

    for (auto checking : _to_check)
    {
        if (_of.find(checking) == std::string::npos)
            return false;
    }

    return true;
}



bool bb::string::contains(const std::string& _to_check, char _what)
{
    return _to_check.find(_what) != std::string::npos;
}



bool bb::string::contains(const std::string& _to_check, const std::string& _what)
{
    return _to_check.find_first_of(_what) != std::string::npos;
}



bool bb::string::contains_sequence(const std::string& _to_check, const std::string& _what)
{
    return _to_check.find(_what) != std::string::npos;
}



bool bb::string::contains_sequence(const std::string& _to_check, const std::string& _what, size_t _at)
{
    return _to_check.find(_what, _at) != std::string::npos;
}



bool bb::string::is_number(char _to_check)
{
    return std::isdigit(static_cast<unsigned char>(_to_check));
}



bool bb::string::is_number(const std::string& _to_check)
{
    if (_to_check.empty() || _to_check == "-") return false;

    size_t front_dot_pos = _to_check.find('.'),
        minus_pos = _to_check.find('-');

    return is_made_of(_to_check, bb::consistance::number)
        and front_dot_pos == _to_check.rfind('.')
        and front_dot_pos >= (minus_pos == 0 ? 2 : 1)
        and front_dot_pos != _to_check.size() - 1
        and (minus_pos == 0 or minus_pos == std::string::npos)
        and minus_pos == _to_check.rfind('-');
}



bool bb::string::is_hex(char _to_check)
{
    return contains(bb::consistance::hex, _to_check);
}



bool bb::string::is_hex(const std::string& _to_check)
{
    return is_made_of(_to_check, bb::consistance::hex);
}



bool bb::string::is_float(const std::string& _to_check)
{
    if (_to_check.empty() || _to_check == "-") return false;

    size_t front_dot_pos = _to_check.find('.'),
        minus_pos = _to_check.find('-');

    return is_made_of(_to_check, bb::consistance::number)
        and front_dot_pos == _to_check.rfind('.')
        and front_dot_pos >= (minus_pos == 0 ? 2 : 1)
        and front_dot_pos != std::string::npos
        and front_dot_pos != _to_check.size() - 1
        and (minus_pos == 0 or minus_pos == std::string::npos)
        and minus_pos == _to_check.rfind('-');
}



bool bb::string::is_int(const std::string& _to_check)
{
    if (_to_check.empty() || _to_check == "-") return false;

    size_t front_dot_pos = _to_check.find('.'),
        minus_pos = _to_check.find('-');

    return is_made_of(_to_check, bb::consistance::number)
        and front_dot_pos == std::string::npos
        and (minus_pos == 0 or minus_pos == std::string::npos)
        and minus_pos == _to_check.rfind('-');
}



bool bb::string::is_path(const std::string& _to_check)
{
    if (_to_check.empty()) return false;

    if (_to_check.size() > 4096) return false;

    size_t colon_pos = _to_check.find(':');
    if (colon_pos != std::string::npos && colon_pos != 1) {
        return false;
    }

    if (_to_check.find_first_of("<>\"|?*") != std::string::npos) {
        return false;
    }

    for (char c : _to_check) {
        if (static_cast<unsigned char>(c) < 32) {
            return false;
        }
    }

    return true;
}



bool bb::string::path_exists(const std::string& _to_check)
{
    return std::filesystem::exists(std::filesystem::path(_to_check));
}



size_t bb::string::hash(const std::string& _to_hash)
{
    return std::hash<std::string>{}(_to_hash);
}

#pragma endregion
