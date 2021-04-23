#include "GameState.hpp"
#include "Console.hpp"

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
	if (GetStateId() & States::CasualMatch
		|| GetStateId() & States::RankedMatch)
	{
		return true;
	}

	return false;
}

void GameStateComponent::MainMenuAdded()
{
	CurrentState = States::MainMenu;
}

void GameStateComponent::WaitingForState()
{
	CurrentState = States::Unknown;
}

void GameStateComponent::Initialize()
{
	StateLabelMap.emplace(States::Unknown, "Unknown");
	StateLabelMap.emplace(States::MainMenu, "MainMenu");
	StateLabelMap.emplace(States::Trading, "Trading");
	StateLabelMap.emplace(States::CasualMatch, "CasualMatch");
	StateLabelMap.emplace(States::RankedMatch, "RankedMatch");

	WaitingForState();

	Console.Write(GetNameFormatted() + "Initialized!");
}

class GameStateComponent GameState;