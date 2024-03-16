#include "pch.h"
#include "RLWinRatioTracker.h"


BAKKESMOD_PLUGIN(RLWinRatioTracker, "Win Ratio Tracker", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLWinRatioTracker::onLoad()
{
	_globalCvarManager = cvarManager;

	this->LoadHooks();
	this->RegisterCvars();
}

void RLWinRatioTracker::RenderSettings()
{
	ImGui::TextUnformatted("Rocket League Win Ratio Tracker");

	for (int i = 0; i < displayToggles.size(); i++) {
		bool enabled = displayToggles[i].getBoolValue();
		std::string checkboxText = "Track " + displayToggles[i].getCVarName().substr(21);

		if (ImGui::Checkbox(checkboxText.c_str(), &enabled)) {
			displayToggles[i].setValue(enabled);
		}
	}

	for (int i = 0; i < statMinimums.size(); i++) {
		int min = statMinimums[i].getIntValue();
		std::string sliderText = "Minimum " + displayToggles[i].getCVarName().substr(21);

		if (ImGui::SliderInt(sliderText.c_str(), &min, 1, 10)) {
			statMinimums[i].setValue(min);
		}
	}

	DisplayWinRatios();
}

void RLWinRatioTracker::RegisterCvars()
{
	displayToggles.push_back(cvarManager->registerCvar("winRatioTracker_trackGoals", "1", "Track win ratio based on games where you score a certain number of goals", true, true, 0, true, 1));
	statMinimums.push_back(cvarManager->registerCvar("winRatioTracker_minGoalsScored", "1", "The minimum number of goals scored in a match to count towards the tracker", true, true, 1));

	displayToggles.push_back(cvarManager->registerCvar("winRatioTracker_trackAssists", "1", "Track win ratio based on games where you make a certain number of assists", true, true, 0, true, 1));
	statMinimums.push_back(cvarManager->registerCvar("winRatioTracker_minAssistsMade", "1", "The minimum number of assists made in a match to count towards the tracker", true, true, 1));

	displayToggles.push_back(cvarManager->registerCvar("winRatioTracker_trackSaves", "1", "Track win ratio based on games where you make a certain number of saves", true, true, 0, true, 1));
	statMinimums.push_back(cvarManager->registerCvar("winRatioTracker_minSavesMade", "1", "The minimum number of saves made in a match to count towards the tracker", true, true, 1));

	displayToggles.push_back(cvarManager->registerCvar("winRatioTracker_trackShots", "1", "Track win ratio based on games where you take a certain number of shots", true, true, 0, true, 1));
	statMinimums.push_back(cvarManager->registerCvar("winRatioTracker_minShotsTaken", "1", "The minimum number of shots taken in a match to count towards the tracker", true, true, 1));
}

void RLWinRatioTracker::LoadHooks()
{
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.OnMatchEnded", std::bind(&RLWinRatioTracker::OnMatchEnd, this, std::placeholders::_1));
}

/// <summary>
/// Get player stats after a match ends and save them to disk
/// </summary>
void RLWinRatioTracker::OnMatchEnd(std::string name)
{
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (!server) { return; }

	PriWrapper player = gameWrapper->GetPlayerController().GetPRI();

	Save(server.GetPlaylist().GetTitle().ToString(), player.GetMatchGoals(), player.GetMatchAssists(), player.GetMatchSaves(), player.GetMatchShots(), server.GetMatchWinner().GetTeamIndex() == player.GetTeam().GetTeamIndex());
}

void RLWinRatioTracker::Save(std::string gameMode, int goals, int assists, int saves, int shots, int won)
{
	// Handling if the player was in a private match
	if (gameMode == "") gameMode = "private_match";

	// Create directories if they don't yet exist
	if (!std::filesystem::exists(gameWrapper->GetDataFolder() / "RLWinRatioTracker")) {
		std::filesystem::create_directory(gameWrapper->GetDataFolder() / "RLWinRatioTracker");
	}

	if (!std::filesystem::exists(gameWrapper->GetDataFolder() / "RLWinRatioTracker" / gameMode)) {
		std::filesystem::create_directory(gameWrapper->GetDataFolder() / "RLWinRatioTracker" / gameMode);
	}

	// Save the data
	std::ofstream stream(gameWrapper->GetDataFolder() / "RLWinRatioTracker" / gameMode / "data.txt", std::ios_base::app);

	stream << "Goals," << goals << ",Assists," << assists << ",Saves," << saves << ",Shots," << shots << ",Won," << won << std::endl;

	stream.close();
}

void RLWinRatioTracker::Load()
{
	std::ifstream stream("C:/Users/heide/Desktop/test.txt"); // <- Replace this with the correct general path from Save()
	std::string line;

	// Iterate through each match in the file
	while (std::getline(stream, line)) {
		std::vector<std::string> splitLine = SplitString(line, ',');
		int won = std::stoi(splitLine[9]);

		std::get<0>(gameStats["Overall"]) += won;
		std::get<1>(gameStats["Overall"])++;

		for (int i = 0; i < splitLine.size() - 2; i += 2) {
			if (std::stoi(splitLine[i + 1]) >= std::get<2>(gameStats[splitLine[i]])) {
				std::get<0>(gameStats[splitLine[i]]) += won;
				std::get<1>(gameStats[splitLine[i]])++;
			}
		}
	}

	stream.close();
}

std::vector<std::string> RLWinRatioTracker::SplitString(std::string stringToSplit, char delimeter)
{
	std::vector<std::string> splitString = std::vector<std::string>();
	std::stringstream stringStream(stringToSplit);
	std::string stringPart;
	while (std::getline(stringStream, stringPart, delimeter)) {
		splitString.push_back(stringPart);
	}

	return splitString;
}

void RLWinRatioTracker::DisplayWinRatios()
{

}