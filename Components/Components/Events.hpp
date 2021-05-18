#pragma once
#include "../Component.hpp"

namespace Functions
{
	void HUDPostRender(class UObject* caller, class UFunction* function, void* params);
	void HUDPostRenderPost(UObject* caller, class UFunction* function, void* params, void* result);
	void GameViewPortPostRender(class UObject* caller, class UFunction* function, void* params);
	void GFxDataMainMenuAdded(class UObject* caller, class UFunction* function, void* params);
	void PlayerControllerTick(class UObject* caller, class UFunction* function, void* params);
	void GameViewPortKeyPress(class UObject* caller, class UFunction* function, void* params);
}

typedef void(*PreEventType)(class UObject*, class UFunction*, void*);
typedef void(*PostEventType)(class UObject*, class UFunction*, void*, void*);
typedef void(*ProcessEventType)(class UObject*, class UFunction*, void*, void*);

class EventsComponent : public Component
{
private:
	static inline std::map<std::string, int32_t> FunctionIndexMap; // Function's internal integer and it's full name.
	static inline std::map<int32_t, PreEventType> PreHookedEvents; // Hooked function's internal integer and arguments.
	static inline std::map<int32_t, PostEventType> PostHookedEvents; // Hooked function's internal integer and arguments.
	static inline std::vector<int32_t> BlackHookedEvents; // Blacklisted function's internal integer.
	static inline bool LogFunctions;

public:
	inline static ProcessEventType ProcessEvent; // This is the function that we send our detoured process event to.

public:
	EventsComponent();
	~EventsComponent();

public:
	void ToggleLogFunctions(const bool& bLog); // Toggle logging functions to the console.
	void MapFunctions(); // Automatically finds functions internal integer from their full name and map them out.
	static void ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result); // Process event gets detoured to this function, then we manually proxy it through to "ProcessEvent".
	void BlacklistEvent(const std::string& functionFullName);
	void HookEventPre(const std::string& functionFullName, const PreEventType& eventType);
	void HookEventPost(const std::string& functionFullName, const PostEventType& eventType);
	void Initialize(); // Initializes binding events to voids.
};

extern class EventsComponent Events;