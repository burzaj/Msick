#include "Music.hpp"

#include "bsl/Exceptions.hpp"
#include "bsl/Log.hpp"
#include "bsl/String.hpp"
#include "bsl/Vector.hpp"

#include "third_party/miniaudio.h"


struct Engine {
	ma_engine engine;

	Engine() {
		ma_engine_config config = ma_engine_config_init();

		if (ma_engine_init(&config, &engine) != MA_SUCCESS) {
			throw bb::exception::Runtime(bb::errid::Critical + " Could not initialize the miniaudio engine");
		}
	}

	~Engine() {
		ma_engine_uninit(&engine);
	}

	// Bezpieczny dostęp gwarantujący inicjalizację w odpowiednim momencie
	static Engine& get() {
		static Engine instance;
		return instance;
	}
};

Music::Music(const std::filesystem::path& _music_path) {
	if (!std::filesystem::exists(_music_path)) {
		throw bb::exception::Runtime(bb::errid::Manageable + "  File does not exist");
	}

	//ma_result res = ma_engine_play_sound(&Engine::get().engine, _music_path.string().c_str(), nullptr);

	ma_result res = ma_sound_init_from_file(&Engine::get().engine, _music_path.string().c_str(), 0, nullptr, nullptr, &music);

	if (res != MA_SUCCESS) {
		std::string err_desc = ma_result_description(res);
		throw bb::exception::Runtime(bb::errid::Manageable + " Could not load a sound/music: " + err_desc);
	}

	is_initialized = true;
}

void Music::load(const std::filesystem::path& _music_path) {
	if (!std::filesystem::exists(_music_path)) {
		throw bb::exception::Runtime(bb::errid::Manageable + " File does not exist");
	}

	// Jeśli grał poprzedni utwór, najpierw go poprawnie posprzątaj
	if (is_initialized) {
		ma_sound_stop(&music);
		ma_sound_uninit(&music);
		is_initialized = false;
	}

	ma_result res = ma_sound_init_from_file(
		&Engine::get().engine,
		_music_path.string().c_str(),
		0,
		nullptr,
		nullptr,
		&music
	);

	if (res != MA_SUCCESS) {
		std::string err_desc = ma_result_description(res);
		throw bb::exception::Runtime(bb::errid::Manageable + " Could not load a sound/music: " + err_desc);
	}

	track_name = _music_path.filename().string();

	size_t cutoff = bb::string::find_first(track_name, '(');

	track_name = track_name.substr(0, cutoff - 1);

	is_initialized = true;
}

void Music::play() {
	ma_sound_start(&music);
	paused = false;
}

void Music::pause() {
	if (paused) {
		play();
		paused = false;
		return;
	}
	ma_sound_stop(&music);
	paused = true;
}

void Music::change_volume(float _vol) {
	ma_sound_set_volume(&music, _vol);
}

std::tuple<double, double> Music::get_progress()
{
	ma_uint64 currentFrame;
	ma_result result = ma_sound_get_cursor_in_pcm_frames(&music, &currentFrame);
	if (result != MA_SUCCESS) {
		throw bb::exception::Runtime(bb::errid::Critical + " Could not retrive the curr progress: ");
	}

	ma_uint64 totalFrames;
	result = ma_sound_get_length_in_pcm_frames(&music, &totalFrames);
	if (result != MA_SUCCESS) {
		throw bb::exception::Runtime(bb::errid::Critical + " Could not retrive the curr progress: ");
	}

	float progressPercentage = ((float)currentFrame / (float)totalFrames) * 100.0f;

	ma_uint32 sampleRate = ma_engine_get_sample_rate(&Engine::get().engine);
	double currentSeconds = (double)currentFrame / sampleRate;
	double totalSeconds = (double)totalFrames / sampleRate;

	return std::tuple<double, double>(currentSeconds, totalSeconds);
}

std::string Music::get_name()
{
	return std::string(track_name);
}

bool Music::has_ended() {
	return ma_sound_at_end(&music);
}

Music::~Music() {
	ma_sound_uninit(&music);
}
