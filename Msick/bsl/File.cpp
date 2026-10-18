#include "File.hpp"

#include <fstream>

#include "Exceptions.hpp"



std::vector<std::filesystem::path> bb::file::get_filepaths_in_folder(const std::filesystem::path& _folder_path, const std::string& _extension_filter)
{
    std::vector<std::filesystem::path> file_paths;

    // Use an error_code overload to safely handle non-existent folders or permission errors without throwing exceptions
    std::error_code ec;

    if (std::filesystem::exists(_folder_path, ec) && std::filesystem::is_directory(_folder_path, ec)) {
        for (const auto& entry : std::filesystem::directory_iterator(_folder_path, ec)) {
            if (ec) {
                break; // Stop iteration if an error occurs mid-way
            }

            if (entry.is_regular_file()) {
                if (_extension_filter == "" or entry.path().extension() == _extension_filter)
                    file_paths.push_back(entry.path());
            }
        }
    }

    return file_paths;
}

void bb::file::load_as_binary(std::vector<char>& _target_location, const std::filesystem::path& _path)
{
    if (!std::filesystem::exists(_path)) {
        throw bb::exception::Invalid_path(bb::errid::Critical + "path " + _path.string() + " is invalid");
    }

    std::ifstream file(_path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        throw bb::exception::File_could_not_be_opened(
            bb::errid::Critical + "file at " + _path.string() + " could not be opened"
        );
    }

    std::streamsize file_size = file.tellg();

    _target_location.resize(static_cast<size_t>(file_size));

    file.seekg(0, std::ios::beg);

    if (file_size > 0) {
        file.read(_target_location.data(), file_size);
    }
}

void bb::file::load_as_string(std::string& _target_location, const std::filesystem::path& _path)
{
    if (!std::filesystem::exists(_path)) {
        throw bb::exception::Invalid_path(bb::errid::Critical + "path " + _path.string() + " is invalid");
    }

    std::ifstream file(_path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        throw bb::exception::File_could_not_be_opened(
            bb::errid::Critical + "file at " + _path.string() + " could not be opened"
        );
    }

    std::streamsize file_size = file.tellg();

    _target_location.resize(static_cast<size_t>(file_size));

    file.seekg(0, std::ios::beg);

    if (file_size > 0) {
        file.read(_target_location.data(), file_size);
    }
}

std::string bb::file::load_as_string(const std::filesystem::path& _path)
{
    std::string new_str;
    load_as_string(new_str, _path);
    return new_str;
}

void bb::file::load_as_page(std::vector<std::string>& _target_location, const std::filesystem::path& _path)
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

        _target_location.push_back(current_line);
    }
}

void bb::file::save_from_binary(std::vector<char>& _data_to_save, const std::filesystem::path& _path)
{
    auto parent_path = _path.parent_path();
    if (!std::filesystem::exists(parent_path) and parent_path.string() != "") {
        throw bb::exception::Invalid_path(bb::errid::Critical + "path " + _path.string() + " is invalid");
    }
    
    std::ofstream file(_path, std::ios::binary);

    if (!file.is_open()) {
        throw bb::exception::File_could_not_be_opened(
            bb::errid::Critical + "file at " + _path.string() + " could not be opened or created"
        );
    }
    
    if (!_data_to_save.empty()) {
        file.write(_data_to_save.data(), _data_to_save.size());
    }
}

void bb::file::save_from_string(const std::string& _data_to_save, const std::filesystem::path& _path)
{
    auto parent_path = _path.parent_path();
    if (!std::filesystem::exists(parent_path) and parent_path.string() != "") {
        throw bb::exception::Invalid_path(bb::errid::Critical + "path " + _path.string() + " is invalid");
    }

    std::ofstream file(_path, std::ios::binary);

    if (!file.is_open()) {
        throw bb::exception::File_could_not_be_opened(
            bb::errid::Critical + "file at " + _path.string() + " could not be opened"
        );
    }

    if (!_data_to_save.empty()) {
        file.write(_data_to_save.data(), _data_to_save.size());
    }
}

void bb::file::save_from_page(const std::vector<std::string>& _data_to_save, const std::filesystem::path& _path)
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
        file << element << "\n";
    }
}
