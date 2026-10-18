#include <vector>

#include "Exceptions.hpp"
#include <utility> 
#include <cstddef> 
#include <algorithm>
#include <ranges>

namespace bb
{
namespace vector
{

	/*===============| Extraction |===============*/

    // returns a vector<T> where the elements are taken from "_source"["_pos" : "_pos" + "_length"]
    // if the "_length" is to big to fit insinde the "_source" its clamped to the end of "_source"
    template <std::ranges::random_access_range Container>
    Container subvec(const Container& _source, size_t _pos, size_t _length)
    {
        if (_pos >= _source.size())
        {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_pos out of range");
        }

        if (_length == 0) {
            throw bb::exception::Non_zero_value_is_zero(bb::errid::Manageable + "_length cannot be 0");
        }

        if (_pos + _length > _source.size()) {
            _length = _source.size() - _pos;
        }

        auto first = _source.begin() + _pos;
        auto last = first + _length;

        return Container(first, last);
    }

    // returns a vector<T> where the elements are taken from "_source"["_pos" : "_end_pos"]
    // if the "_end_pos" is to big to fit insinde the "_source" its clamped to the end of "_source"
    template <std::ranges::random_access_range Container>
    Container subvec_pos(const Container& _source, size_t _pos, size_t _end_pos)
    {
        if (_pos >= _source.size())
        {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_pos out of range");
        }

        if (_end_pos == _pos) {
            return Container();
        }

        if (_end_pos < _pos)
        {
            throw bb::exception::Flipped_iterator(bb::errid::Manageable + "wrong _end_pos value");
        }

        if (_end_pos > _source.size()) {
            _end_pos = _source.size();
        }

        auto first = _source.begin() + _pos;
        auto last = _source.begin() + _end_pos;

        return Container(first, last);
    }


    /*===============| Moving |===============*/

    // moves a T taken from "_from_pos" to "_to_pos" ie. [0,1,2,3,4] -> [0,2,3,1,4]
    template <std::ranges::random_access_range Container>
    void move(Container& _source, size_t _from_pos, size_t _to_pos)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_from_pos >= _source.size() || _to_pos >= _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_to_pos or _from_pos out of range");
        }

        if (_from_pos == _to_pos) return;

        auto first = _source.begin() + _from_pos;
        auto target = _source.begin() + _to_pos;

        if (_from_pos < _to_pos) {
            std::rotate(first, first + 1, target + 1);
        }
        else {
            std::rotate(target, first, first + 1);
        }
    }

    // moves a T taken from "_from_pos" by "_change" ie. [0,1,2,3,4] -> [0,2,3,1,4]
    template <std::ranges::random_access_range Container>
    void move_relative(Container& _source, size_t _from_pos, std::ptrdiff_t _change)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_from_pos >= _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_from_pos out of range");
        }

        if (_change == 0) return;

        std::ptrdiff_t target_pos = static_cast<std::ptrdiff_t>(_from_pos) + _change;

        if (target_pos < 0 || target_pos >= static_cast<std::ptrdiff_t>(_source.size())) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_change moves target out of range");
        }

        auto first = _source.begin() + _from_pos;
        auto target = _source.begin() + target_pos;

        if (_change > 0) {
            std::rotate(first, first + 1, target + 1);
        }
        else {
            std::rotate(target, first, first + 1);
        }
    }

    // moves the _source.subvec(_subvec_start, _subvec_length) to _to
    template <std::ranges::random_access_range Container>
    void move_subvec(Container& _source, size_t _subvec_start, size_t _subvec_length, size_t _to)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_subvec_start >= _source.size() || _subvec_start + _subvec_length > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_source sub-vector range out of bounds");
        }

        if (_to + _subvec_length > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "target subvector position out of range");
        }

        if (_subvec_length == 0 || _to == _subvec_start) {
            return;
        }

        if (_to < _subvec_start) {
            auto first = _source.begin() + _to;
            auto middle = _source.begin() + _subvec_start;
            auto last = middle + _subvec_length;

            std::rotate(first, middle, last);
        }
        else {
            auto first = _source.begin() + _subvec_start;
            auto middle = first + _subvec_length;
            auto last = _source.begin() + _to + _subvec_length;

            std::rotate(first, middle, last);
        }
    }

    // moves the _source.subvec(_subvec_start, _subvec_end) to _to
    template <std::ranges::random_access_range Container>
    void move_subvec_pos(Container& _source, size_t _subvec_start, size_t _subvec_end, size_t _to)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_subvec_start >= _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_subvec_start out of range");
        }

        if (_subvec_end < _subvec_start) {
            throw bb::exception::Flipped_iterator(bb::errid::Manageable + "_subvec_end cannot be less than _subvec_start");
        }

        if (_subvec_end > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_subvec_end out of range");
        }

        size_t subvec_length = _subvec_end - _subvec_start;

        if (_to + subvec_length > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "'_to' out of range");
        }

        if (subvec_length == 0 || _to == _subvec_start) {
            return;
        }

        if (_to < _subvec_start) {
            auto first = _source.begin() + _to;
            auto middle = _source.begin() + _subvec_start;
            auto last = _source.begin() + _subvec_end;

            std::rotate(first, middle, last);
        }
        else {
            auto first = _source.begin() + _subvec_start;
            auto middle = _source.begin() + _subvec_end;
            auto last = _source.begin() + _to + subvec_length;

            std::rotate(first, middle, last);
        }
    }

    // moves the _source.subvec(_subvec_start, _subvec_end) by _change
    template <std::ranges::random_access_range Container>
    void move_relative_subvec_pos(Container& _source, size_t _subvec_start, size_t _subvec_end, std::ptrdiff_t _change)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_subvec_start >= _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_subvec_start out of range");
        }

        if (_subvec_end < _subvec_start) {
            throw bb::exception::Flipped_iterator(bb::errid::Manageable + "_subvec_end cannot be less than _subvec_start");
        }

        if (_subvec_end > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_subvec_end out of range");
        }

        if (_change == 0) return;

        size_t subvec_length = _subvec_end - _subvec_start;
        if (subvec_length == 0) return;

        std::ptrdiff_t target_pos = static_cast<std::ptrdiff_t>(_subvec_start) + _change;

        if (target_pos < 0 || static_cast<size_t>(target_pos) + subvec_length > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "relative shift moves sub-vector out of bounds");
        }

        if (target_pos < static_cast<std::ptrdiff_t>(_subvec_start)) {
            auto first = _source.begin() + target_pos;
            auto middle = _source.begin() + _subvec_start;
            auto last = _source.begin() + _subvec_end;

            std::rotate(first, middle, last);
        }
        else {
            auto first = _source.begin() + _subvec_start;
            auto middle = _source.begin() + _subvec_end;
            auto last = _source.begin() + target_pos + subvec_length;

            std::rotate(first, middle, last);
        }
    }

    // moves the _source.subvec(_subvec_start, _subvec_length) by _change
    template <std::ranges::random_access_range Container>
    void move_relative_subvec(Container& _source, size_t _subvec_start, size_t _subvec_length, std::ptrdiff_t _change)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_subvec_start >= _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_subvec_start out of range");
        }

        if (_subvec_length > _source.size() - _subvec_start) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_subvec_length extends past container bounds");
        }

        if (_change == 0 || _subvec_length == 0) return;

        size_t subvec_end = _subvec_start + _subvec_length;

        std::ptrdiff_t target_pos = static_cast<std::ptrdiff_t>(_subvec_start) + _change;

        if (target_pos < 0 || static_cast<size_t>(target_pos) + _subvec_length > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "relative shift moves sub-vector out of bounds");
        }

        if (target_pos < static_cast<std::ptrdiff_t>(_subvec_start)) {
            // Moving Left (Backward)
            auto first = _source.begin() + target_pos;
            auto middle = _source.begin() + _subvec_start;
            auto last = _source.begin() + subvec_end;

            std::rotate(first, middle, last);
        }
        else {
            // Moving Right (Forward)
            auto first = _source.begin() + _subvec_start;
            auto middle = _source.begin() + subvec_end;
            auto last = _source.begin() + target_pos + _subvec_length;

            std::rotate(first, middle, last);
        }
    }


    /*===============| Swaping |===============*/

    // swap elements form "_A" and "_B" ie. [0,1,2,3,4] -> [0,3,2,1,4]
    template <std::ranges::random_access_range Container>
    void swap(Container& _source, size_t _A, size_t _B)
    {
        if (_A >= _source.size() || _B >= _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_A or _B out of range");
        }

        using std::swap;
        swap(_source[_A], _source[_B]);
    }

    // swap elements form "_A" and "_A" + "_change" ie. [0,1,2,3,4] -> [0,3,2,1,4]
    template <std::ranges::random_access_range Container>
    void swap_relative(Container& _source, size_t _A, std::ptrdiff_t _change)
    {
        // 1. Check the starting point first
        if (_A >= _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_A out of range");
        }

        if (_change == 0) return;

        std::ptrdiff_t target_pos = static_cast<std::ptrdiff_t>(_A) + _change;

        if (target_pos < 0 || target_pos >= static_cast<std::ptrdiff_t>(_source.size())) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_change moves target out of range");
        }
        using std::swap;
        swap(_source[_A], _source[target_pos]);
    }

    // swaps the subvecs A and B of length _length_of
    template <std::ranges::random_access_range Container>
    void swap_subvec(Container& _source, size_t _first_of_A, size_t _length_of, size_t _first_of_B)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_first_of_A >= _source.size() || _length_of > _source.size() - _first_of_A) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "subvec A range out of bounds");
        }

        if (_first_of_B >= _source.size() || _length_of > _source.size() - _first_of_B) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "subvec B range out of bounds");
        }

        if (_length_of == 0 || _first_of_A == _first_of_B) {
            return;
        }

        size_t end_of_A = _first_of_A + _length_of;
        size_t end_of_B = _first_of_B + _length_of;

        if (_first_of_A < end_of_B && _first_of_B < end_of_A) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "overlapping sub-vectors");
        }

        auto first1 = _source.begin() + _first_of_A;
        auto last1 = first1 + _length_of;
        auto first2 = _source.begin() + _first_of_B;

        std::swap_ranges(first1, last1, first2);
    }

    // swaps subvecs _first_of_A _last_of_A)
    template <std::ranges::random_access_range Container>
    void swap_subvec_pos(Container& _source, size_t _first_of_A, size_t _last_of_A, size_t _first_of_B)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_first_of_A > _last_of_A) {
            throw bb::exception::Flipped_iterator(bb::errid::Manageable + "_first_of_A cannot be larger than _last_of_A");
        }

        if (_last_of_A > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "Subvec A range out of bounds");
        }

        size_t _length_of = _last_of_A - _first_of_A;
        if (_length_of == 0 || _first_of_A == _first_of_B) {
            return;
        }

        if (_first_of_B >= _source.size() || _length_of > _source.size() - _first_of_B) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "Subvec B range out of bounds");
        }

        size_t end_of_B = _first_of_B + _length_of;
        if (_first_of_A < end_of_B && _first_of_B < _last_of_A) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "Swapping overlapping sub-vectors is invalid");
        }

        auto first1 = _source.begin() + _first_of_A;
        auto last1 = _source.begin() + _last_of_A;
        auto first2 = _source.begin() + _first_of_B;

        std::swap_ranges(first1, last1, first2);
    }

    // swaps a subvector A with a subvector B at relative _change
    template <std::ranges::random_access_range Container>
    void swap_relative_subvec_pos(Container& _source, size_t _first_of_A, size_t _last_of_A, std::ptrdiff_t _change)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_first_of_A > _last_of_A) {
            throw bb::exception::Flipped_iterator(bb::errid::Manageable + "_first_of_A cannot be larger than _last_of_A");
        }

        if (_last_of_A > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "Subvec A range out of bounds");
        }

        size_t length = _last_of_A - _first_of_A;
        if (length == 0 || _change == 0) {
            return;
        }

        std::ptrdiff_t target_pos = static_cast<std::ptrdiff_t>(_first_of_A) + _change;

        if (target_pos < 0 || static_cast<size_t>(target_pos) + length > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "Relative shift places subvec B out of bounds");
        }

        size_t first_of_B = static_cast<size_t>(target_pos);
        size_t last_of_B = first_of_B + length;

        if (_first_of_A < last_of_B && first_of_B < _last_of_A) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "Swapping overlapping sub-vectors is invalid");
        }

        auto first1 = _source.begin() + _first_of_A;
        auto last1 = _source.begin() + _last_of_A;
        auto first2 = _source.begin() + first_of_B;

        std::swap_ranges(first1, last1, first2);
    }

    // swaps a subvector A with a subvector B at relative _change
    template <std::ranges::random_access_range Container>
    void swap_relative_subvec(Container& _source, size_t _first_of_A, size_t _length, std::ptrdiff_t _change)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_first_of_A >= _source.size() || _length > _source.size() - _first_of_A) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "subvec A range out of bounds");
        }

        if (_length == 0 || _change == 0) {
            return;
        }

        std::ptrdiff_t target_pos = static_cast<std::ptrdiff_t>(_first_of_A) + _change;

        if (target_pos < 0 || static_cast<size_t>(target_pos) + _length > _source.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "relative shift places subvec B out of bounds");
        }

        size_t first_of_B = static_cast<size_t>(target_pos);
        size_t end_of_A = _first_of_A + _length;
        size_t end_of_B = first_of_B + _length;

        if (_first_of_A < end_of_B && first_of_B < end_of_A) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "swapping overlapping sub-vectors is invalid");
        }

        auto first1 = _source.begin() + _first_of_A;
        auto last1 = first1 + _length;
        auto first2 = _source.begin() + first_of_B;

        std::swap_ranges(first1, last1, first2);
    }


    /*===============| Swaping unequal subvectors |===============*/

    // swaps two vectors of unequal size
    template <std::ranges::random_access_range Container>
    void swap_unequal_subvec(Container& _source, size_t _first_of_A, size_t _length_A, size_t _first_of_B, size_t _length_B)
    {
        if (_source.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_source cannot be empty");
        }

        if (_first_of_A >= _source.size() || _length_A > _source.size() - _first_of_A) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "subvec A out of bounds");
        }

        if (_first_of_B >= _source.size() || _length_B > _source.size() - _first_of_B) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "subvec B out of bounds");
        }

        if (_first_of_A == _first_of_B) {
            if (_length_A != _length_B) {
                throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "cannot swap different lengths at the exact same position");
            }
            return;
        }

        size_t first_L, length_L, first_R, length_R;
        if (_first_of_A < _first_of_B) {
            first_L = _first_of_A; length_L = _length_A;
            first_R = _first_of_B; length_R = _length_B;
        }
        else {
            first_L = _first_of_B; length_L = _length_B;
            first_R = _first_of_A; length_R = _length_A;
        }

        if (first_L + length_L > first_R) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "swapping overlapping unequal sub-vectors is invalid");
        }

        if (length_L == 0 && length_R == 0) return;

        auto total_first = _source.begin() + first_L;
        auto total_middle = _source.begin() + first_R;
        auto total_last = _source.begin() + first_R + length_R;

        std::rotate(total_first, total_middle, total_last);

        auto second_first = total_first + length_R;
        auto second_middle = second_first + length_L;
        auto second_last = total_last;

        std::rotate(second_first, second_middle, second_last);
    }

    // swaps two vectors at _first_of_X and _last_of_X of unequal size
    template <std::ranges::random_access_range Container>
    void swap_unequal_subvec_pos(Container& _source, size_t _first_of_A, size_t _last_of_A, size_t _first_of_B, size_t _last_of_B)
    {
        if (_last_of_A < _first_of_A or _last_of_B < _first_of_B)
        {
            throw bb::exception::Flipped_iterator(bb::errid::Manageable + "_last_of_X cannot be smaller than _first_of_X");
        }

        bb::vector::swap_unequal_subvec(_source, _first_of_A, _last_of_A - _first_of_A, _first_of_B, _last_of_B - _first_of_B);
    }


    /*===============| Erasing |===============*/

    // removes an element form "_target" at "_where"
    template <std::ranges::random_access_range Container>
    void erase(Container& _target, size_t _where)
    {
        if (_target.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_target cannot be empty");
        }

        if (_where >= _target.size()) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_where index out of range");
        }

        auto it = _target.begin() + _where;

        _target.erase(it);
    }

    // removes a subvec from "_target" at "_at" to "_length"
    template <std::ranges::random_access_range Container>
    void erase_subvec(Container& _target, size_t _at, size_t _length)
    {
        if (_target.empty()) {
            throw bb::exception::Unexpected_empty(bb::errid::Critical + "_target cannot be empty");
        }

        if (_at > _target.size() || _length > _target.size() - _at) {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "erasure range extends out of bounds");
        }

        if (_length == 0) {
            return;
        }

        auto begin = _target.begin() + _at;
        auto end = begin + _length;

        _target.erase(begin, end);
    }

    // removes elements from "_target" at "_begin" to "_end"
    template <std::ranges::random_access_range Container>
    void erase_subvec_pos(Container& _target, size_t _begin, size_t _end)
    {
        if (_end < _begin)
        {
            throw bb::exception::Flipped_iterator(bb::errid::Manageable + "_end cannot be smaller than _begin");
        }

        if (_end > _target.size())
        {
            throw bb::exception::Iterator_out_of_range(bb::errid::Critical + "_end iterator out of range");
        }

        erase_subvec(_target, _begin, _end - _begin);
    }


    /*===============| Spliting and Dividing |===============*/

    // splits a Container by separator (separoator is lost)
    template <std::ranges::random_access_range Container, typename T = std::ranges::range_value_t<Container>>
    std::vector<Container> split(const Container& _to_split, const T& _separator)
    {
        std::vector<Container> chunks;

        if (_to_split.empty()) {
            return chunks;
        }

        auto it = _to_split.begin();
        auto end = _to_split.end();

        while (it != end) {
            auto sep_it = std::find(it, end, _separator);

            chunks.emplace_back(it, sep_it);

            if (sep_it == end) {
                break;
            }

            it = sep_it + 1;

            if (it == end) {
                chunks.emplace_back();
            }
        }

        return chunks;
    }

    // splits a Container by separator list (separoators are lost)
    template <std::ranges::random_access_range Container>
    std::vector<Container> split(const Container& _to_split, const Container& _separators)
    {
        std::vector<Container> chunks;

        if (_to_split.empty()) {
            return chunks;
        }

        auto it = _to_split.begin();
        auto end = _to_split.end();

        while (it != end) {
            auto sep_it = std::find_first_of(it, end, _separators.begin(), _separators.end());

            chunks.emplace_back(it, sep_it);

            if (sep_it == end) {
                break;
            }

            it = sep_it + 1;

            if (it == end) {
                chunks.emplace_back();
            }
        }

        return chunks;
    }

    // divides a Container into subvecs by separator (separoator is kept at the begining of the subvecs)
    template <std::ranges::random_access_range Container, typename T = std::ranges::range_value_t<Container>>
    std::vector<Container> divide_begin(const Container& _target, const T& _separator)
    {
        std::vector<Container> chunks;

        if (_target.empty()) {
            return chunks;
        }

        auto it = _target.begin();
        auto end = _target.end();

        auto first_sep = std::find(it, end, _separator);
        chunks.emplace_back(it, first_sep);

        it = first_sep;

        while (it != end) {
            auto next_sep = std::find(it + 1, end, _separator);

            chunks.emplace_back(it, next_sep);

            it = next_sep;
        }

        return chunks;
    }

    // divides a Container into subvecs by separator (separoator is kept at the end of the subvecs)
    template <std::ranges::random_access_range Container, typename T = std::ranges::range_value_t<Container>>
    std::vector<Container> divide_end(const Container& _target, const T& _separator)
    {
        std::vector<Container> chunks;

        if (_target.empty()) {
            return chunks;
        }

        auto it = _target.begin();
        auto end = _target.end();

        while (it != end) {
            auto sep_it = std::find(it, end, _separator);

            if (sep_it != end) {
                chunks.emplace_back(it, sep_it + 1);
                it = sep_it + 1;
            }
            else {
                chunks.emplace_back(it, end);
                it = end;
            }
        }

        return chunks;
    }

} //namespace vector
} //namespace bb