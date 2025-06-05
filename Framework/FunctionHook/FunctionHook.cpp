#include "FunctionHook.hpp"

namespace CodeRed
{
	FunctionHook::FunctionHook() : m_targetFunction(nullptr), m_hookedFunction(nullptr), m_overrideFunction(nullptr) {}

	FunctionHook::FunctionHook(const FunctionHook& functionHook) : m_targetFunction(functionHook.m_targetFunction), m_hookedFunction(functionHook.m_hookedFunction), m_overrideFunction(functionHook.m_overrideFunction) {}

	FunctionHook::~FunctionHook()
	{
		Detach();
	}

	bool FunctionHook::IsAttached() const
	{
		return (GetTarget() && GetHooked());
	}

	uintptr_t* FunctionHook::GetTarget() const
	{
		return m_targetFunction;
	}

	uintptr_t* FunctionHook::GetHooked() const
	{
		return m_hookedFunction;
	}

	void* FunctionHook::GetOverride() const
	{
		return m_overrideFunction;
	}

	bool FunctionHook::Attach(uintptr_t* targetFunction, void* overrideFunction)
	{
		if (!IsAttached() && targetFunction && overrideFunction)
		{
			m_targetFunction = targetFunction;
			m_overrideFunction = overrideFunction;
			m_hookedFunction = nullptr;

#ifdef CR_MINHOOK
			MinHook::MH_STATUS detourResult = MinHook::MH_CreateHook(GetTarget(), GetOverride(), reinterpret_cast<LPVOID*>(&m_hookedFunction));
			return (detourResult == MinHook::MH_STATUS::MH_OK);
#elif defined(CR_DETOURS)
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			int32_t detourResult = DetourAttach(&reinterpret_cast<PVOID&>(m_targetFunction), reinterpret_cast<PVOID>(GetOverride()));
			DetourTransactionCommit();
			return (detourResult == NO_ERROR);
#endif
		}

		return false;
	}

	void FunctionHook::Detach()
	{
		if (IsAttached() && GetOverride())
		{
#ifdef CR_MINHOOK
			Disable();
			MinHook::MH_RemoveHook(GetTarget());
#elif defined(CR_DETOURS)
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourDetach(&reinterpret_cast<PVOID&>(m_targetFunction), reinterpret_cast<PVOID>(GetOverride()));
			DetourTransactionCommit();
#endif
			m_hookedFunction = nullptr;
		}
	}

	void FunctionHook::Enable()
	{
#ifdef CR_MINHOOK
		if (IsAttached())
		{
			MinHook::MH_EnableHook(GetTarget());
		}
#endif
	}

	void FunctionHook::Disable()
	{
#ifdef CR_MINHOOK
		if (IsAttached())
		{
			MinHook::MH_DisableHook(GetTarget());
		}
#endif
	}

	FunctionHook& FunctionHook::operator=(const FunctionHook& functionHook)
	{
		m_targetFunction = functionHook.m_targetFunction;
		m_hookedFunction = functionHook.m_hookedFunction;
		m_overrideFunction = functionHook.m_overrideFunction;
		return *this;
	}
}