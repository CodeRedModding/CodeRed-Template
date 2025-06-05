#pragma once
#include "../../pch.hpp"

namespace CodeRed::HookTypes
{
	// Parameters and types will be different depending on 64 bit or 32 bit games.
	typedef void(*ProcessEvent)(class UObject*, class UFunction*, void*, void*); // Calling object, function, structure pointer with parameters, unused result.
	typedef void(*ProcessInternal)(class UObject*, struct FFrame&, void*); // Calling object, frame, unused result.
	typedef void(*CallFunction)(class UObject*, struct FFrame&, void*, class UFunction*); // Calling object, frame, unused result, function.
}

namespace CodeRed
{
	class FunctionHook
	{
	private:
		uintptr_t* m_targetFunction; // Address of the actual function we want to detour.
		uintptr_t* m_hookedFunction; // The callback for the original function created AFTER hooking, to call from the override function.
		void* m_overrideFunction;

	public:
		FunctionHook();
		FunctionHook(const FunctionHook& functionHook);
		~FunctionHook();

	public:
		bool IsAttached() const;
		uintptr_t* GetTarget() const;
		uintptr_t* GetHooked() const;
		void* GetOverride() const;

	public:
		bool Attach(uintptr_t* targetFunction, void* overrideFunction);
		template <typename T> T Call()
		{
			return reinterpret_cast<T>(GetHooked());
		}
		void Detach();
		void Enable(); // Only works with MinHook, not Detours.
		void Disable(); // Only works with MinHook, not Detours.

	public:
		FunctionHook& operator=(const FunctionHook& functionHook);
	};
}