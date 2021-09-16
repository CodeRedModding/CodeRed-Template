#pragma once
#include "../Component.hpp"

class PreEvent
{
protected:
	class UObject* PE_Caller;		// The Calling class/unreal object.
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
	bool Detour() const;
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
	void GameViewPortKeyPress(PreEvent& event);
}

typedef void(*ProcessEventType)(class UObject*, class UFunction*, void*, void*);

class EventsComponent : public Component
{
private:
	typedef void(*PreEventType)(PreEvent&);
	typedef void(*PostEventType)(const PostEvent&);
	static inline std::map<int32_t, std::vector<PreEventType>> PreHookedEvents; // Hooked function's internal integer and arguments.
	static inline std::map<int32_t, std::vector<PostEventType>> PostHookedEvents; // Hooked function's internal integer and arguments.
	static inline std::vector<int32_t> BlacklistedEvents; // Blacklisted function's internal integer.

public:
	inline static ProcessEventType ProcessEvent; // This is the function that we send our detoured process event to.

public:
	EventsComponent();
	~EventsComponent();

public:
	static void ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result); // Process event gets detoured to this function, then we manually proxy it through to "ProcessEvent".
	void BlacklistEvent(const std::string& functionFullName);
	void WhitelistEvent(const std::string& functionFullName);
	void HookEventPre(const std::string& functionFullName, const PreEventType& eventType);
	void HookEventPost(const std::string& functionFullName, const PostEventType& eventType);
	void Initialize(); // Initializes binding events to voids.
};

extern class EventsComponent Events;