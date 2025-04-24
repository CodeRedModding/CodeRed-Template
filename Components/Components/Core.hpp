#pragma once
#include "../Component.hpp"

namespace CodeRed
{
	// Manages global objects and names, along with initializing a new thread for the mod to run on.
	class CoreComponent : public Component
	{
	private:
		HANDLE m_mainThread;

	public:
		CoreComponent();
		~CoreComponent() override;

	public:
		void OnCreate() override;
		void OnDestroy() override;

	public:
		void InitializeThread();
		static void InitializeGlobals(HMODULE hModule);

	private:
		static bool AreGlobalsValid();
		static bool AreGObjectsValid();
		static bool AreGNamesValid();
	};

	extern class CoreComponent Core;
}