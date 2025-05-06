#include "GameState.hpp"
#include "../Includes.hpp"

namespace CodeRed
{
	GameStateComponent::GameStateComponent() : Component("GameState", "Tracks which game state the player is currently in.") { OnCreate(); }

	GameStateComponent::~GameStateComponent() { OnDestroy(); }

	void GameStateComponent::OnCreate()
	{
		m_currentState = States::STATES_Unknown;

		m_stateNames = {
			{ States::STATES_Unknown, "Unknown" },
			{ States::STATES_MainMenu, "MainMenu" },
			{ States::STATES_OfflineMatch, "OfflineMatch" },
			{ States::STATES_CasualMatch, "CasualMatch" },
			{ States::STATES_RankedMatch, "RankedMatch" },
			{ States::STATES_All, "All" }
		};
	}

	void GameStateComponent::OnDestroy() {}

	bool GameStateComponent::Initialize()
	{
		if (!IsInitialized())
		{
			Console.Success(GetNameFormatted() + "Initialized!");
			SetInitialized(true);
		}

		return IsInitialized();
	}

	States GameStateComponent::GetState() const
	{
		return static_cast<States>(GetRawState());
	}

	uint32_t GameStateComponent::GetRawState() const
	{
		return m_currentState;
	}

	std::string GameStateComponent::GetStateName() const
	{
		if (m_stateNames.contains(GetState()))
		{
			return m_stateNames.at(GetState());
		}

		return "Unknown";
	}

	bool GameStateComponent::IsInState(States state) const
	{
		return (m_currentState & state);
	}

	void GameStateComponent::SetState(States state)
	{
		m_currentState = state;
	}

	bool GameStateComponent::IsInOnlineGame() const
	{
		return (IsInState(States::STATES_CasualMatch) || IsInState(States::STATES_RankedMatch));
	}

	class GameStateComponent GameState;
}