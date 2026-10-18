#pragma once

#include <vector>
#include <string>

#include "third_party/olcPixelGameEngine.h"

#include "Music.hpp"

class Msick : public olc::PixelGameEngine {
	Music msc;

	std::vector<std::filesystem::path> playlist;

	bool playlist_loaded = false;
	bool shuffle = false;

	float default_vol = 0.001f;

	size_t curr_music_id = 0;
public:
	Msick();

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;


	void Play();

	void ChangeVol(float vol);

	void GoBack();

	void Shuffle();

	void DisplayName();
	void DisplayProgress();
	void Button(int start_w, int start_h, int w, int h, std::function<void()> foo);
	void HandleButtons();

	void LoadPlaylist(size_t id);

	void ProgressSong();
};

