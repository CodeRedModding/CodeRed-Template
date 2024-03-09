#pragma once
#include "../Component.hpp"

// This component is largely dependent on your game, this is just a bare bones example of how it would look.
// You will need to set your own function hooks here through function scanning to determine what gets triggered when your game state changes.

enum States : uint32_t
{
	STATES_Unknown = 0U,
	STATES_MainMenu = 1U << 0U,
	STATES_OfflineMatch = 1U << 1U,
	STATES_CasualMatch = 1U << 2U,
	STATES_RankedMatch = 1U << 3U,
	STATES_All = 1U << 4U
};

// THIS COMPONENT IS LARGELY DEPENDENT ON YOUR GAME
// Manages where the player is at all times, informs us when it is safe to perform certain actions or functions inside of modules.
class GameStateComponent : public Component
{
private:
	uint32_t m_currentState;
	std::map<States, std::string> m_stateNames;

public:
	GameStateComponent();
	~GameStateComponent() override;

public:
	void OnCreate() override;
	void OnDestroy() override;

public:
	States GetState() const;
	uint32_t GetRawState() const;
	std::string GetStateName() const;
	bool IsInState(States state) const;

public:
	void SetState(States state);
	bool IsInOnlineGame() const;
};

extern class GameStateComponent GameState;