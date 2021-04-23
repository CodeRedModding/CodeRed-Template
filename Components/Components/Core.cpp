#include "Core.hpp"
#include "../Includes.hpp"
#include "../Extensions/Includes.hpp"
#include <detours.h>

CoreComponent::CoreComponent() : Component("Core", "Initializes globals, components, and modules.")
{
	MainThread = nullptr;
}

// Necessary for if your DLL gets intentionally (or unintentionally) unloaded before your game exits.
CoreComponent::~CoreComponent()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&reinterpret_cast<PVOID&>(Events.ProcessEvent), reinterpret_cast<PVOID>(Events.ProcessEventDetour));
	DetourTransactionCommit();
}

void CoreComponent::InitializeThread()
{
	MainThread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(InitializeGlobals), nullptr, 0, nullptr);
}

void CoreComponent::InitializeGlobals(HMODULE hModule)
{
	// Disables the DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications.

	DisableThreadLibraryCalls(hModule);

	// Initialize the console file which also opens the standard out stream.

	Console.Initialize(Filesystem::GetCurrentPath(), "CodeRed.log");

	// Populate the GObject and GName addresses, remember to replace "PlaceholderGame" with your game.

	uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA("PlaceholderGame.exe"));
	GObjects = reinterpret_cast<TArray<UObject*>*>(Memory::FindPattern(GetModuleHandleA("PlaceholderGame.exe"), GObjects_Pattern, GObjects_Mask));
	GNames = reinterpret_cast<TArray<FNameEntry*>*>(Memory::FindPattern(GetModuleHandleA("PlaceholderGame.exe"), GNames_Pattern, GNames_Mask));

	// Verifies the global addresses are correct before continuing.

	if (AreGlobalsValid())
	{
		// You can use either a pattern for Process Event or it's place in the VfTable index.

		Events.ProcessEvent = reinterpret_cast<ProcessEventType>(Memory::FindPattern(GetModuleHandleA("PlaceholderGame.exe"), ProcessEvent_Pattern, ProcessEvent_Mask));
		//Events.ProcessEvent = reinterpret_cast<ProcessEventType>(reinterpret_cast<void**>(UObject::StaticClass()->VfTableObject.Dummy)[0]); // Zero being the index.

		// Redirects the process event virtual function to our own void, for us to manually process later to the typedef.

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&reinterpret_cast<PVOID&>(Events.ProcessEvent), reinterpret_cast<PVOID>(Events.ProcessEventDetour));
		DetourTransactionCommit();

		char buffer[512] = { NULL };

		sprintf_s(buffer, "[Core Module] Entry Point: 0x%" PRIxPTR, baseAddress);
		Console.Notify(buffer);

		sprintf_s(buffer, "[Core Module] Global Objects: 0x%" PRIxPTR, GObjects);
		Console.Notify(buffer);

		sprintf_s(buffer, "[Core Module] Global Names: 0x%" PRIxPTR, GNames);
		Console.Notify(buffer);

		Console.Write("[Core Module] Initialized!");

		Instances.Initialize(); // Initialize class instances that aren't automatically set by function hooks.
		Manager.Initialize(); // Initialize settings, commands, and mods.
		Events.Initialize(); // Initialize hooking function events to voids.
	}
	else
	{
		Console.Error("[Core Module] GObject and GNames are not valid, wrong address detected!");
	}
}

bool CoreComponent::AreGObjectsValid()
{
	if (UObject::GObjObjects()->Num() > 0 && UObject::GObjObjects()->Max() > UObject::GObjObjects()->Num())
	{
		if (UObject::GObjObjects()->At(0)->GetFullName() == "Class Core.Config_ORS")
		{
			return true;
		}
	}

	return false;
}

bool CoreComponent::AreGNamesValid()
{
	if (FName::Names()->Num() > 0 && FName::Names()->Max() > FName::Names()->Num())
	{
		if (FName(0).ToString() == "None")
		{
			return true;
		}
	}

	return false;
}

bool CoreComponent::AreGlobalsValid()
{
	static bool alreadyChecked = false;
	static bool globalsValid = false;

	if (!alreadyChecked)
	{
		if (AreGObjectsValid() && AreGNamesValid())
		{
			globalsValid = true;
		}
		else
		{
			globalsValid = false;
		}

		alreadyChecked = true;
	}

	return globalsValid;
}

class CoreComponent Core;