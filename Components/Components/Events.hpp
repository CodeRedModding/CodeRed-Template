#pragma once
#include "../Component.hpp"

class PreEvent
{
protected:
	class UObject* PE_Caller;		// The calling class/unreal object.
	class UFunction* PE_Function;	// Unreal function, used as a reference in Process Event to call the native function.
	void* PE_Params;				// If params are provided in the function, this would be a pointer to a struct with said params in them.
	bool PE_Detour;					// If we should detour this event or not through Process Event, this is entirely up to us and not the game.

public:
	PreEvent();
	PreEvent(class UObject* caller, class UFunction* function, void* params);
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
	PreEvent operator=(const PreEvent& other);
};

class PostEvent : public PreEvent
{
private:
	void* PE_Result;				// Unused result after going through Process Event.

public:
	PostEvent();
	PostEvent(class UObject* caller, class UFunction* function, void* params, void* result);
	~PostEvent();

public:
	void* Result() const;
	template <typename T> T* GetResult() const;

public:
	PostEvent operator=(const PostEvent& other);
};

namespace Hooks
{
	void HUDPostRender(PreEvent& event);
	void HUDPostRenderPost(const PostEvent& event);
	void GameViewPortPostRender(PreEvent& event);
	void GFxDataMainMenuAdded(PreEvent& event);
	void PlayerControllerTick(PreEvent& event);
	void GameViewPortKeyPress(const PostEvent& event);
}

typedef void(*ProcessEventType)(class UObject*, class UFunction*, void*, void*); // Calling object, function, structure pointer with parameters, unused result

// Manages everything related to hooking functions form Process Event, as well as detouring.
class EventsComponent : public Component
{
private:
	static inline bool Detoured;
	static inline ProcessEventType ProcessEvent;
	static inline std::map<uint32_t, std::vector<std::function<void(PreEvent&)>>> PreHookedEvents; // Hooked functions internal integer and bound function.
	static inline std::map<uint32_t, std::vector<std::function<void(const PostEvent&)>>> PostHookedEvents; // Hooked functions internal integer and bound function.
	static inline std::vector<uint32_t> BlacklistedEvents; // Blacklisted functions internal integer.

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
	static bool IsEventBlacklisted(uint32_t functionIndex);
	
public:
	void BlacklistEvent(const std::string& functionName);
	void WhitelistEvent(const std::string& functionName);
	void HookEventPre(const std::string& functionName, std::function<void(PreEvent&)> preHook);
	void HookEventPre(uint32_t functionIndex, std::function<void(PreEvent&)> preHook);
	void HookEventPost(const std::string& functionName, std::function<void(const PostEvent&)> postHook);
	void HookEventPost(uint32_t functionIndex, std::function<void(const PostEvent&)> postHook);
	void Initialize(); // Initializes hooking events to functions.
};

extern class EventsComponent Events;