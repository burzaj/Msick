#pragma once

#include <vector>
#include <filesystem>
#include <functional>
#include <ranges>

#include "Exceptions.hpp"

namespace bb
{
namespace file
{

    // abstract class template for creating parsers for loading data
    template <typename T>
    class Parser
    {
    public:
        virtual T get() = 0;
        virtual bool parse(const std::string& _line) = 0;

        virtual ~Parser() = default;
    };

    // abstract class template for creating parsers for saving data
    template <typename T>
    class UnParser
    {
    public:
        virtual std::string get() = 0;
        virtual bool parse(const T& _elemnt) = 0;

        virtual ~UnParser() = default;
    };



    /*===============| Reading |===============*/

    // returns a vector of filepaths to all the files in a folder
    std::vector<std::filesystem::path> get_filepaths_in_folder(const std::filesystem::path& _folder_path, const std::string& _extension_filter = "");

	// loads the raw bytes of the file at "_path" to "_target_location"
	void load_as_binary(std::vector<char>& _target_location, const std::filesystem::path& _path);

	// loads the file as one continus string at "_path" to "_target_location"
	// the _target_location will contain characters like \r\n so beware
	void load_as_string(std::string& _target_location, const std::filesystem::path& _path);
	std::string load_as_string(const std::filesystem::path& _path);

	// loads the file as individual lines at "_path" to "_target_location"
	void load_as_page(std::vector<std::string>& _target_location, const std::filesystem::path& _path);

	// load a file as "_line_parsing_function" parsed lines into the "_target_location"
	template <typename T, typename ParserFunc>
	void load_with_parsing_func(std::vector<T>& _target_location, ParserFunc _line_parsing_function, const std::filesystem::path& _path)
	{
        if (!std::filesystem::exists(_path)) {
            throw bb::exception::Invalid_path(bb::errid::Critical + "path " + _path.string() + " is invalid");
        }

        std::ifstream file(_path);

        if (!file.is_open()) {
            throw bb::exception::File_could_not_be_opened(
                bb::errid::Critical + "file at " + _path.string() + " could not be opened"
            );
        }

        _target_location.clear();

        std::string current_line;
        while (std::getline(file, current_line)) {

            if (!current_line.empty() && current_line.back() == '\r') {
                current_line.pop_back();
            }

            _target_location.push_back(_line_parsing_function(current_line));
        }
	}

    // load a file as "_line_parsing_object" parsed lines into the "_target_location"
    template <typename T>
    void load_with_parser_class(std::vector<T>& _target_location, Parser<T>& _line_parsing_object, const std::filesystem::path& _path)
    {
        if (!std::filesystem::exists(_path)) {
            throw bb::exception::Invalid_path(bb::errid::Critical + "path " + _path.string() + " is invalid");
        }

        std::ifstream file(_path);

        if (!file.is_open()) {
            throw bb::exception::File_could_not_be_opened(
                bb::errid::Critical + "file at " + _path.string() + " could not be opened"
            );
        }

        _target_location.clear();

        std::string current_line;
        while (std::getline(file, current_line)) {

            if (!current_line.empty() && current_line.back() == '\r') {
                current_line.pop_back();
            }

            if (_line_parsing_object.parse(current_line))
                _target_location.push_back(_line_parsing_object.get());
        }
    }


    /*===============| Sorting |================*/

    // sort the filepath list by crea
    void sort_filepaths_by_time(std::vector<std::filesystem::path>& _filepaths, bool ascending = true);

    /*===============| Saving |===============*/

    // create a new file at "_path" and fill it with the "_data_to_save"
    void save_from_binary(std::vector<char>& _data_to_save, const std::filesystem::path& _path);

    // create a new file at "_path" and fill it with the "_data_to_save"
    void save_from_string(const std::string& _data_to_save, const std::filesystem::path& _path);

    // create a new file at "_path" and fill it with the "_data_to_save"
    void save_from_page(const std::vector<std::string>& _data_to_save, const std::filesystem::path& _path);

    // create a new file at "_path" and fill it with the "_data_to_save" parsed with "_line_parsing_function"
    template <typename T, typename ParserFunc>
    void save_parsed_with_func(const std::vector<T>& _data_to_save, ParserFunc _line_parsing_function, const std::filesystem::path& _path)
    {
        auto parent_path = _path.parent_path();

        if (!std::filesystem::exists(parent_path) and parent_path.string() != "") {
            throw bb::exception::Invalid_path(bb::errid::Critical + "path " + _path.string() + " is invalid");
        }

        std::ofstream file(_path);

        if (!file.is_open()) {
            throw bb::exception::File_could_not_be_opened(
                bb::errid::Critical + "file at " + _path.string() + " could not be opened"
            );
        }
        
        for (const auto& element : _data_to_save)
        {
            file << _line_parsing_function(element) << "\n";
        }
        
    }

    // create a new file at "_path" and fill it with the "_data_to_save" parsed with "_line_parsing_obj"
    template <typename T>
    void save_parsed_with_class(const std::vector<T>& _data_to_save, UnParser<T>& _line_parsing_obj, const std::filesystem::path& _path)
    {
        auto parent_path = _path.parent_path();

        if (!std::filesystem::exists(parent_path) and parent_path.string() != "") {
            throw bb::exception::Invalid_path(bb::errid::Critical + "path " + _path.string() + " is invalid");
        }

        std::ofstream file(_path);

        if (!file.is_open()) {
            throw bb::exception::File_could_not_be_opened(
                bb::errid::Critical + "file at " + _path.string() + " could not be opened"
            );
        }

        for (const auto& element : _data_to_save)
        {
            if (_line_parsing_obj.parse(element))
                file << _line_parsing_obj.get() << "\n";
        }

    }

} // namespace file
} // namespace bb