#include "Core.hpp"
#include "../Includes.hpp"

CoreComponent::CoreComponent() : Component("Core", "Initializes globals, components, and modules.")
{
	OnCreate();
}

CoreComponent::~CoreComponent()
{
	OnDestroy();
}

void CoreComponent::OnCreate()
{
	MainThread = nullptr;
}

void CoreComponent::OnDestroy() { }

void CoreComponent::InitializeThread()
{
	MainThread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(InitializeGlobals), nullptr, 0, nullptr);
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

void CoreComponent::InitializeGlobals(HMODULE hModule)
{
	// Disables the DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications.

	DisableThreadLibraryCalls(hModule);

	// Initialize the console file which also opens the standard out stream.

	Console.Initialize(std::filesystem::current_path(), "CodeRed.log");

	// Populate the GObject and GName addresses, remember to replace "PlaceholderGame" with your game.

	uintptr_t entryPoint = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
	GObjects = reinterpret_cast<TArray<UObject*>*>(Memory::FindPattern(GetModuleHandleW(NULL), GObjects_Pattern, GObjects_Mask));
	GNames = reinterpret_cast<TArray<FNameEntry*>*>(Memory::FindPattern(GetModuleHandleW(NULL), GNames_Pattern, GNames_Mask));

	// Verifies the global addresses are correct before continuing.

	if (AreGlobalsValid())
	{
		// You can use either a pattern for Process Event or its place in the VfTable index (not both).
		void** unrealVTable = reinterpret_cast<void**>(UObject::StaticClass()->VfTableObject.Dummy);
		//EventsComponent::AttachDetour(reinterpret_cast<ProcessEventType>(unrealVTable[0])); // Index method.
		//EventsComponent::AttachDetour(reinterpret_cast<ProcessEventType>(Memory::FindPattern(GetModuleHandleW(NULL), ProcessEvent_Pattern, ProcessEvent_Mask))); // Find pattern method.

		Console.Notify("[Core Module] Entry Point " + Format::Hex(entryPoint, sizeof(entryPoint)));
		Console.Notify("[Core Module] Global Objects: " + Format::Hex(reinterpret_cast<uintptr_t>(GObjects), sizeof(GObjects)));
		Console.Notify("[Core Module] Global Names: " + Format::Hex(reinterpret_cast<uintptr_t>(GNames), sizeof(GNames)));

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
	if (UObject::GObjObjects()->Num() > 0
		&& UObject::GObjObjects()->Max() > UObject::GObjObjects()->Num())
	{
		return true;
	}

	return false;
}

bool CoreComponent::AreGNamesValid()
{
	if (FName::Names()->Num() > 0
		&& FName::Names()->Max() > FName::Names()->Num())
	{
		return true;
	}

	return false;
}

class CoreComponent Core{};