#include "pch.h"
#include "RLWinRatioTracker.h"


BAKKESMOD_PLUGIN(RLWinRatioTracker, "Easily track your win ratio and how different statistics affect it!", "0.1.0", 0)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLWinRatioTracker::onLoad()
{
	this->LoadHooks();
}

void RLWinRatioTracker::onUnload()
{

}

void RLWinRatioTracker::LoadHooks()
{
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.OnMatchEnded", std::bind(&RLWinRatioTracker::OnMatchEnd, this, std::placeholders::_1));
}

void RLWinRatioTracker::OnMatchEnd(std::string name)
{
	Log("This match has ended");
}

void RLWinRatioTracker::Log(std::string message)
{
	cvarManager->log(message);
}