#pragma once
#include "../Component.hpp"

// This component is largely dependent on your game, this is just a bare bones example of how it would look.
// You will need to set your own function hooks here via scanning your game to determine what gets triggered when your game state changes.

enum States
{
	STATES_Unknown = 0U,
	STATES_MainMenu  = 1U << 0U,
	STATES_Trading = 1U << 1U,
	STATES_CasualMatch = 1U << 2U,
	STATES_RankedMatch = 1U << 3U,
	STATES_All = 1U << 4U
};

// Tracks where the player is, very useful in knowing if it is safe to perform certain unreal functions or modules.
class GameStateComponent : public Component
{
private:
	States CurrentState;
	std::map<States, std::string> StateLabelMap;

public:
	GameStateComponent();
	~GameStateComponent() override;

public:
	States GetState() const;
	uint32_t GetStateId() const;
	std::string GetStateLabel();
	bool IsInGame() const;
	void MainMenuAdded();
	void WaitingForState();
	void Initialize();
};

extern class GameStateComponent GameState;