#pragma once
#include <string>
#include <array>
#include <vector>
#include <map>
#include "common/types.h"
namespace i18nText
{
	enum i18nTextIndex
	{
		LANGUAGE_NAME,
		OFF,
		ON,
		OK,
		CANCEL,
		APPLY,
		ADD_MORE,
		MAIN_KEYCONFIG,
		MAIN_SETTINGS,
		MAIN_ABOUT,
		MAIN_EXIT,
		TODO,
		SETTINGS_GENERAL,
		SETTINGS_JUKEBOX,
		SETTINGS_VIDEO,
		SETTINGS_AUDIO,
		SETTINGS_PLAY,
		SETTINGS_ADVANCED,
		PROFILE,
		PLAYER_NAME,
		LANGUAGE,
		JUKEBOX_REFRESH_HINT,
		JUKEBOX_FOLDER,
		JUKEBOX_DELETE_SELECTED,
		JUKEBOX_BROWSE_SELECTED,
		JUKEBOX_TABLES,
		VIDEO_RESOLUTION,
		VIDEO_SS_LEVEL,
		VIDEO_SCREEN_MODE,
		VIDEO_WINDOWED,
		VIDEO_FULLSCREEN,
		VIDEO_BORDERLESS,
		VIDEO_VSYNC,
		VIDEO_ADAPTIVE,
		VIDEO_MAXFPS,
		AUDIO_DEVICE,
		AUDIO_REFRESH_DEVICE_LIST,
		AUDIO_BUFFER_COUNT,
		AUDIO_BUFFER_LENGTH,
		MISS_BGA_TIME,
		MIN_INPUT_INTERVAL,
		MIN_INPUT_INTERVAL_HINT,
		SCROLL_SPEED,
		SCROLL_SPEED_HINT,
		NEW_SONG_DURATION,
		NEW_SONG_DURATION_HINT,
		NEW_PROFILE,
		NEW_PROFILE_NAME,
		NEW_PROFILE_NAME_HINT,
		NEW_PROFILE_COPY_FROM_CURRENT,
		NEW_PROFILE_EMPTY,
		NEW_PROFILE_DUPLICATE,
		INITIALIZING,
		CHECKING_FOLDERS,
		LOADING_CHARTS,
		CHECKING_TABLES,
		LOADING_TABLE,
		DOWNLOADING_TABLE,
		LOADING_COURSES,
		COURSE_TITLE,
		COURSE_SUBTITLE,
		CLASS_TITLE,
		CLASS_SUBTITLE,
		BMS_NOT_FOUND,
		BMS_NOT_FOUND_HINT,
		PLEASE_WAIT,
		LOADING_SKIN_OPTIONS,
		LOADING_SKIN_OPTIONS_FINISHED,
		REFRESH_FOLDER,
		REFRESH_FOLDER_DETAIL,
		SEARCH_SONG,
		SEARCH_FAILED,
		SEARCH_RESULT,
		CHART_NOT_FOUND,
		CHART_NOT_FOUND_MD5,
		CUSTOM_FOLDER_DESCRIPTION,
		COURSE_FOLDER_DESCRIPTION,

		TEXT_COUNT
	};
}
constexpr size_t i18n_TEXT_COUNT = (size_t)i18nText::TEXT_COUNT;

class i18n
{
private:
	i18n(const Path& translationFile);
public:
	~i18n() = default;

private:
	std::array<std::string, i18n_TEXT_COUNT> text;

private:
	static std::vector<i18n> languages;
	static size_t currentLanguage;

public:
	static void init();

	static std::vector<std::string> getLanguageList();

	static void setLanguage(size_t index);	// get index from getLanguageList()
	static void setLanguage(const std::string& name);

	static const std::string& s(size_t index);	// i18nText::i18nTextIndex
	static const char* c(size_t index);	// i18nText::i18nTextIndex
};