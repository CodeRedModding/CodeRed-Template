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
	inline static bool LogFunctions;
	inline static std::map<int32_t, PreEventType> PreHookedEvents; // Hooked function's internal integer and arguments.
	inline static std::map<int32_t, PostEventType> PostHookedEvents; // Hooked function's internal integer and arguments.
	inline static std::map<std::string, PreEventType> PreEvents; // Function's full name and arguments that need to be searched for in "FindHookedEvents".
	inline static std::map<std::string, PostEventType> PostEvents; // Function's full name and arguments that need to be searched for in "FindHookedEvents".

public:
	inline static ProcessEventType ProcessEvent; // This is the void that let's us call process event.

public:
	EventsComponent();
	~EventsComponent();

public:
	static void ProcessEventDetour(class UObject* caller, class UFunction* function, void* params, void* result); // Process event gets detoured to this function, then we manually proxy it through to "ProcessEvent".
	void ToggleLogFunctions(const bool& bLog); // Toggle logging functions to the console.
	void FindHookedEvents(); // Automatically finds functions internal integer from their full name and map them out.
	void HookEventPre(const std::string& functionFullName, const PreEventType& eventType);
	void HookEventPost(const std::string& functionFullName, const PostEventType& eventType);
	void Initialize(); // Initializes binding events to voids.
};

extern class EventsComponent Events;