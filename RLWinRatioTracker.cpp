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

void RLWinRatioTracker::RegisterCvars()
{
	cvarManager->registerCvar("winRatioTracker_trackGoals", "1", "Track win ratio based on games where you score a certain number of goals", true, true, 0, true, 1);
	cvarManager->registerCvar("winRatioTracker_minGoalsScored", "1", "The minimum number of goals scored in a match to count towards the tracker", true, true, 1);
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

	Save(server.GetPlaylist().GetTitle().ToString(), player.GetMatchGoals(), player.GetMatchAssists(), player.GetMatchSaves(), player.GetMatchShots());
}

/// <summary>
/// Save player data to disk
/// </summary>
/// <param name="gameMode">The playlist</param>
/// <param name="goals">Number of goals scored</param>
/// <param name="assists">Number of assists</param>
/// <param name="saves">Number of saves</param>
/// <param name="shots">Number of shot attempts</param>
void RLWinRatioTracker::Save(std::string gameMode, int goals, int assists, int saves, int shots)
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

	stream << goals << "," << assists << "," << saves << "," << shots << std::endl;

	stream.close();
}

void RLWinRatioTracker::Load()
{
}