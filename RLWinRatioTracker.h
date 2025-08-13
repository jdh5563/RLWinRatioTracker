#pragma once

#include <fstream>

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class RLWinRatioTracker: public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase
{
private:
	std::string mostRecentTouchID = "";

	int crossbarTotal = 0;
	int crossbarByGame = 0;

	const std::unordered_map<std::string, std::unordered_map<std::string, std::tuple<int, int>>> defaultStats = {
		{ "Duel", { {"Overall", {0, 0}}, {"Goals", {0, 0}}, {"Assists", {0, 0}}, {"Saves", {0, 0}}, {"Shots", {0, 0}} } },
		{ "Doubles", { {"Overall", {0, 0}}, {"Goals", {0, 0}}, {"Assists", {0, 0}}, {"Saves", {0, 0}}, {"Shots", {0, 0}} } },
		{ "Standard", { {"Overall", {0, 0}}, {"Goals", {0, 0}}, {"Assists", {0, 0}}, {"Saves", {0, 0}}, {"Shots", {0, 0}} } }
	};

	// Maps game modes to statistics related to that mode
	std::unordered_map<std::string, std::unordered_map<std::string, std::tuple<int, int>>> gameStats = defaultStats;

	// Holds whether each statistic should be displayed
	std::vector<CVarWrapper> displayToggles;

	// Holds the minimum value a statistic must meet to be counted for tracking
	std::vector<CVarWrapper> statMinimums;

	/// <summary>
	/// Saves match statistics to disk
	/// </summary>
	/// <param name="gameMode">The playlist associate with this match</param>
	/// <param name="goals">The number of goals scored</param>
	/// <param name="assists">The number of assists made</param>
	/// <param name="saves">The number of saves made</param>
	/// <param name="shots">The number of shots taken</param>
	/// <param name="won">Whether the match was won</param>
	void Save(std::string gameMode, int goals, int assists, int saves, int shots, int won, int crossbars);

	/// <summary>
	/// Loads saved match data
	/// </summary>
	void Load();

	/// <summary>
	/// Update internal values for statistics
	/// </summary>
	/// <param name="gameMode">The playlist associate with this match</param>
	/// <param name="goals">The number of goals scored</param>
	/// <param name="assists">The number of assists made</param>
	/// <param name="saves">The number of saves made</param>
	/// <param name="shots">The number of shots taken</param>
	/// <param name="won">Whether the match was won</param>
	void UpdateGameStats(std::string gameMode, int goals, int assists, int saves, int shots, int won);

	/// <summary>
	/// Returns a vector containing the given string split based on the given delimeter
	/// </summary>
	/// <param name="stringToSplit">The string to split</param>
	/// <param name="delimeter">The delimeter to base the split on</param>
	std::vector<std::string> SplitString(std::string stringToSplit, char delimeter);

	/// <summary>
	/// Displays all statistics in the Bakkesmod GUI (WARNING: This function only works if called from the RenderSettings function)
	/// </summary>
	void DisplayWinRatios();

	/// <summary>
	/// Returns a vector containing all the keys in the given map
	/// </summary>
	/// <typeparam name="TKey">The type of the map's keys</typeparam>
	/// <typeparam name="TVal">The type of the map's values</typeparam>
	/// <param name="map">The map to parse</param>
	template<typename TKey, typename TVal>
	std::vector<TKey> GetMapKeys(std::unordered_map<TKey, TVal> map) {
		std::vector<TKey> keys;

		for (auto element : map) {
			keys.push_back(element.first);
		}

		return keys;
	}
	
public:
	void onLoad() override;
	void RegisterCvars();
	void LoadHooks();
	void OnMatchEnd(std::string name);
	void OnCrossbarHit(std::string name);
	void RenderSettings() override;
};