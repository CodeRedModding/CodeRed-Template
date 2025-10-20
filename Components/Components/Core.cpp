#include "Core.hpp"
#include "../Includes.hpp"

namespace CodeRed
{
	std::atomic<HANDLE> CoreComponent::m_mainThread = nullptr;

	CoreComponent::CoreComponent() : Component("Core", "Initializes globals, components, and modules.") { OnCreate(); }

	CoreComponent::~CoreComponent() { OnDestroy(); }

	void CoreComponent::OnCreate() {}

	void CoreComponent::OnDestroy()
	{
		if (m_mainThread)
		{
			WaitForSingleObject(m_mainThread, 5000); // Wait up to 5 seconds before giving up.
			CloseHandle(m_mainThread);
			m_mainThread = nullptr;
		}

#ifdef CR_MINHOOK
		MinHook::MH_Uninitialize();
#endif
	}

	bool CoreComponent::Initialize()
	{
		if (!IsInitialized())
		{
			if (Console.Initialize()) // Initialize the console file which also opens the standard out stream.
			{
				if (FindGlobals())
				{
					Console.Notify(GetNameFormatted() + "Entry Point " + Format::ToHex(reinterpret_cast<void*>(GetModuleHandleW(nullptr))));
					Console.Notify(GetNameFormatted() + "Global Objects: " + Format::ToHex(UObject::GObjObjects()));
					Console.Notify(GetNameFormatted() + "Global Names: " + Format::ToHex(FName::Names()));

#ifdef CR_MINHOOK
					MinHook::MH_STATUS minhookStatus = MinHook::MH_Initialize();

					if (minhookStatus != MinHook::MH_STATUS::MH_OK)
					{
						Console.Error(GetNameFormatted() + "Failed to initalize MinHook, cannot continue!");
						return false;
					}
#endif

					GameState.Initialize();			// Unimplemented.
					Instances.Initialize();			// Initialize class instances that aren't automatically set by function hooks.
					Variables.Initialize();			// Initialize any misc settings or commands here.
					Manager.Initialize();			// Initialize modules and their variables.
					Variables.SetupVariables();		// Load in any variables that have been previously saved.
					Manager.UpdateAllSettings();	// Update all loaded in variables for each module if any are found.
					Hooks.Initialize();				// Unimplemented.
					Events.Initialize();			// Initialize hooking functions from process event to your own.

					Console.Success(GetNameFormatted() + "Initialized!");
					SetInitialized(true);
				}
				else
				{
					Console.Error(GetNameFormatted() + "Failed to find globals!");
				}
			}
			else
			{
				MessageBoxA(NULL, "Failed to initialize console, cannot continue!", "CodeRed", (IDOK | MB_ICONHAND));
			}
		}

		return IsInitialized();
	}

	void CoreComponent::InitializeThread()
	{
		if (!m_mainThread)
		{
			m_mainThread = CreateThread(nullptr, 0, CoreComponent::OnThreadCreated, nullptr, 0, nullptr);
		}
	}

	DWORD WINAPI CoreComponent::OnThreadCreated(LPVOID lpParam)
	{
		Core.Initialize();
		return 0;
	}

	bool CoreComponent::FindGlobals()
	{
#ifdef WALKTHROUGH
		Console.Notify("\"Components\\Components\\Core.cpp -> CoreComponent::FindGlobals\". Looks like you forgot to check this file, this is important for initializing your sdk!");
		return false;
#endif

		if (!UObject::GObjObjects() && !FName::Names())
		{
			// Populate the GObject and GName addresses, remember to replace "PlaceholderSDK" with your own.
			GObjects = reinterpret_cast<TArray<UObject*>*>(Memory::FindPattern(GObjects_Pattern, GObjects_Mask));
			GNames = reinterpret_cast<TArray<FNameEntry*>*>(Memory::FindPattern(GNames_Pattern, GNames_Mask));
		}

		return AreGlobalsValid();
	}

	bool CoreComponent::AreGlobalsValid()
	{
		return (AreGObjectsValid() && AreGNamesValid());
	}

	bool CoreComponent::AreGObjectsValid()
	{
		if (UObject::GObjObjects()
			&& !UObject::GObjObjects()->empty()
			&& (UObject::GObjObjects()->capacity() > UObject::GObjObjects()->size()))
		{
			return true;
		}

		return false;
	}

	bool CoreComponent::AreGNamesValid()
	{
		if (FName::Names()
			&& !FName::Names()->empty()
			&& (FName::Names()->capacity() > FName::Names()->size()))
		{
			return true;
		}

		return false;
	}

	class CoreComponent Core;
}