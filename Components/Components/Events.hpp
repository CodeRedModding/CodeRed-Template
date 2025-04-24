#pragma once
#include "../Component.hpp"

namespace CodeRed
{
	typedef void(*ProcessEventType)(class UObject*, class UFunction*, void*, void*); // Calling object, function, structure pointer with parameters, unused result

	class PreEvent
	{
	protected:
		class UObject* m_caller;		// The calling class/unreal object.
		class UFunction* m_function;	// Unreal function, used as a reference in Process Event to call the native function.
		void* m_params;					// If params are provided in the function, this would be a pointer to a struct with said params in them.
		bool m_detour;					// If we should detour this event or not through Process Event, this is entirely up to us and not the game.

	public:
		PreEvent();
		PreEvent(class UObject* caller, class UFunction* function, void* params, bool bDetour = true);
		PreEvent(const PreEvent& preEvent);
		~PreEvent();

	public:
		class UObject* Caller() const;
		template <typename T> T* GetCaller() const;
		class UFunction* Function() const;
		void* Params() const;
		template <typename T> T* GetParams() const;
		bool ShouldDetour() const;
		void SetDetour(bool bDetour);

	public:
		PreEvent& operator=(const PreEvent& other);
	};

	class PostEvent : public PreEvent
	{
	private:
		void* m_result;					// Unused result after going through Process Event.

	public:
		PostEvent();
		PostEvent(class UObject* caller, class UFunction* function, void* params, void* result);
		PostEvent(const PostEvent& postEvent);
		~PostEvent();

	public:
		void* Result() const;
		template <typename T> T* GetResult() const;

	public:
		PostEvent& operator=(const PostEvent& other);
	};

	class HooksComponent : public Component
	{
	public:
		HooksComponent();
		~HooksComponent() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;

	public: // Pre Hooks
		void HUDPostRender(PreEvent& event);
		void GameViewPortPostRender(PreEvent& event);
		void PlayerControllerTick(PreEvent& event);

	public: // Post Hooks
		void GameViewPortKeyPress(const PostEvent& event);
	};

	extern class HooksComponent Hooks;

	// Manages everything related to hooking functions form Process Event, as well as detouring.
	class EventsComponent : public Component
	{
	private:
		static inline bool m_detoured;
		static inline ProcessEventType m_processEvent;

	private:
		static inline std::atomic<bool> m_hookSafe;
		static inline std::vector<uint32_t> m_blacklisted; // Blacklisted functions internal integer.
		static inline std::map<uint32_t, std::vector<std::function<void(PreEvent&)>>> m_preHooks; // Hooked functions internal integer and bound function.
		static inline std::map<uint32_t, std::vector<std::function<void(const PostEvent&)>>> m_postHooks; // Hooked functions internal integer and bound function.

	public:
		EventsComponent();
		~EventsComponent() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;

	public:
		static bool IsDetoured();
		static void AttachDetour(const ProcessEventType& detourFunction); // Redirects the process event virtual function to our own void, for us to manually process later to the typedef.
		static void DetachDetour(); // Called by the deconstuctor, necessary for if your DLL gets intentionally (or unintentionally) unloaded before your game exits.
		static void ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result); // Process event gets detoured to this function, then we manually proxy it through to "ProcessEvent".

	private:
		static bool ProcessBefore(class UObject* caller, class UFunction* function, void* params, void* result);
		static void ProcessAfter(class UObject* caller, class UFunction* function, void* params, void* result);

	public:
		static bool IsEventBlacklisted(uint32_t functionIndex);
		static void BlacklistEvent(const std::string& functionName);
		static void WhitelistEvent(const std::string& functionName);
		static void HookEventPre(const std::string& functionName, const std::function<void(PreEvent&)>& preHook);
		static void HookEventPre(uint32_t functionIndex, const std::function<void(PreEvent&)>& preHook);
		static void HookEventPost(const std::string& functionName, const std::function<void(const PostEvent&)>& postHook);
		static void HookEventPost(uint32_t functionIndex, const std::function<void(const PostEvent&)>& postHook);
		void Initialize(); // Initializes hooking events to functions.
	};

	extern class EventsComponent Events;
}