#include "GameState.hpp"
#include "../Includes.hpp"

GameStateComponent::GameStateComponent() : Component("GameState", "Tracks which game state the player is currently in.")
{
	WaitingForState();
}

GameStateComponent::~GameStateComponent() { }

States GameStateComponent::GetState() const
{
	return CurrentState;
}

uint32_t GameStateComponent::GetStateId() const
{
	return static_cast<uint32_t>(GetState());
}

std::string GameStateComponent::GetStateLabel()
{
	return StateLabelMap[CurrentState];
}

bool GameStateComponent::IsInGame() const
{
	if (GetStateId() & States::STATES_CasualMatch
		|| GetStateId() & States::STATES_RankedMatch)
	{
		return true;
	}

	return false;
}

void GameStateComponent::MainMenuAdded()
{
	CurrentState = States::STATES_MainMenu;
}

void GameStateComponent::WaitingForState()
{
	CurrentState = States::STATES_Unknown;
}

void GameStateComponent::Initialize()
{
	StateLabelMap.emplace(States::STATES_Unknown, "Unknown");
	StateLabelMap.emplace(States::STATES_MainMenu, "MainMenu");
	StateLabelMap.emplace(States::STATES_Trading, "Trading");
	StateLabelMap.emplace(States::STATES_CasualMatch, "CasualMatch");
	StateLabelMap.emplace(States::STATES_RankedMatch, "RankedMatch");

	WaitingForState();

	Console.Write(GetNameFormatted() + "Initialized!");
}

class GameStateComponent GameState{};