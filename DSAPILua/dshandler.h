#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>

#include <httplib.h>

#include <nlohmann/json.hpp>

struct Offset {
	uintptr_t offset;
	size_t size;

	bool isBit = false;
	int bitOffset = 0;

	bool valid = false;

	operator bool() const {
		return valid;
	}
};

struct GameData {
	std::string id;
	std::string engine;
	std::string location;
};

/// Based on https://github.com/Spuckwaffel/Dumpspace-API/
class DSHandler {
public:
	DSHandler() {
		// First, we download the game list!

		auto url = gameList;

		httplib::Client cli("dumpspace.spuckwaffel.com");
		auto res = cli.Get("/Games/GameList.json");
	}

	void loadFromGameHash(std::string_view gameHash) {
		gameData_.id = gameHash;
	}

private:
	static inline constexpr const char* website = "https://dumpspace.spuckwaffel.com/Games/";
	static inline constexpr const char* gameList = "https://dumpspace.spuckwaffel.com/Games/GameList.json";

	GameData gameData_;

	nlohmann::json gameListJson_ = nlohmann::detail::value_t::discarded;

	nlohmann::json classJson_ = nlohmann::detail::value_t::discarded;
	nlohmann::json structJson_ = nlohmann::detail::value_t::discarded;
	nlohmann::json enumJson_ = nlohmann::detail::value_t::discarded;
	nlohmann::json offsetJson_ = nlohmann::detail::value_t::discarded;

	std::unordered_map<std::string, Offset> classMembers_; // className + varName -> offset
	std::unordered_map<std::string, size_t> classSizes_; // class name -> size
	std::unordered_map<std::string, std::string> enumNameMap_; // enum name + value -> name
	std::unordered_map<std::string, uintptr_t> offsets_; // offset name -> value (e.g. for gworld)

	std::string downloadData(std::string_view url);
	void downloadGameList();
};

static inline DSHandler g_DSHandler;
