#pragma once

#include <filesystem>
#include "third_party/miniaudio.h"

#include <tuple>

class Music {
	ma_sound music;

	bool is_initialized = false;
	bool paused = false;

	std::string track_name;
public:
	Music(const std::filesystem::path& _music_path);
	Music() = default;

	void load(const std::filesystem::path& _music_path);

	void play();
	void pause();
	void change_volume(float _vol);
	
	std::tuple<double, double> get_progress();

	std::string get_name();

	bool has_ended();

	~Music();
};

