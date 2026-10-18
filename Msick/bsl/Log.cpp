#include "Log.hpp"

#include <fstream>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <mutex>

#include "Exceptions.hpp"

namespace bb
{
    namespace log
    {
        static std::filesystem::path log_location = "log.txt";
        static std::ofstream log_file;
        static std::mutex log_mutex;
        static size_t log_count = 0;


        static void unguarded_clear()
        {
            if (log_file.is_open()) {
                log_file.close();
            }

            log_file.open(log_location, std::ios::out | std::ios::trunc);

            if (!log_file.is_open()) {
                throw bb::exception::File_could_not_be_opened(
                    bb::errid::Critical + "failed to clear and reopen log at " + log_location.string()
                );
            }
        }
    }
}



void bb::log::init(const std::filesystem::path& _log_location)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    log_count++;

    auto actual_path = _log_location.parent_path();

    if (!std::filesystem::exists(actual_path) and actual_path.string() != "")
            throw bb::exception::Invalid_path(bb::errid::Critical + "invalid log path");

    log_location = _log_location;

    unguarded_clear();

    auto curr_time = std::chrono::system_clock::now();

    log_file << "[" << curr_time << "][ log #" << log_count << " started ]" << std::endl;

    return;
}

void bb::log::clear()
{
    std::lock_guard<std::mutex> lock(log_mutex);

    unguarded_clear();

    return;
}

void bb::log::error(const std::exception& _error_to_log)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    if (!log_file.is_open()) {
        throw bb::exception::File_was_expected_to_be_opened_but_isnt(bb::errid::Critical + "file at" + log_location.string() + " could not be opened");
    }

    auto curr_time = std::chrono::system_clock::now();

    log_file << "[" << curr_time << "]" << _error_to_log.what() << std::endl;

    return;
}

void bb::log::string(const std::string& _string_to_log)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    if (!log_file.is_open()) {
        throw bb::exception::File_was_expected_to_be_opened_but_isnt(bb::errid::Critical + "file at" + log_location.string() + " could not be opened");
    }

    log_file << _string_to_log << std::endl;

    return;
}

void bb::log::timestamp(const std::string& _timestamp_label)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    if (!log_file.is_open()) {
        throw bb::exception::File_was_expected_to_be_opened_but_isnt(bb::errid::Critical + "file at" + log_location.string() + " could not be opened");
    }

    auto curr_time = std::chrono::system_clock::now();

    log_file << "[" << curr_time << "]" << _timestamp_label << std::endl;

    return;
}

void bb::log::shutdown()
{
    std::lock_guard<std::mutex> lock(log_mutex);

    if (log_file.is_open())
        log_file.close();
}

void bb::log::print_to_console()
{
    std::lock_guard<std::mutex> lock(log_mutex);

    if (log_file.is_open()) {
        log_file.flush();
    }

    std::ifstream reader(log_location);

    if (!reader.is_open()) {
        throw bb::exception::File_was_expected_to_be_opened_but_isnt(
            bb::errid::Critical + "failed to open log file for console printing at " + log_location.string()
        );
    }

    std::cout << reader.rdbuf();
}

std::filesystem::path bb::log::get_path()
{
    std::lock_guard<std::mutex> lock(log_mutex);

    return std::filesystem::path(log_location);
}
