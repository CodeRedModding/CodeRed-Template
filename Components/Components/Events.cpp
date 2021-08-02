#include "Events.hpp"
#include "../Includes.hpp"
#include "../Extensions/Includes.hpp"

namespace Functions
{
	void HUDPostRender(class UObject* caller, class UFunction* function, void* params)
	{
		if (caller)
		{
			if (Instances.IAHUD() != caller)
			{
				Instances.SetHUD(reinterpret_cast<AHUD*>(caller));
			}
				
			if (Instances.IUCanvas() != Instances.IAHUD()->Canvas)
			{
				Instances.SetCanvas(Instances.IAHUD()->Canvas);
			}
		}
	}

	void HUDPostRenderPost(class UObject* caller, class UFunction* function, void* params, void* result)
	{
		FRainbowColor::Tick(); // Example of where you could put your rainbow color hook.
		Manager.QueueTick(); // Example of where you could put your command queue tick.
	}

	void GameViewPortPostRender(class UObject* caller, class UFunction* function, void* params)
	{
		if (caller)
		{
			if (Instances.IUGameViewportClient() != caller)
			{
				Instances.SetGameViewportClient(reinterpret_cast<UGameViewportClient*>(caller));
			}				
		}
	}

	void PlayerControllerTick(class UObject* caller, class UFunction* function, void* params)
	{
		if (caller)
		{
			if (Instances.IAPlayerController() != caller)
			{
				Instances.SetPlayerController(reinterpret_cast<APlayerController*>(caller));
			}
		}
	}

	void GameViewPortKeyPress(class UObject* caller, class UFunction* function, void* params)
	{
		if (params)
		{
			UGameViewportClient_TA_execHandleKeyPress_Params* handleKeyPress = reinterpret_cast<UGameViewportClient_TA_execHandleKeyPress_Params*>(params);

			if (handleKeyPress->EventType == static_cast<uint8_t>(EInputEvent::IE_Released))
			{
				Manager.KeyPressed(handleKeyPress->Key.ToString());
			}
		}
	}

	void GFxDataMainMenuAdded(class UObject* caller, class UFunction* function, void* params)
	{
		if (caller)
		{
			GameState.MainMenuAdded();
		}
	}
}

EventsComponent::EventsComponent() : Component("Events", "Manages function hooks and process event.")
{
	ProcessEvent = nullptr;
}

EventsComponent::~EventsComponent() { }

void EventsComponent::ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result)
{
	if (ProcessEvent)
	{
		std::map<int32_t, std::vector<PreEventType>>::iterator preIt = PreHookedEvents.find(function->ObjectInternalInteger);

		if (preIt != PreHookedEvents.end())
		{
			for (const PreEventType& preEvent : preIt->second)
			{
				preEvent(caller, function, params);
			}
		}

		if (std::find(BlacklistedEvents.begin(), BlacklistedEvents.end(), function->ObjectInternalInteger) == BlacklistedEvents.end())
		{
			ProcessEvent(caller, function, params, result);
		}

		std::map<int32_t, std::vector<PostEventType>>::iterator postIt = PostHookedEvents.find(function->ObjectInternalInteger);

		if (postIt != PostHookedEvents.end())
		{
			for (const PostEventType& postEvent : postIt->second)
			{
				postEvent(caller, function, params, result);
			}
		}
	}
}

void EventsComponent::HookEventPre(const std::string& functionFullName, const PreEventType& eventType)
{
	const std::map<std::string, UFunction*>::iterator& functionIt = Instances.StaticFunctions.find(functionFullName);

	if (functionIt != Instances.StaticFunctions.end() && functionIt->second)
	{
		int32_t functionIndex = functionIt->second->ObjectInternalInteger;

		if (PreHookedEvents.find(functionIndex) != PreHookedEvents.end())
		{
			PreHookedEvents[functionIndex].push_back(eventType);
		}
		else
		{
			PreHookedEvents[functionIndex] = std::vector<PreEventType>{ eventType };
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to hook function \"" + functionFullName + "\"!");
	}
}

void EventsComponent::HookEventPost(const std::string& functionFullName, const PostEventType& eventType)
{
	const std::map<std::string, UFunction*>::iterator& functionIt = Instances.StaticFunctions.find(functionFullName);

	if (functionIt != Instances.StaticFunctions.end() && functionIt->second)
	{
		int32_t functionIndex = functionIt->second->ObjectInternalInteger;

		if (PostHookedEvents.find(functionIndex) != PostHookedEvents.end())
		{
			PostHookedEvents[functionIndex].push_back(eventType);
		}
		else
		{
			PostHookedEvents[functionIndex] = std::vector<PostEventType>{ eventType };
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to hook function \"" + functionFullName + "\"!");
	}
}

void EventsComponent::BlacklistEvent(const std::string& functionFullName)
{
	const std::map<std::string, UFunction*>::iterator& functionIt = Instances.StaticFunctions.find(functionFullName);

	if (functionIt != Instances.StaticFunctions.end() && functionIt->second)
	{
		if (std::find(BlacklistedEvents.begin(), BlacklistedEvents.end(), functionIt->second->ObjectInternalInteger) == BlacklistedEvents.end())
		{
			BlacklistedEvents.emplace_back(functionIt->second->ObjectInternalInteger);
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to blacklist function \"" + functionFullName + "\"!");
	}
}

void EventsComponent::WhitelistEvent(const std::string& functionFullName)
{
	const std::map<std::string, UFunction*>::iterator& functionIt = Instances.StaticFunctions.find(functionFullName);

	if (functionIt != Instances.StaticFunctions.end() && functionIt->second)
	{
		std::vector<int32_t>::iterator blackIt = std::find(BlacklistedEvents.begin(), BlacklistedEvents.end(), functionIt->second->ObjectInternalInteger);

		if (blackIt != BlacklistedEvents.end())
		{
			BlacklistedEvents.erase(blackIt);
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to whitelist function \"" + functionFullName + "\"!");
	}
}

void EventsComponent::Initialize()
{
	// Example functions only, you will need to function scan in your game for your own.

	BlacklistEvent("Function Engine.Tracker.ReportMetrics");

	HookEventPre("Function Engine.HUD.PostRender", &Functions::HUDPostRender);
	HookEventPost("Function Engine.HUD.PostRender", &Functions::HUDPostRenderPost);
	HookEventPre("Function Engine.GameViewportClient.PostRender", &Functions::GameViewPortPostRender);
	HookEventPre("Function Engine.PlayerController.PlayerTick", &Functions::PlayerControllerTick);
	HookEventPre("Function Engine.GameViewportClient.HandleKeyPress", &Functions::GameViewPortKeyPress);
	HookEventPre("Function Engine.GFxData_MainMenu.MainMenuAdded", &Functions::GFxDataMainMenuAdded);

	Console.Write(GetNameFormatted() + std::to_string(PreHookedEvents.size()) + " Pre-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(PostHookedEvents.size()) + " Post-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(BlacklistedEvents.size()) + " Backlisted Event(s) Initialized!");
}

class EventsComponent Events;