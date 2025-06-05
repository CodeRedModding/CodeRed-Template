#pragma once
#include "../Component.hpp"

namespace CodeRed
{
	// Useful for debugging purposes to track what happened to certain function hooks or callbacks.
	enum class ProcessResults : uint8_t
	{
		Should,					// The original function hook should be called, but no callbacks were found.
		Blacklist,				// The original function shouldn't be called, it is blacklisted and no callbacks were found.
		ShouldCallback,			// The original function hook should be called, callbacks were found and triggered.
		CallbackBlacklist		// The original function shouldn't be called, but callbacks were found and triggered.
	};

	class PreEvent
	{
	protected:
		class UObject* m_caller;		// The calling object.
		class UFunction* m_function;	// The unreal function being processed.
		void* m_params;					// If parameters are provided in the function, this would be a pointer to a struct to the params.
		EventTypes m_type;				// The origin type of this function call.
		bool n_callable;				// If we should end up calling this function, or block it.

	public:
		PreEvent();
		PreEvent(class UObject* caller, class UFunction* function, void* params, EventTypes eventType, bool bCallable = true);
		PreEvent(const PreEvent& preEvent);
		~PreEvent();

	public:
		EventTypes GetType() const;
		class UObject* Caller() const;
		template <typename T> T* GetCaller() const;
		class UFunction* Function() const;
		void* Params() const;
		template <typename T> T* GetParams() const;
		bool IsCallable() const;
		void SetCallable(bool bCallable);

	public:
		PreEvent& operator=(const PreEvent& preEvent);
	};

	class PostEvent : public PreEvent
	{
	private:
		void* m_result;					// Unused result after being processed, can contain arbitrary data.

	public:
		PostEvent();
		PostEvent(class UObject* caller, class UFunction* function, void* params, void* result, EventTypes eventType, bool bCallable = true);
		PostEvent(const PostEvent& postEvent);
		~PostEvent();

	public:
		void* Result() const;
		template <typename T> T* GetResult() const;

	public:
		PostEvent& operator=(const PostEvent& postEvent);
	};

	class HooksComponent : public Component
	{
	public:
		HooksComponent();
		~HooksComponent() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;
		bool Initialize() override;

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
	private: // Internal Hooks
		static inline FunctionHook m_processEvent;
		static inline FunctionHook m_processInternal;

	private: // Custom Callbacks & Function Blacklist.
		static inline std::atomic<bool> m_hooksSafe; // Used to prevent thread racing issues related to adding or removing data from the maps.
		static inline std::atomic<bool> m_blacklistSafe; // Similar thread racing issue preventive measure as "m_hooksSafe".
		static inline std::vector<uint32_t> m_blacklisted; // Blacklisted functions internal integer.
		static inline std::map<uint32_t, std::vector<std::function<void(PreEvent&)>>> m_preHooks; // Hooked functions internal integer and bound function.
		static inline std::map<uint32_t, std::vector<std::function<void(const PostEvent&)>>> m_postHooks; // Hooked functions internal integer and bound function.

	public:
		EventsComponent();
		~EventsComponent() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;
		bool Initialize() override;

	public:
		static bool AreDetoursAttached();
		static bool AttachDetours(); // Redirects the games functions to our own void, for us to manually process later and trigger callbacks.
		static void DetachDetours(); // Called by the deconstuctor, necessary for if your DLL gets intentionally (or unintentionally) unloaded before your game exits.
		static void ProcessEventHook(class UObject* caller, class UFunction* function, void* params, void* result); // ProcessEvent gets redirected to this function.
		static void ProcessInternalHook(class UObject* caller, struct FFrame& frame, void* result); // ProcessInternal gets redirected to this function.

	public:
		static bool IsBlacklisted(class UFunction* function);
		static bool IsBlacklisted(uint32_t functionIndex);
		static bool IsPreHooked(class UFunction* function);
		static bool IsPreHooked(uint32_t functionIndex);
		static bool IsPostHooked(class UFunction* function);
		static bool IsPostHooked(uint32_t functionIndex);
		static void BlacklistEvent(const std::string& functionName);
		static void WhitelistEvent(const std::string& functionName);
		static void HookEventPre(const std::string& functionName, const std::function<void(PreEvent&)>& preHook);
		static void HookEventPre(uint32_t functionIndex, const std::function<void(PreEvent&)>& preHook);
		static void HookEventPost(const std::string& functionName, const std::function<void(const PostEvent&)>& postHook);
		static void HookEventPost(uint32_t functionIndex, const std::function<void(const PostEvent&)>& postHook);

	private:
		static bool CanCallHook(class UFunction* function, ProcessResults processResult, EventTypes eventType);
		static bool CanProcessHook(class UObject* caller, class UFunction* function, EventTypes eventType);
		static ProcessResults ProcessBefore(class UObject* caller, class UFunction* function, void* params, void* result, EventTypes eventType);
		static ProcessResults ProcessAfter(class UObject* caller, class UFunction* function, void* params, void* result, EventTypes eventType);
	};

	extern class EventsComponent Events;
}