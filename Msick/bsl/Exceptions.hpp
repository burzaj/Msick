#pragma once

#include <stdexcept>
#include <string>

namespace bb
{
	namespace errid
	{
		inline const std::string Critical =   "[Critical] ";
		inline const std::string Manageable = "[Error]    ";
		inline const std::string LowLevel =   "[Bug]      ";
	}

	namespace exception
	{

		class File_not_found : public std::runtime_error {
		public:
			using std::runtime_error::runtime_error;
		};

		class File_could_not_be_opened : public std::runtime_error {
		public:
			using std::runtime_error::runtime_error;
		};

		class File_was_expected_to_be_opened_but_isnt : public std::runtime_error {
		public:
			using std::runtime_error::runtime_error;
		};

		class Invalid_path : public std::runtime_error {
		public:
			using std::runtime_error::runtime_error;
		};


		class Runtime : public std::runtime_error {
		public:
			using std::runtime_error::runtime_error;
		};


		class Iterator_out_of_range : public std::out_of_range {
		public:
			using std::out_of_range::out_of_range;
		};

		class Unexpected_empty : public std::logic_error {
		public:
			using std::logic_error::logic_error;
		};

		class Non_zero_value_is_zero : public std::logic_error {
		public:
			using std::logic_error::logic_error;
		};

		class Flipped_iterator : public std::out_of_range {
		public:
			using std::out_of_range::out_of_range;
		};

		class Size_mismatch : public std::logic_error {
		public:
			using std::logic_error::logic_error;
		};
	}
}