#pragma once

#include <string>
#include <filesystem>

namespace bb
{
	namespace log
	{
		// create the log at "_log_location"
		void init(const std::filesystem::path& _log_location);

		// clears the log, returns false if the process was not a success
		void clear();

		// logs the "_error_to_log", returns false if the process was not a success
		void error(const std::exception& _error_to_log);

		// logs the "_string_to_log", returns false if the process was not a success
		void string(const std::string& _string_to_log);

		// logs the "_string_to_log" with a timestamp, returns false if the process was not a success
		void timestamp(const std::string& _timestamp_label);

		// close the log
		void shutdown();

		// prints the current log to console
		void print_to_console();

		// returns the path to the current log
		std::filesystem::path get_path();
	}
}