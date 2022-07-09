#include "Events.hpp"
#include "../Includes.hpp"

PreEvent::PreEvent() : PE_Caller(nullptr), PE_Function(nullptr), PE_Params(nullptr), PE_Detour(true) {}

PreEvent::PreEvent(class UObject* caller, class UFunction* function, void* params) : PE_Caller(caller), PE_Function(function), PE_Params(params), PE_Detour(true) {}

PreEvent::~PreEvent() {}

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

bool PreEvent::ShouldDetour() const
{
	if (PE_Function && EventsComponent::IsEventBlacklisted(PE_Function->ObjectInternalInteger))
	{
		return false;
	}

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

PostEvent::PostEvent() : PE_Result(nullptr) {}

PostEvent::PostEvent(class UObject* caller, class UFunction* function, void* params, void* result) : PreEvent(caller, function, params), PE_Result(result) {}

PostEvent::~PostEvent() {}

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
			AHUD* hud = event.GetCaller<AHUD>();
			Instances.SetHUD(hud);
		}
	}

	void HUDPostRenderPost(const PostEvent& event)
	{
		GRainbowColor::OnTick(); // Example of where you could put your rainbow color hook.
		Manager.OnTick(); // Example of where you could put your command queue tick.
	}

	void GameViewPortPostRender(PreEvent& event)
	{
		if (event.Caller())
		{
			Instances.SetGameViewportClient(event.GetCaller<UGameViewportClient>());			
		}
	}

	void PlayerControllerTick(PreEvent& event)
	{
		if (event.Caller())
		{
			Instances.SetPlayerController(event.GetCaller<APlayerController>());
		}
	}

	void GameViewPortKeyPress(const PostEvent& event)
	{
		if (event.Params())
		{
			UGameViewportClient_TA_execHandleKeyPress_Params* handleKeyPress = event.GetParams<UGameViewportClient_TA_execHandleKeyPress_Params>();

			if (handleKeyPress->EventType == static_cast<uint8_t>(EInputEvent::IE_Released))
			{
				// Blah blah do key pressed stuff here blah blah
			}
		}
	}

	void GFxDataMainMenuAdded(PreEvent& event)
	{
		GameState.MainMenuAdded();
		event.SetDetour(false); // Purely an example only, if you were to "SetDetour(false)" your hooked function will NOT go through Process Event, so the game will never recognize that it was called.
	}
}

EventsComponent::EventsComponent() : Component("Events", "Manages function hooks and process event.") { OnCreate(); }

EventsComponent::~EventsComponent() { OnDestroy(); }

void EventsComponent::OnCreate()
{
	Detoured = false;
	ProcessEvent = nullptr;
}

void EventsComponent::OnDestroy()
{
	DetachDetour();
}

bool EventsComponent::IsDetoured()
{
	return (Detoured && ProcessEvent);
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
		Detoured = true;
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
		auto preIt = PreHookedEvents.find(function->ObjectInternalInteger);
		PreEvent event(caller, function, params);

		if (preIt != PreHookedEvents.end())
		{
			for (const std::function<void(PreEvent&)>& preEvent : preIt->second)
			{
				preEvent(event);

				if (event.ShouldDetour())
				{
					ProcessEvent(caller, function, params, result);
				}
			}
		}
		else if (event.ShouldDetour())
		{
			ProcessEvent(caller, function, params, result);
		}

		auto postIt = PostHookedEvents.find(function->ObjectInternalInteger);

		if (postIt != PostHookedEvents.end())
		{
			for (const std::function<void(const PostEvent&)>& postEvent : postIt->second)
			{
				postEvent(PostEvent(caller, function, params, result));
			}
		}
	}
}

bool EventsComponent::IsEventBlacklisted(uint32_t functionInteger)
{
	return (std::find(BlacklistedEvents.begin(), BlacklistedEvents.end(), functionInteger) != BlacklistedEvents.end());
}

void EventsComponent::BlacklistEvent(const std::string& function)
{
	UFunction* foundFunction = Instances.FindStaticFunction(function);

	if (foundFunction)
	{
		if (!IsEventBlacklisted(foundFunction->ObjectInternalInteger))
		{
			BlacklistedEvents.emplace_back(foundFunction->ObjectInternalInteger);
			std::sort(BlacklistedEvents.begin(), BlacklistedEvents.end());
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to blacklist function \"" + function + "\"!");
	}
}

void EventsComponent::WhitelistEvent(const std::string& functionName)
{
	UFunction* foundFunction = Instances.FindStaticFunction(functionName);

	if (foundFunction)
	{
		std::vector<uint32_t>::iterator blackIt = std::find(BlacklistedEvents.begin(), BlacklistedEvents.end(), foundFunction->ObjectInternalInteger);

		if (blackIt != BlacklistedEvents.end())
		{
			BlacklistedEvents.erase(blackIt);
			std::sort(BlacklistedEvents.begin(), BlacklistedEvents.end());
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to whitelist function \"" + functionName + "\"!");
	}
}

void EventsComponent::HookEventPre(const std::string& functionName, std::function<void(PreEvent&)> preHook)
{
	UFunction* foundFunction = Instances.FindStaticFunction(functionName);

	if (foundFunction)
	{
		HookEventPre(foundFunction->ObjectInternalInteger, preHook);
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to hook function \"" + functionName + "\"!");
	}
}

void EventsComponent::HookEventPre(uint32_t functionIndex, std::function<void(PreEvent&)> preHook)
{
	UObject* foundFunction = UObject::GObjObjects()->At(functionIndex);

	if (foundFunction && foundFunction->IsA(UFunction::StaticClass()))
	{
		if (PreHookedEvents.find(functionIndex) != PreHookedEvents.end())
		{
			PreHookedEvents[functionIndex].push_back(preHook);
		}
		else
		{
			PreHookedEvents[functionIndex] = std::vector<std::function<void(PreEvent&)>>{ preHook };
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to hook function at index \"" + std::to_string(functionIndex) + "\"!");
	}
}

void EventsComponent::HookEventPost(const std::string& functionName, std::function<void(const PostEvent&)> postHook)
{
	UFunction* foundFunction = Instances.FindStaticFunction(functionName);

	if (foundFunction)
	{
		HookEventPost(foundFunction->ObjectInternalInteger, postHook);
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to hook function \"" + functionName + "\"!");
	}
}

void EventsComponent::HookEventPost(uint32_t functionIndex, std::function<void(const PostEvent&)> postHook)
{
	UObject* foundFunction = UObject::GObjObjects()->At(functionIndex);

	if (foundFunction && foundFunction->IsA(UFunction::StaticClass()))
	{
		if (PostHookedEvents.find(functionIndex) != PostHookedEvents.end())
		{
			PostHookedEvents[functionIndex].push_back(postHook);
		}
		else
		{
			PostHookedEvents[functionIndex] = std::vector<std::function<void(const PostEvent&)>>{ postHook };
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to hook function at index \"" + std::to_string(functionIndex) + "\"!");
	}
}

void EventsComponent::Initialize()
{
#ifdef WALKTHROUGH
	Console.Notify("\"Components\\Components\\Events.cpp -> EventsComponent::Initialize()\". Looks like you forgot to check this file, the only thing here are demo functions hooks!");
	return;
#endif

	// Example functions only, you will need to function scan in your game for your own to hook!
	BlacklistEvent("Function Engine.Tracker.ReportMetrics");

	// You can hook functions like this.
	HookEventPre("Function Engine.HUD.PostRender", &Hooks::HUDPostRender);

	// Or like this, with std::bind.
	HookEventPost("Function Engine.HUD.PostRender", std::bind(&Hooks::HUDPostRenderPost, std::placeholders::_1));

	// Or even like this, with lambda expressions.
	HookEventPre("Function Engine.GameViewportClient.PostRender", [&](PreEvent& event) {
		Hooks::GameViewPortPostRender(event);
		Console.Write("I'm a lambda function hook!");
	});

	HookEventPre("Function Engine.PlayerController.PlayerTick", &Hooks::PlayerControllerTick);
	HookEventPost("Function Engine.GameViewportClient.HandleKeyPress", &Hooks::GameViewPortKeyPress);
	HookEventPre("Function Engine.GFxData_MainMenu.MainMenuAdded", &Hooks::GFxDataMainMenuAdded);

	Console.Write(GetNameFormatted() + std::to_string(PreHookedEvents.size()) + " Pre-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(PostHookedEvents.size()) + " Post-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(BlacklistedEvents.size()) + " Backlisted Event(s) Initialized!");
}

class EventsComponent Events{};