#include "Events.hpp"
#include "../Includes.hpp"

PreEvent::PreEvent() : m_caller(nullptr), m_function(nullptr), m_params(nullptr), m_detour(true) {}

PreEvent::PreEvent(class UObject* caller, class UFunction* function, void* params, bool bDetour) : m_caller(caller), m_function(function), m_params(params), m_detour(bDetour) {}

PreEvent::PreEvent(const PreEvent& preEvent) : m_caller(preEvent.m_caller), m_function(preEvent.m_function), m_params(preEvent.m_params), m_detour(preEvent.m_detour) {}

PreEvent::~PreEvent() {}

class UObject* PreEvent::Caller() const
{
	return m_caller;
}

template <typename T> T* PreEvent::GetCaller() const
{
	if (m_caller)
	{
		return static_cast<T*>(m_caller);
	}

	return nullptr;
}

class UFunction* PreEvent::Function() const
{
	return m_function;
}

void* PreEvent::Params() const
{
	return m_params;
}

template <typename T> T* PreEvent::GetParams() const
{
	if (m_params)
	{
		return reinterpret_cast<T*>(m_params);
	}

	return nullptr;
}

bool PreEvent::ShouldDetour() const
{
	return m_detour;
}

void PreEvent::SetDetour(bool bDetour)
{
	m_detour = bDetour;
}

PreEvent& PreEvent::operator=(const PreEvent& other)
{
	m_caller = other.m_caller;
	m_function = other.m_function;
	m_params = other.m_params;
	m_detour = other.m_detour;
	return *this;
}

PostEvent::PostEvent() : m_result(nullptr) {}

PostEvent::PostEvent(class UObject* caller, class UFunction* function, void* params, void* result) : PreEvent(caller, function, params), m_result(result) {}

PostEvent::PostEvent(const PostEvent& postEvent) : PreEvent(postEvent.m_caller, postEvent.m_function, postEvent.m_params, postEvent.m_detour), m_result(postEvent.m_result) {}

PostEvent::~PostEvent() {}

void* PostEvent::Result() const
{
	return m_result;
}

template <typename T> T* PostEvent::GetResult() const
{
	if (m_result)
	{
		return reinterpret_cast<T*>(m_result);
	}

	return nullptr;
}

PostEvent& PostEvent::operator=(const PostEvent& other)
{
	m_caller = other.m_caller;
	m_function = other.m_function;
	m_params = other.m_params;
	m_detour = other.m_detour;
	m_result = other.m_result;
	return *this;
}

namespace Hooks
{
	void HUDPostRender(PreEvent& event)
	{
		if (event.Caller())
		{
			Instances.SetHUD(event.GetCaller<AHUD>());
		}
	}

	void HUDPostRenderPost(const PostEvent& event)
	{
		if (event.Caller())
		{
			Manager.OnTick();
		}
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
		GameState.SetState(STATES_MainMenu);
		event.SetDetour(false); // Purely an example only, if you were to "SetDetour(false)" your hooked function will NOT go through Process Event, so the game will never recognize that it was called.
	}
}

EventsComponent::EventsComponent() : Component("Events", "Manages function hooks and process event.") { OnCreate(); }

EventsComponent::~EventsComponent() { OnDestroy(); }

void EventsComponent::OnCreate()
{
	m_detoured = false;
	m_processEvent = nullptr;
}

void EventsComponent::OnDestroy()
{
	DetachDetour();
	m_blacklisted.clear();
	m_preHooks.clear();
	m_postHooks.clear();
}

bool EventsComponent::IsDetoured()
{
	return (m_detoured && m_processEvent);
}

void EventsComponent::AttachDetour(const ProcessEventType& detourFunction)
{
	if (!IsDetoured())
	{
		m_processEvent = detourFunction;
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&reinterpret_cast<PVOID&>(m_processEvent), reinterpret_cast<PVOID>(ProcessEventDetour));
		DetourTransactionCommit();
		m_detoured = true;
	}
}

void EventsComponent::DetachDetour()
{
	if (IsDetoured())
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&reinterpret_cast<PVOID&>(m_processEvent), reinterpret_cast<PVOID>(ProcessEventDetour));
		DetourTransactionCommit();
		m_detoured = false;
		m_processEvent = nullptr;
	}
}

void EventsComponent::ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result)
{
	if (m_processEvent && function)
	{
		auto preIt = m_preHooks.find(function->ObjectInternalInteger);
		PreEvent event(caller, function, params);

		if (preIt != m_preHooks.end())
		{
			for (const auto& preEvent : preIt->second)
			{
				preEvent(event);

				if (event.ShouldDetour())
				{
					m_processEvent(caller, function, params, result);
				}
			}
		}
		else if (event.ShouldDetour())
		{
			m_processEvent(caller, function, params, result);
		}

		auto postIt = m_postHooks.find(function->ObjectInternalInteger);

		if (postIt != m_postHooks.end())
		{
			for (const auto& postEvent : postIt->second)
			{
				postEvent(PostEvent(caller, function, params, result));
			}
		}
	}
}

bool EventsComponent::IsEventBlacklisted(uint32_t functionIndex)
{
	return (std::find(m_blacklisted.begin(), m_blacklisted.end(), functionIndex) != m_blacklisted.end());
}

void EventsComponent::BlacklistEvent(const std::string& function)
{
	UFunction* foundFunction = Instances.FindStaticFunction(function);

	if (foundFunction)
	{
		if (!IsEventBlacklisted(foundFunction->ObjectInternalInteger))
		{
			m_blacklisted.push_back(foundFunction->ObjectInternalInteger);
			std::sort(m_blacklisted.begin(), m_blacklisted.end());
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
		auto blackIt = std::find(m_blacklisted.begin(), m_blacklisted.end(), foundFunction->ObjectInternalInteger);

		if (blackIt != m_blacklisted.end())
		{
			m_blacklisted.erase(blackIt);
			std::sort(m_blacklisted.begin(), m_blacklisted.end());
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to whitelist function \"" + functionName + "\"!");
	}
}

void EventsComponent::HookEventPre(const std::string& functionName, const std::function<void(PreEvent&)>& preHook)
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

void EventsComponent::HookEventPre(uint32_t functionIndex, const std::function<void(PreEvent&)>& preHook)
{
	UObject* foundFunction = UObject::GObjObjects()->at(functionIndex);

	if (foundFunction && foundFunction->IsA<UFunction>())
	{
		if (m_preHooks.contains(functionIndex))
		{
			m_preHooks[functionIndex].push_back(preHook);
		}
		else
		{
			m_preHooks[functionIndex] = std::vector<std::function<void(PreEvent&)>>{ preHook };
		}
	}
	else
	{
		Console.Warning(GetNameFormatted() + "Warning: Failed to hook function at index \"" + std::to_string(functionIndex) + "\"!");
	}
}

void EventsComponent::HookEventPost(const std::string& functionName, const std::function<void(const PostEvent&)>& postHook)
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

void EventsComponent::HookEventPost(uint32_t functionIndex, const std::function<void(const PostEvent&)>& postHook)
{
	UObject* foundFunction = UObject::GObjObjects()->at(functionIndex);

	if (foundFunction && foundFunction->IsA<UFunction>())
	{
		if (m_postHooks.contains(functionIndex))
		{
			m_postHooks[functionIndex].push_back(postHook);
		}
		else
		{
			m_postHooks[functionIndex] = std::vector<std::function<void(const PostEvent&)>>{ postHook };
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

	Console.Write(GetNameFormatted() + std::to_string(m_blacklisted.size()) + " Backlisted Event(s)!");
	Console.Write(GetNameFormatted() + std::to_string(m_preHooks.size()) + " Pre-Hook(s) Initialized!");
	Console.Write(GetNameFormatted() + std::to_string(m_postHooks.size()) + " Post-Hook(s) Initialized!");
}

class EventsComponent Events{};