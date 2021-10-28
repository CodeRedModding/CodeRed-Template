#include "Events.hpp"
#include "../Includes.hpp"

PreEvent::PreEvent() : PE_Caller(nullptr), PE_Function(nullptr), PE_Params(nullptr), PE_Detour(true) { }

PreEvent::PreEvent(class UObject* caller, class UFunction* function, void* params) : PE_Caller(caller), PE_Function(function), PE_Params(params), PE_Detour(true) { }

PreEvent::~PreEvent() { }

class UObject* PreEvent::Caller() const
{
	return PE_Caller;
}

template <typename T> T* PreEvent::GetCaller() const
{
	if (PE_Caller)
	{
		return static_cast<T*>(PE_Caller);
	}

	return nullptr;
}

class UFunction* PreEvent::Function() const
{
	return PE_Function;
}

void* PreEvent::Params() const
{
	return PE_Params;
}

template <typename T> T* PreEvent::GetParams() const
{
	if (PE_Params)
	{
		return reinterpret_cast<T*>(PE_Params);
	}

	return nullptr;
}

bool PreEvent::Detour() const
{
	return PE_Detour;
}

void PreEvent::SetDetour(bool bDetour)
{
	PE_Detour = bDetour;
}

PreEvent PreEvent::operator=(const PreEvent& other)
{
	PE_Caller = other.PE_Caller;
	PE_Function = other.PE_Function;
	PE_Params = other.PE_Params;
	PE_Detour = other.PE_Detour;
	return *this;
}

PostEvent::PostEvent() : PE_Result(nullptr) { }

PostEvent::PostEvent(class UObject* caller, class UFunction* function, void* params, void* result) : PreEvent(caller, function, params), PE_Result(result) { }

PostEvent::~PostEvent() { }

void* PostEvent::Result() const
{
	return PE_Result;
}

template <typename T> T* PostEvent::GetResult() const
{
	if (PE_Result)
	{
		return reinterpret_cast<T*>(PE_Result);
	}

	return nullptr;
}

PostEvent PostEvent::operator=(const PostEvent& other)
{
	PE_Caller = other.PE_Caller;
	PE_Function = other.PE_Function;
	PE_Params = other.PE_Params;
	PE_Detour = other.PE_Detour;
	PE_Result = other.PE_Result;
	return *this;
}

namespace Hooks
{
	void HUDPostRender(PreEvent& event)
	{
		if (event.Caller())
		{
			if (Instances.IAHUD() != event.Caller())
			{
				Instances.SetHUD(event.GetCaller<AHUD>());
			}
				
			if (Instances.IUCanvas() != Instances.IAHUD()->Canvas)
			{
				Instances.SetCanvas(Instances.IAHUD()->Canvas);
			}
		}
	}

	void HUDPostRenderPost(const PostEvent& event)
	{
		FRainbowColor::OnTick(); // Example of where you could put your rainbow color hook.
		Manager.QueueTick(); // Example of where you could put your command queue tick.
	}

	void GameViewPortPostRender(PreEvent& event)
	{
		if (event.Caller())
		{
			if (Instances.IUGameViewportClient() != event.Caller())
			{
				Instances.SetGameViewportClient(event.GetCaller<UGameViewportClient>());
			}				
		}
	}

	void PlayerControllerTick(PreEvent& event)
	{
		if (event.Caller())
		{
			if (Instances.IAPlayerController() != event.Caller())
			{
				Instances.SetPlayerController(event.GetCaller<APlayerController>());
			}
		}
	}

	void GameViewPortKeyPress(PreEvent& event)
	{
		if (event.Params())
		{
			UGameViewportClient_TA_execHandleKeyPress_Params* handleKeyPress = event.GetParams<UGameViewportClient_TA_execHandleKeyPress_Params>();

			if (handleKeyPress->EventType == static_cast<uint8_t>(EInputEvent::IE_Released))
			{
				Manager.KeyPressed(handleKeyPress->Key.ToString());
			}
		}
	}

	void GFxDataMainMenuAdded(PreEvent& event)
	{
		GameState.MainMenuAdded();

		// Purely an example only, if you were to "SetDetour(false)" your hooked function will NOT go through Process Event, so the game will never recognize that it was called.
		event.SetDetour(false);
	}
}

EventsComponent::EventsComponent() : Component("Events", "Manages function hooks and process event.")
{
	OnCreate();
}

EventsComponent::~EventsComponent()
{
	OnDestroy();
}

void EventsComponent::OnCreate()
{
	ProcessEvent = nullptr;
}

void EventsComponent::OnDestroy()
{
	DetachDetour();
}

bool EventsComponent::IsDetoured()
{
	return (Detoured && !ProcessEvent);
}

void EventsComponent::AttachDetour(const ProcessEventType& detourFunction)
{
	if (!IsDetoured())
	{
		ProcessEvent = detourFunction;
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&reinterpret_cast<PVOID&>(ProcessEvent), reinterpret_cast<PVOID>(ProcessEventDetour));
		DetourTransactionCommit();
	}
}

void EventsComponent::DetachDetour()
{
	if (IsDetoured())
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&reinterpret_cast<PVOID&>(ProcessEvent), reinterpret_cast<PVOID>(ProcessEventDetour));
		DetourTransactionCommit();
		Detoured = false;
		ProcessEvent = nullptr;
	}
}

void EventsComponent::ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result)
{
	if (ProcessEvent)
	{
		std::map<int32_t, std::vector<PreEventType>>::iterator preIt = PreHookedEvents.find(function->ObjectInternalInteger);

		PreEvent event; // In the default constructor we set "PE_Detour" to true, so even if a function hook is found it will correctly go through Process Event.

		if (preIt != PreHookedEvents.end())
		{
			for (const PreEventType& preEvent : preIt->second)
			{
				event = PreEvent(caller, function, params);
				preEvent(event);
			}
		}

		if (std::find(BlacklistedEvents.begin(), BlacklistedEvents.end(), function->ObjectInternalInteger) == BlacklistedEvents.end())
		{
			if (event.Detour())
			{
				ProcessEvent(caller, function, params, result);
			}
		}

		std::map<int32_t, std::vector<PostEventType>>::iterator postIt = PostHookedEvents.find(function->ObjectInternalInteger);

		if (postIt != PostHookedEvents.end())
		{
			for (const PostEventType& postEvent : postIt->second)
			{
				postEvent(PostEvent(caller, function, params, result));
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
	// Example functions only, you will need to function scan in your game for your own to hook!

	BlacklistEvent("Function Engine.Tracker.ReportMetrics");

	HookEventPre("Function Engine.HUD.PostRender", &Hooks::HUDPostRender);
	HookEventPost("Function Engine.HUD.PostRender", &Hooks::HUDPostRenderPost);
	HookEventPre("Function Engine.GameViewportClient.PostRender", &Hooks::GameViewPortPostRender);
	HookEventPre("Function Engine.PlayerController.PlayerTick", &Hooks::PlayerControllerTick);
	HookEventPre("Function Engine.GameViewportClient.HandleKeyPress", &Hooks::GameViewPortKeyPress);
	HookEventPre("Function Engine.GFxData_MainMenu.MainMenuAdded", &Hooks::GFxDataMainMenuAdded);

	Console.Write(GetNameFormatted() + std::to_string(PreHookedEvents.size()) + " Pre-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(PostHookedEvents.size()) + " Post-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(BlacklistedEvents.size()) + " Backlisted Event(s) Initialized!");
}

class EventsComponent Events{};