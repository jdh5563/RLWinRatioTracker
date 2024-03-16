#pragma once

#include <fstream>

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class RLWinRatioTracker: public BakkesMod::Plugin::BakkesModPlugin
	,public SettingsWindowBase
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	//std::shared_ptr<bool> enabled;
private:
	std::unordered_map<std::string, std::tuple<int, int, int>> gameStats = { {"Overall", {0, 0, 0}}, {"Goals", {0, 0, 0}}, {"Assists", {0, 0, 0}}, {"Saves", {0, 0, 0}}, {"Shots", {0, 0, 0}} };
	std::vector<CVarWrapper> displayToggles;
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
	void Save(std::string gameMode, int goals, int assists, int saves, int shots, int won);

	/// <summary>
	/// Loads saved match data
	/// </summary>
	void Load();

	/// <summary>
	/// Returns a vector containing the given string split based on the given delimeter
	/// </summary>
	/// <param name="stringToSplit">The string to split</param>
	/// <param name="delimeter">The delimeter to base the split on</param>
	/// <returns>A vector containing the split string</returns>
	std::vector<std::string> SplitString(std::string stringToSplit, char delimeter);


	void DisplayWinRatios();
	
public:
	void onLoad() override;

	void RegisterCvars();
	void LoadHooks();
	void OnMatchEnd(std::string name);
	void RenderSettings() override;
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
