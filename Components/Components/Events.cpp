#include "Events.hpp"
#include "../Includes.hpp"

namespace CodeRed
{
	#define TRACE_PROCESSES
	#define TRACE_BLACKLISTS
	#define TRACE_GUARDS

	#define DETOUR_PROCEVENT
	#define DETOUR_PROCINTERNAL

	PreEvent::PreEvent() : m_caller(nullptr), m_function(nullptr), m_params(nullptr), m_type(EventTypes::Unknown), n_callable(true) {}

	PreEvent::PreEvent(class UObject* caller, class UFunction* function, void* params, EventTypes eventType, bool bCallable) : m_caller(caller), m_function(function), m_params(params), m_type(eventType), n_callable(bCallable) {}

	PreEvent::PreEvent(const PreEvent& preEvent) : m_caller(preEvent.m_caller), m_function(preEvent.m_function), m_params(preEvent.m_params), m_type(preEvent.m_type), n_callable(preEvent.n_callable) {}

	PreEvent::~PreEvent() {}

	EventTypes PreEvent::GetType() const
	{
		return m_type;
	}

	class UObject* PreEvent::Caller() const
	{
		return m_caller;
	}

	template <typename T> T* PreEvent::GetCaller() const
	{
		if (Caller())
		{
			return static_cast<T*>(Caller());
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
		if (Params())
		{
			return reinterpret_cast<T*>(Params());
		}

		return nullptr;
	}

	bool PreEvent::IsCallable() const
	{
		return n_callable;
	}

	void PreEvent::SetCallable(bool bCallable)
	{
		n_callable = bCallable;
	}

	PreEvent& PreEvent::operator=(const PreEvent& preEvent)
	{
		m_caller = preEvent.m_caller;
		m_function = preEvent.m_function;
		m_params = preEvent.m_params;
		m_type = preEvent.m_type;
		n_callable = preEvent.n_callable;
		return *this;
	}

	PostEvent::PostEvent() : m_result(nullptr) {}

	PostEvent::PostEvent(class UObject* caller, class UFunction* function, void* params, void* result, EventTypes eventType, bool bCallable) : m_result(result), PreEvent(caller, function, params, eventType, bCallable) {}

	PostEvent::PostEvent(const PostEvent& postEvent) : m_result(postEvent.m_result), PreEvent(postEvent) {}

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

	PostEvent& PostEvent::operator=(const PostEvent& postEvent)
	{
		m_result = postEvent.m_result;
		PreEvent::operator=(postEvent);
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

			Manager.OnTick(); // Required to process commands from different threads or commands with async delays.
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
		m_hooksSafe = false;
		m_blacklistSafe = false;
	}

	void EventsComponent::OnDestroy()
	{
		DetachDetours();
		OnCreate(); // Reset everything to default.
		m_blacklisted.clear();
		m_preHooks.clear();
		m_postHooks.clear();
		EventGuard::ClearRefStore();
	}

	bool EventsComponent::Initialize()
	{
		if (!IsInitialized())
		{
#ifdef WALKTHROUGH
			Console.Notify("\"Components\\Components\\Events.cpp -> EventsComponent::Initialize()\". Looks like you forgot to check this file, the only thing here are demo functions hooks!");
			return false;
#endif

			if (AttachDetours())
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
#ifdef DETOUR_PROCEVENT
		bool eventValid = m_processEvent.IsAttached();
#else
		eventValid = true;
#endif

#ifdef DETOUR_PROCINTERNAL
		bool internalValid = m_processInternal.IsAttached();
#else
		bool internalValid = true;
#endif

		return (eventValid && internalValid);
	}

	bool EventsComponent::AttachDetours()
	{
#ifdef WALKTHROUGH
		Console.Notify("\"Components\\Components\\Events.cpp -> EventsComponent::AttachDetours()\". Looks like you forgot to check this file, you need to setup hooking ProcessEvent!");
		return false;
#endif

#ifdef DETOUR_PROCEVENT
		if (!m_processEvent.IsAttached())
		{
			// You can use either a pattern for Process Event or its place in the VfTable index (not both).	
			//Attach(reinterpret_cast<uintptr_t*>(unrealVTable[0])); // Index method.
			//Attach(reinterpret_cast<uintptr_t*>(Memory::FindPattern(ProcessEvent_Pattern, ProcessEvent_Mask))); // Find pattern method.

			void** vfTable = reinterpret_cast<void**>(UObject::StaticClass()->VfTableObject.Dummy);

			if (vfTable && m_processEvent.Attach(reinterpret_cast<uintptr_t*>(vfTable[69]), ProcessEventHook))
			{
				m_processEvent.Enable();
			}
			else
			{
				Console.Error("[Events Component] (AttachDetours) Failed to hook process event!");
			}
		}
#endif

#ifdef DETOUR_PROCINTERNAL
		if (!m_processInternal.IsAttached())
		{
			UFunction* function = Instances.FindStaticFunction("Function Engine.HUD.PostRender"); // The "Func" pointer here is a pointer to the function "ProcessInternal".

			if (function && function->Func.Dummy)
			{
				if (m_processInternal.Attach(reinterpret_cast<uintptr_t*>(function->Func.Dummy), ProcessInternalHook))
				{
					m_processInternal.Enable();
				}
				else
				{
					Console.Error("[Events Component] (AttachDetours) Failed to hook process internal!");
				}
			}
		}
#endif

		if (AreDetoursAttached())
		{
			m_hooksSafe = true;
			m_blacklistSafe = true;
		}

		return AreDetoursAttached();
	}

	void EventsComponent::DetachDetours()
	{
		m_processEvent.Detach();
		m_processInternal.Detach();
		m_hooksSafe = false;
		m_blacklistSafe = false;
	}

	void EventsComponent::ProcessEventHook(class UObject* caller, class UFunction* function, void* params, void* result)
	{
#ifdef DETOUR_PROCEVENT
		if (m_processEvent.IsAttached())
		{
			if (CanProcessHook(caller, function, EventTypes::ProcessEvent))
			{
				EventGuard eventGuard(caller, function);

				if (eventGuard.IsFirst())
				{
					ProcessResults processResult = ProcessBefore(caller, function, params, result, EventTypes::ProcessEvent);

					if (CanCallHook(function, processResult, EventTypes::ProcessEvent))
					{
						m_processEvent.Call<HookTypes::ProcessEvent>()(caller, function, params, result);
					}
#ifdef TRACE_BLACKLISTS
					else if (function)
					{
						Console.Error("[Events Component] (ProcessEventHook) Don't want to call function \"" + function->GetFullName() + "\"!");
					}
#endif

					ProcessAfter(caller, function, params, result, EventTypes::ProcessEvent);
					return;
				}
#ifdef TRACE_GUARDS
				else if (function)
				{
					Console.Error("[Events Component] (ProcessEventHook) Guard isn't safe for \"" + function->GetFullName() + "\"!");
				}
#endif
			}
			else if (IsBlacklisted(function))
			{
#ifdef TRACE_BLACKLISTS
				if (function)
				{
					Console.Error("[Events Component] (ProcessEventHook) Blacklist found for \"" + function->GetFullName() + "\"!");
				}
#endif

				return;
			}

			m_processEvent.Call<HookTypes::ProcessEvent>()(caller, function, params, result);
		}
#endif
	}

	void EventsComponent::ProcessInternalHook(class UObject* caller, struct FFrame& frame, void* result)
	{
#ifdef DETOUR_PROCINTERNAL
		if (m_processInternal.IsAttached())
		{
			UFunction* function = static_cast<UFunction*>(frame.Node);
			void* params = frame.Locals;

			if (CanProcessHook(caller, function, EventTypes::ProcessInternal))
			{
				EventGuard eventGuard(caller, function);

				if (eventGuard.IsFirst())
				{
					ProcessResults processResult = ProcessBefore(caller, function, params, result, EventTypes::ProcessInternal);

					if (CanCallHook(function, processResult, EventTypes::ProcessInternal))
					{
						m_processInternal.Call<HookTypes::ProcessInternal>()(caller, frame, result);
					}
#ifdef TRACE_BLACKLISTS
					else if (function)
					{
						Console.Error("[Events Component] (ProcessInternalHook) Cannot call function for \"" + function->GetFullName() + "\"!");
					}
#endif

					ProcessAfter(caller, function, params, result, EventTypes::ProcessInternal);
					return;
				}
#ifdef TRACE_GUARDS
				else if (function)
				{
					Console.Error("[Events Component] (ProcessInternalHook) Guard isn't safe for \"" + function->GetFullName() + "\"!");
				}
#endif
			}
			else if (IsBlacklisted(function))
			{
#ifdef TRACE_BLACKLISTS
				if (function)
				{
					Console.Error("[Events Component] (ProcessInternalHook) Blacklist found for \"" + function->GetFullName() + "\"!");
				}
#endif

				return;
			}

			m_processInternal.Call<HookTypes::ProcessInternal>()(caller, frame, result);
		}
#endif
	}

	bool EventsComponent::IsBlacklisted(class UFunction* function)
	{
		if (function)
		{
			return IsBlacklisted(function->ObjectInternalInteger);
		}

		return false;
	}

	bool EventsComponent::IsBlacklisted(uint32_t functionIndex)
	{
		if (m_blacklistSafe)
		{
			return (std::find(m_blacklisted.begin(), m_blacklisted.end(), functionIndex) != m_blacklisted.end());
		}

		return false;
	}

	bool EventsComponent::IsPreHooked(class UFunction* function)
	{
		if (function)
		{
			return IsPreHooked(function->ObjectInternalInteger);
		}

		return false;
	}

	bool EventsComponent::IsPreHooked(uint32_t functionIndex)
	{
		if (m_hooksSafe)
		{
			return m_preHooks.contains(functionIndex);
		}

		return false;
	}

	bool EventsComponent::IsPostHooked(class UFunction* function)
	{
		if (function)
		{
			return IsPostHooked(function->ObjectInternalInteger);
		}

		return false;
	}

	bool EventsComponent::IsPostHooked(uint32_t functionIndex)
	{
		if (m_hooksSafe)
		{
			return m_postHooks.contains(functionIndex);
		}

		return false;
	}

	void EventsComponent::BlacklistEvent(const std::string& function)
	{
		UFunction* foundFunction = Instances.FindStaticFunction(function);

		if (foundFunction)
		{
			bool isBlacklisted = IsBlacklisted(foundFunction);

			if (!isBlacklisted)
			{
				m_blacklistSafe = false;
				m_blacklisted.push_back(foundFunction->ObjectInternalInteger);
				std::sort(m_blacklisted.begin(), m_blacklisted.end());
				m_blacklistSafe = true;
			}
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
			m_hooksSafe = false;
			auto blackIt = std::find(m_blacklisted.begin(), m_blacklisted.end(), foundFunction->ObjectInternalInteger);

			if (blackIt != m_blacklisted.end())
			{
				m_blacklisted.erase(blackIt);
				std::sort(m_blacklisted.begin(), m_blacklisted.end());
			}

			m_hooksSafe = true;
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
				m_hooksSafe = false;
				m_preHooks[functionIndex].push_back(preHook);
				m_hooksSafe = true;
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
				m_hooksSafe = false;
				m_postHooks[functionIndex].push_back(postHook);
				m_hooksSafe = true;
			}
			else
			{
				Console.Warning("Warning: Failed to hook function at index \"" + std::to_string(functionIndex) + "\"!");
			}
		}
	}

	bool EventsComponent::CanCallHook(class UFunction* function, ProcessResults processResult, EventTypes eventType)
	{
		if (eventType != EventTypes::CallFunction) // CallFunction is limited to what you can hook, including its parameters, so this isn't supported fully for now.
		{
			return ((processResult != ProcessResults::Blacklist) && (processResult != ProcessResults::CallbackBlacklist) && !IsBlacklisted(function));
		}

		return true;
	}

	bool EventsComponent::CanProcessHook(class UObject* caller, class UFunction* function, EventTypes eventType)
	{
		if ((eventType != EventTypes::Unknown) && function)
		{
			return (IsPreHooked(function) || IsPostHooked(function));
		}

		return false;
	}

	ProcessResults EventsComponent::ProcessBefore(class UObject* caller, class UFunction* function, void* params, void* result, EventTypes eventType)
	{
		ProcessResults processResult = ProcessResults::Should;

		if (function)
		{
			if (IsPreHooked(function->ObjectInternalInteger))
			{
				processResult = ProcessResults::ShouldCallback; // At least one callback was found.
				PreEvent preEvent(caller, function, params, eventType);

				for (const auto& functionHook : m_preHooks[function->ObjectInternalInteger])
				{
					functionHook(preEvent);

					if (!preEvent.IsCallable() && (eventType != EventTypes::CallFunction)) // Can't blacklist from CallFunction or else it will crash, would need to hook "UObject::SkipFunction" and call that instead.
					{
#ifdef TRACE_BLACKLISTS
						Console.Notify("[Events Component] (ProcessBefore) CallbackBlacklist!");
#endif
						processResult = ProcessResults::CallbackBlacklist; // Callback was found, and it said we should blacklist it.
					}
				}
			}

			if ((processResult != ProcessResults::CallbackBlacklist) && (eventType != EventTypes::CallFunction)) // Save ourselves some function calls and checks, it's already blacklisted.
			{
				if (IsBlacklisted(function->ObjectInternalInteger))
				{
					if (processResult == ProcessResults::ShouldCallback)
					{
#ifdef TRACE_BLACKLISTS
						Console.Notify("[Events Component] (ProcessBefore) CallbackBlacklist!");
#endif
						processResult = ProcessResults::CallbackBlacklist;
					}
					else
					{
#ifdef TRACE_BLACKLISTS
						Console.Notify("[Events Component] (ProcessBefore) Blacklist!");
#endif
						processResult = ProcessResults::Blacklist;
					}
				}
			}
		}

		return processResult;
	}

	ProcessResults EventsComponent::ProcessAfter(class UObject* caller, class UFunction* function, void* params, void* result, EventTypes eventType)
	{
		ProcessResults processResult = ProcessResults::Should;

		if (function)
		{
			if (IsPostHooked(function->ObjectInternalInteger))
			{
				processResult = ProcessResults::ShouldCallback; // At least one callback was found.
				PostEvent postEvent(caller, function, params, result, eventType);

				for (const auto& functionHook : m_postHooks[function->ObjectInternalInteger])
				{
					functionHook(postEvent);

					if (!postEvent.IsCallable() && (eventType != EventTypes::CallFunction)) // Can't blacklist from CallFunction or else it will crash, would need to hook "UObject::SkipFunction" and call that instead.
					{
#ifdef TRACE_BLACKLISTS
						Console.Notify("[Events Component] (ProcessAfter) CallbackBlacklist!");
#endif
						processResult = ProcessResults::CallbackBlacklist; // Obviously the function is already called at this point, but we don't want to return that it can be called anyway if it is blacklisted.
					}
				}

				if ((processResult != ProcessResults::CallbackBlacklist) && (eventType != EventTypes::CallFunction)) // Save ourselves some function calls and checks, it's already blacklisted.
				{
					if (IsBlacklisted(function->ObjectInternalInteger))
					{
						if (processResult == ProcessResults::ShouldCallback)
						{
#ifdef TRACE_BLACKLISTS
							Console.Notify("[Events Component] (ProcessAfter) CallbackBlacklist!");
#endif
							processResult = ProcessResults::CallbackBlacklist;
						}
						else
						{
#ifdef TRACE_BLACKLISTS
							Console.Notify("[Events Component] (ProcessAfter) Blacklist!)");
#endif
							processResult = ProcessResults::Blacklist;
						}
					}
				}
			}
		}

		return processResult;
	}

	class EventsComponent Events;
}