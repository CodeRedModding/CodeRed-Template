#include "Events.hpp"
#include "../Includes.hpp"

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
	LogFunctions = false;
	ProcessEvent = nullptr;
}

EventsComponent::~EventsComponent() { }

void EventsComponent::ToggleLogFunctions(const bool& bLog)
{
	if (bLog)
	{
		Console.ToggleTimestamp(false);
		Console.Write(GetNameFormatted() + "Enabled Function Logging.");
		LogFunctions = true;
	}
	else
	{
		Console.ToggleTimestamp(true);
		Console.Write(GetNameFormatted() + "Disabled Function Logging.");
		LogFunctions = false;
	}
}

void EventsComponent::MapFunctions()
{
	for (UObject* uObject : *UObject::GObjObjects())
	{
		if (uObject && uObject->IsA(UFunction::StaticClass()))
		{
			UFunction* function = reinterpret_cast<UFunction*>(uObject);
			std::string functionFullName = function->GetFullName();

			if (FunctionIndexMap.find(functionFullName) == FunctionIndexMap.end())
			{
				FunctionIndexMap.emplace(functionFullName, function->ObjectInternalInteger);
			}
		}
	}
}

void EventsComponent::ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result)
{
	if (ProcessEvent)
	{
		if (LogFunctions && function)
		{
			Console.Write(function->GetFullName());
		}

		std::map<int32_t, PreEventType>::iterator preIt = PreHookedEvents.find(function->ObjectInternalInteger);

		if (preIt != PreHookedEvents.end())
		{
			preIt->second(caller, function, params);
		}

		if (std::find(BlackHookedEvents.begin(), BlackHookedEvents.end(), function->ObjectInternalInteger) == BlackHookedEvents.end())
		{
			ProcessEvent(caller, function, params, result);
		}

		std::map<int32_t, PostEventType>::iterator postIt = PostHookedEvents.find(function->ObjectInternalInteger);

		if (postIt != PostHookedEvents.end())
		{
			postIt->second(caller, function, params, result);
		}
	}
}

void EventsComponent::HookEventPre(const std::string& functionFullName, const PreEventType& eventType)
{
	PreHookedEvents.emplace(FunctionIndexMap[functionFullName], eventType);
}

void EventsComponent::HookEventPost(const std::string& functionFullName, const PostEventType& eventType)
{
	PostHookedEvents.emplace(FunctionIndexMap[functionFullName], eventType);
}

void EventsComponent::BlacklistEvent(const std::string& functionFullName)
{
	BlackHookedEvents.emplace_back(FunctionIndexMap[functionFullName]);
}

void EventsComponent::Initialize()
{
	MapFunctions();

	// Example functions only, you will need to function scan in your game for your own.

	HookEventPre("Function Engine.HUD.PostRender", &Functions::HUDPostRender);
	HookEventPost("Function Engine.HUD.PostRender", &Functions::HUDPostRenderPost);
	HookEventPre("Function Engine.GameViewportClient.PostRender", &Functions::GameViewPortPostRender);
	HookEventPre("Function Engine.PlayerController.PlayerTick", &Functions::PlayerControllerTick);
	HookEventPre("Function Engine.GameViewportClient.HandleKeyPress", &Functions::GameViewPortKeyPress);
	HookEventPre("Function Engine.GFxData_MainMenu.MainMenuAdded", &Functions::GFxDataMainMenuAdded);

	Console.Write(GetNameFormatted() + std::to_string(PreHookedEvents.size()) + " Pre-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(PostHookedEvents.size()) + " Post-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(BlackHookedEvents.size()) + " Backlisted Event(s) Initialized!");
}

class EventsComponent Events;