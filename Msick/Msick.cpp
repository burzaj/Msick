#include "Msick.hpp"
#include "Music.hpp"

#include "bsl/String.hpp"
#include "bsl/File.hpp"

inline const olc::Pixel main_gui_col = olc::YELLOW;
inline const olc::Pixel secondary_gui_col = olc::DARK_YELLOW;

#if defined(_WIN32)
#include <windows.h>
#include <aclapi.h>
#include <lmcons.h>
// Note on Windows: Ensure you link against Advapi32.lib for security functions
#else
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#endif

namespace bb
{
	namespace file
	{
		enum class SortOrder
		{
			Ascending,
			Descending
		};

		// Helper to retrieve the file owner/creator username cross-platform
		inline std::string get_file_owner(const std::filesystem::path& _path)
		{
#if defined(_WIN32)
			PSID pSidOwner = nullptr;
			PSECURITY_DESCRIPTOR pSD = nullptr;
			std::string owner_name = "";

			if (GetNamedSecurityInfoW(
				_path.wstring().c_str(),
				SE_FILE_OBJECT,
				OWNER_SECURITY_INFORMATION,
				&pSidOwner,
				nullptr, nullptr, nullptr,
				&pSD) == ERROR_SUCCESS)
			{
				wchar_t name[UNLEN + 1];
				wchar_t domain[UNLEN + 1];
				DWORD name_len = UNLEN + 1;
				DWORD domain_len = UNLEN + 1;
				SID_NAME_USE sidType;

				if (LookupAccountSidW(nullptr, pSidOwner, name, &name_len, domain, &domain_len, &sidType)) {
					int size_needed = WideCharToMultiByte(CP_UTF8, 0, name, (int)wcslen(name), nullptr, 0, nullptr, nullptr);
					std::string str(size_needed, 0);
					WideCharToMultiByte(CP_UTF8, 0, name, (int)wcslen(name), &str[0], size_needed, nullptr, nullptr);
					owner_name = str;
				}
				if (pSD) LocalFree(pSD);
			}
			return owner_name.empty() ? "Unknown" : owner_name;
#else
			struct stat st;
			if (stat(_path.string().c_str(), &st) == 0) {
				struct passwd* pwd = getpwuid(st.st_uid);
				if (pwd && pwd->pw_name) {
					return std::string(pwd->pw_name);
				}
				return std::to_string(st.st_uid); // Fallback to numeric UID if mapping fails
			}
			return "Unknown";
#endif
		}

		// Sorts a vector of filepaths alphabetically by their creator/owner username
		inline void sort_filepaths_by_owner(std::vector<std::filesystem::path>& _filepaths, SortOrder _order = SortOrder::Ascending)
		{
			if (_filepaths.empty()) return;

			// 1. Decorate: Pair each path with its owner *once* to avoid expensive redundant OS calls
			std::vector<std::pair<std::filesystem::path, std::string>> decorated;
			decorated.reserve(_filepaths.size());

			for (const auto& path : _filepaths) {
				decorated.emplace_back(path, get_file_owner(path));
			}

			// 2. Sort the pre-computed pairs safely
			std::sort(decorated.begin(), decorated.end(), [_order](const auto& a, const auto& b) {
				if (_order == SortOrder::Ascending) {
					return a.second < b.second;
				}
				else {
					return a.second > b.second;
				}
				});

			// 3. Undecorate: Unpack the sorted paths back into the original vector
			for (size_t i = 0; i < _filepaths.size(); ++i) {
				_filepaths[i] = std::move(decorated[i].first);
			}
		}

	} // namespace file
} // namespace bb

Msick::Msick() {
}

bool Msick::OnUserCreate()
{
	std::srand(std::time(nullptr));

	msc.play();
	msc.change_volume(default_vol);
	sAppName = "Msick";
	return true;
}

bool Msick::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLACK);

	if (not playlist_loaded)
		LoadPlaylist(0);
	else
	{
		ProgressSong();
		DisplayName();
		DisplayProgress();
		HandleButtons();

		if (GetMouseWheel() != 0) {
			ChangeVol(GetMouseWheel()/120 * 0.005f);
		}
	}

	return true;
}

void Msick::Play() {
	if (shuffle) {
		size_t random_step = (std::rand() / playlist.size()) % playlist.size();
		curr_music_id += random_step;
	}
	else
		curr_music_id++;

	if (curr_music_id >= playlist.size())
		curr_music_id = 0;

	msc.load(playlist[curr_music_id]);
	msc.play();
	msc.change_volume(default_vol);
}

void Msick::ChangeVol(float vol)
{
	float new_vol = default_vol + vol;

	if (new_vol > 1.0f or new_vol < 0.0f)
		return;

	DrawString({ 64,144 / 2 }, "vol = " + bb::string::clean_float(std::to_string(default_vol)), main_gui_col);

	default_vol = new_vol;
	msc.change_volume(new_vol);
}

void Msick::GoBack() {
	curr_music_id -= 2;
	if (curr_music_id >= playlist.size()) {
		curr_music_id = playlist.size() - 2;
		if (curr_music_id >= playlist.size())
			curr_music_id = 0;
	}

	Play();
}

void Msick::Shuffle() {
	shuffle = !shuffle;
}

void Msick::DisplayName() {
	DrawString({ 5,1 }, ">> " + msc.get_name(), main_gui_col);

	DrawLine({ 0,10 }, { 256, 10 }, main_gui_col);
	//DrawStringDecal({ 5,5 }, msc.get_name(), olc::WHITE, { 0.5f,0.5f });
}

void Msick::DisplayProgress() {
	const size_t start = 64;

	auto prog = msc.get_progress();

	FillRect({ start,125 }, { (int)(std::get<0>(prog) * ((255-start) / std::get<1>(prog))), 144 }, secondary_gui_col);

	DrawString({ start+5,130 }, bb::string::clean_float(std::to_string((std::get<0>(prog)))) + '/' + bb::string::clean_float(std::to_string(round(std::get<1>(prog)))), main_gui_col);

	DrawLine({ start,125 }, { 256, 125 }, main_gui_col);
}

void Msick::Button(int start_w, int start_h, int w, int h, std::function<void()> foo) {
	if (GetMouseX() < start_w+w and GetMouseX() > start_w and GetMouseY() > start_h and GetMouseY() < start_h+h)
	{
		if (GetMouse(0).bPressed) {
			foo();
			FillRect({ start_w,start_h }, { w,h }, main_gui_col);
		}

		FillRect({ start_w,start_h }, { w,h }, secondary_gui_col);
	}
	else
		DrawRect({ start_w,start_h }, { w,h }, main_gui_col);
}



void Msick::HandleButtons() {
	const size_t w = 16;
	const size_t start = 125;
	Button(0, start, w, w, [this]() {this->msc.pause();});
	DrawTriangle({ 3,start + 3 }, { 3,start + w - 3 }, { w - 3, start + w / 2 }, main_gui_col);

	Button(w, start, w, w, [this]() {this->Play();});
	DrawTriangle({ 3+w,start + 3 }, { 3+w,start + w - 3 }, { w/2+w, start + w / 2 }, main_gui_col);
	DrawTriangle({ w / 2 + w,start + 3 }, { w / 2 + w,start + w - 3 }, { w - 3 + w, start + w / 2 }, main_gui_col);

	Button(w*2, start, w, w, [this]() {this->Shuffle();});
	DrawTriangle({ 3 + w * 2,start + 3 }, { 3 + w * 2,start + w - 3 }, { w / 2 + w * 2, start + w / 2 }, main_gui_col);
	DrawTriangle({ w - 3 + w * 2, start + w / 2 } ,{ w / 2 + w*2,start + 3 }, { w / 2 + w * 2,start + w - 3 }, main_gui_col);


	Button(w * 3, start, w, w, [this]() {this->GoBack();});
	DrawTriangle({ w * 3 + 3, start + w / 2 }, { w * 3 + w - 1, start + 3 }, { w * 3 + w - 1, start + w-3 }, main_gui_col);
}

void Msick::LoadPlaylist(size_t id) {
	static const std::filesystem::path config_filepath = "config.txt";

	std::vector<std::string> paths;

	bb::file::load_as_page(paths, config_filepath);

	if (paths.empty())
		throw bb::exception::Unexpected_empty(bb::errid::Manageable + " config file empty");

	if (paths.size() <= id)
		throw bb::exception::Unexpected_empty(bb::errid::Manageable + " playlist folder id > playlist amount");

	if (not std::filesystem::exists(paths[id]) or not std::filesystem::is_directory(paths[id]))
		throw bb::exception::Unexpected_empty(bb::errid::Manageable + " playlist foder path is invalid");

	playlist = bb::file::get_filepaths_in_folder(paths[id], ".mp3");

	if (!playlist.empty()) {

		bb::file::sort_filepaths_by_owner(playlist, bb::file::SortOrder::Ascending);

		curr_music_id = 0;
		Play();
	}
	else {
		throw bb::exception::Unexpected_empty(bb::errid::Manageable + " playlist foder empty / doen not contain any .mp3 files");
	}

	playlist_loaded = true;
}

void Msick::ProgressSong() {
	if (msc.has_ended()) {
		Play();
	}
}


