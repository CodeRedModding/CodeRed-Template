#include "GameState.hpp"
#include "../Includes.hpp"

GameStateComponent::GameStateComponent() : Component("GameState", "Tracks which game state the player is currently in.")
{
	OnCreate();
}

GameStateComponent::~GameStateComponent()
{
	OnDestroy();
}

void GameStateComponent::OnCreate()
{
	StateLabelMap.emplace(States::STATES_Unknown, "Unknown");
	StateLabelMap.emplace(States::STATES_MainMenu, "MainMenu");
	StateLabelMap.emplace(States::STATES_Trading, "Trading");
	StateLabelMap.emplace(States::STATES_CasualMatch, "CasualMatch");
	StateLabelMap.emplace(States::STATES_RankedMatch, "RankedMatch");
	StateLabelMap.emplace(States::STATES_All, "All");
}

void GameStateComponent::OnDestroy() {}

States GameStateComponent::GetState() const
{
	return CurrentState;
}

uint32_t GameStateComponent::GetRawState() const
{
	return static_cast<uint32_t>(CurrentState);
}

std::string GameStateComponent::GetStateLabel() const
{
	if (StateLabelMap.find(CurrentState) != StateLabelMap.end())
	{
		return StateLabelMap.at(CurrentState);
	}

	return "Unknown";
}

void GameStateComponent::SetState(States state)
{
	CurrentState = state;
}

bool GameStateComponent::IsInGame() const
{
	if ((GetRawState() & States::STATES_CasualMatch) || (GetRawState() & States::STATES_RankedMatch))
	{
		return true;
	}

	return false;
}

void GameStateComponent::MainMenuAdded()
{
	CurrentState = States::STATES_MainMenu;
}

class GameStateComponent GameState{};