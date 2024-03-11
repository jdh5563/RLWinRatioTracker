#pragma once

#include <fstream>

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class RLWinRatioTracker: public BakkesMod::Plugin::BakkesModPlugin
	//,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	//std::shared_ptr<bool> enabled;
private:
	int games = 0;
	int wins = 0;

	std::unordered_map<std::string, int> conditionalGames = { {"Goals", 0}, {"Assists", 0}, {"Saves", 0}, {"Shots", 0} };
	
	void Save(std::string gameMode, int goals, int assists, int saves, int shots);
	void Load();
	
public:
	void onLoad() override;

	void RegisterCvars();
	void LoadHooks();
	void OnMatchEnd(std::string name);
	//void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
