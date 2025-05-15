#include "Events.hpp"
#include "../Includes.hpp"

namespace CodeRed
{
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

	HooksComponent::HooksComponent() : Component("Hooks", "Manages hooked functions and their arguments.") { OnCreate(); }

	HooksComponent::~HooksComponent() { OnDestroy(); }

	void HooksComponent::OnCreate() {}

	void HooksComponent::OnDestroy() {}

	bool HooksComponent::Initialize()
	{
		if (!IsInitialized())
		{
			Console.Success(GetNameFormatted() + "Initialized!");
			SetInitialized(true);
		}

		return IsInitialized();
	}

	// Pre Hooks

	void HooksComponent::HUDPostRender(PreEvent& event)
	{
		if (event.Caller())
		{
			AHUD* hud = event.GetCaller<AHUD>();
			Instances.SetHUD(hud);

			if (hud->Canvas)
			{
				Manager.OnCanvasDraw(hud->Canvas);
			}
		}
	}

	void HooksComponent::GameViewPortPostRender(PreEvent& event)
	{
		if (event.Caller())
		{
			Instances.SetGameViewportClient(event.GetCaller<UGameViewportClient>());
		}
	}

	void HooksComponent::PlayerControllerTick(PreEvent& event)
	{
		if (event.Caller())
		{
			Instances.SetPlayerController(event.GetCaller<APlayerController>());
		}
	}

	// Post Hooks

	void HooksComponent::GameViewPortKeyPress(const PostEvent& event)
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

	class HooksComponent Hooks;

	EventsComponent::EventsComponent() : Component("Events", "Manages function hooks and process event.") { OnCreate(); }

	EventsComponent::~EventsComponent() { OnDestroy(); }

	void EventsComponent::OnCreate()
	{
		m_detoured = false;
		m_processEvent = nullptr;
		m_hookSafe = false;
	}

	void EventsComponent::OnDestroy()
	{
		DetachDetour();
		OnCreate();
		m_blacklisted.clear();
		m_preHooks.clear();
		m_postHooks.clear();
	}

	bool EventsComponent::Initialize()
	{
		if (!IsInitialized())
		{
#ifdef WALKTHROUGH
			Console.Notify("\"Components\\Components\\Events.cpp -> EventsComponent::Initialize()\". Looks like you forgot to check this file, the only thing here are demo functions hooks!");
			return false;
#endif
			// You can use either a pattern for Process Event or its place in the VfTable index (not both).

			//void** unrealVTable = reinterpret_cast<void**>(UObject::StaticClass()->VfTableObject.Dummy);
			//AttachDetour(reinterpret_cast<ProcessEventType>(unrealVTable[0])); // Index method.
			//AttachDetour(reinterpret_cast<ProcessEventType>(Memory::FindPattern(ProcessEvent_Pattern, ProcessEvent_Mask))); // Find pattern method.

			if (AreDetoursAttached())
			{
				// Example functions only, you will need to function scan in your game for your own to hook!
				BlacklistEvent("Function Engine.Tracker.UploadData");

				// Pre Hooks

				HookEventPre("Function Engine.HUD.PostRender", [&](PreEvent& event) { Hooks.HUDPostRender(event); });
				HookEventPre("Function Engine.GameViewportClient.PostRender", [&](PreEvent& event) { Hooks.GameViewPortPostRender(event); });
				HookEventPre("Function Engine.PlayerController.PlayerTick", [&](PreEvent& event) { Hooks.PlayerControllerTick(event); });

				// Post Hooks

				HookEventPost("Function Engine.GameViewportClient.HandleKeyPress", [&](const PostEvent& event) { Hooks.GameViewPortKeyPress(event); });

				Console.Write(GetNameFormatted() + std::to_string(m_blacklisted.size()) + " Backlisted Event(s)!");
				Console.Write(GetNameFormatted() + std::to_string(m_preHooks.size()) + " Pre-Hook(s) Initialized!");
				Console.Write(GetNameFormatted() + std::to_string(m_postHooks.size()) + " Post-Hook(s) Initialized!");
				Console.Success(GetNameFormatted() + "Initialized!");
				SetInitialized(true);
			}
		}

		return IsInitialized();
	}

	bool EventsComponent::AreDetoursAttached()
	{
		return (m_detoured && m_processEvent);
	}

	void EventsComponent::AttachDetour(const ProcessEventType& detourFunction)
	{
		if (!AreDetoursAttached())
		{
			m_processEvent = detourFunction;
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&reinterpret_cast<PVOID&>(m_processEvent), reinterpret_cast<PVOID>(ProcessEventDetour));
			DetourTransactionCommit();
			m_detoured = true;
			m_hookSafe = true;
		}
	}

	void EventsComponent::DetachDetour()
	{
		if (AreDetoursAttached())
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&reinterpret_cast<PVOID&>(m_processEvent), reinterpret_cast<PVOID>(ProcessEventDetour));
			DetourTransactionCommit();
			m_detoured = false;
			m_processEvent = nullptr;
			m_hookSafe = false;
		}
	}

	void EventsComponent::ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result)
	{
		if (m_processEvent)
		{
			if (function)
			{
				bool shouldDetour = ProcessBefore(caller, function, params, result);

				if (shouldDetour)
				{
					m_processEvent(caller, function, params, result);
				}

				ProcessAfter(caller, function, params, result);
			}
			else
			{
				m_processEvent(caller, function, params, result);
			}
		}
	}

	bool EventsComponent::ProcessBefore(class UObject* caller, class UFunction* function, void* params, void* result)
	{
		if (function)
		{
			bool shouldDetour = true;

			if (m_hookSafe && m_preHooks.contains(function->ObjectInternalInteger))
			{
				PreEvent preEvent(caller, function, params);

				for (const auto& preHook : m_preHooks[function->ObjectInternalInteger])
				{
					preHook(preEvent);

					if (!preEvent.ShouldDetour())
					{
						shouldDetour = false;
					}
				}
			}

			if (shouldDetour)
			{
				if (m_hookSafe && IsEventBlacklisted(function->ObjectInternalInteger))
				{
					return false;
				}
			}

			return shouldDetour;
		}

		return true;
	}

	void EventsComponent::ProcessAfter(class UObject* caller, class UFunction* function, void* params, void* result)
	{
		if (function)
		{
			if (m_hookSafe && m_postHooks.contains(function->ObjectInternalInteger))
			{
				PostEvent postEvent(caller, function, params, result);

				for (const auto& postHook : m_postHooks[function->ObjectInternalInteger])
				{
					postHook(postEvent);
				}
			}
		}
	}

	bool EventsComponent::IsEventBlacklisted(uint32_t functionIndex)
	{
		m_hookSafe = false;
		bool blacklisted = (std::find(m_blacklisted.begin(), m_blacklisted.end(), functionIndex) != m_blacklisted.end());
		m_hookSafe = true;
		return blacklisted;
	}

	void EventsComponent::BlacklistEvent(const std::string& function)
	{
		UFunction* foundFunction = Instances.FindStaticFunction(function);

		if (foundFunction)
		{
			m_hookSafe = false;

			if (!IsEventBlacklisted(foundFunction->ObjectInternalInteger))
			{
				m_blacklisted.push_back(foundFunction->ObjectInternalInteger);
				std::sort(m_blacklisted.begin(), m_blacklisted.end());
			}

			m_hookSafe = true;
		}
		else
		{
			Console.Warning("Warning: Failed to blacklist function \"" + function + "\"!");
		}
	}

	void EventsComponent::WhitelistEvent(const std::string& functionName)
	{
		UFunction* foundFunction = Instances.FindStaticFunction(functionName);

		if (foundFunction)
		{
			m_hookSafe = false;
			auto blackIt = std::find(m_blacklisted.begin(), m_blacklisted.end(), foundFunction->ObjectInternalInteger);

			if (blackIt != m_blacklisted.end())
			{
				m_blacklisted.erase(blackIt);
				std::sort(m_blacklisted.begin(), m_blacklisted.end());
			}

			m_hookSafe = true;
		}
		else
		{
			Console.Warning("Warning: Failed to whitelist function \"" + functionName + "\"!");
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
			Console.Warning("Warning: Failed to hook function \"" + functionName + "\"!");
		}
	}

	void EventsComponent::HookEventPre(uint32_t functionIndex, const std::function<void(PreEvent&)>& preHook)
	{
		if (UObject::GObjObjects())
		{
			UObject* foundFunction = UObject::GObjObjects()->at(functionIndex);

			if (foundFunction && foundFunction->IsA<UFunction>())
			{
				m_hookSafe = false;

				if (m_preHooks.contains(functionIndex))
				{
					m_preHooks[functionIndex].push_back(preHook);
				}
				else
				{
					m_preHooks[functionIndex] = std::vector<std::function<void(PreEvent&)>>{ preHook };
				}

				m_hookSafe = true;
			}
			else
			{
				Console.Warning("Warning: Failed to hook function at index \"" + std::to_string(functionIndex) + "\"!");
			}
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
			Console.Warning("Warning: Failed to hook function \"" + functionName + "\"!");
		}
	}

	void EventsComponent::HookEventPost(uint32_t functionIndex, const std::function<void(const PostEvent&)>& postHook)
	{
		if (UObject::GObjObjects())
		{
			UObject* foundFunction = UObject::GObjObjects()->at(functionIndex);

			if (foundFunction && foundFunction->IsA<UFunction>())
			{
				m_hookSafe = false;

				if (m_postHooks.contains(functionIndex))
				{
					m_postHooks[functionIndex].push_back(postHook);
				}
				else
				{
					m_postHooks[functionIndex] = std::vector<std::function<void(const PostEvent&)>>{ postHook };
				}

				m_hookSafe = true;
			}
			else
			{
				Console.Warning("Warning: Failed to hook function at index \"" + std::to_string(functionIndex) + "\"!");
			}
		}
	}

	class EventsComponent Events;
}