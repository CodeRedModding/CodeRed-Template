#include "EventGuard.hpp"

namespace CodeRed
{
	EventPair::EventPair() : Caller(nullptr), Function(nullptr) {}

	EventPair::EventPair(class UObject* caller, class UFunction* function) : Caller(caller), Function(function) {}

	EventPair::EventPair(const EventPair& eventPair) : Caller(eventPair.Caller), Function(eventPair.Function) {}

	EventPair::~EventPair() {}

	size_t EventPair::Hash() const
	{
		size_t callerHash = std::hash<void*>()(Caller);
		size_t functionHash = std::hash<void*>()(Function);
		return (callerHash ^ (functionHash + GOLDEN_RATIO_MAGIC + (callerHash << 6) + (callerHash >> 2)));
	}

	bool EventPair::operator==(const EventPair& eventPair) const
	{
		return ((Caller == eventPair.Caller) && (Function == eventPair.Function));
	}

	bool EventPair::operator!=(const EventPair& eventPair) const
	{
		return !(*this == eventPair);
	}

	bool EventPair::operator<(const EventPair& eventPair) const
	{
		return (Function < eventPair.Function);
	}

	bool EventPair::operator>(const EventPair& eventPair) const
	{
		return (Function > eventPair.Function);
	}

	EventPair& EventPair::operator=(const EventPair& eventPair)
	{
		Caller = eventPair.Caller;
		Function = eventPair.Function;
		return *this;
	}

	EventGuard::EventGuard() : m_isFirst(false) {}

	EventGuard::EventGuard(class UObject* uObject, class UStruct* function) : m_isFirst(false)
	{
		Store(uObject, function);
	}

	EventGuard::EventGuard(class UObject* uObject, class UFunction* function) : m_isFirst(false)
	{
		Store(uObject, function);
	}

	EventGuard::EventGuard(const EventGuard& eventGuard) : m_event(eventGuard.m_event), m_isFirst(eventGuard.m_isFirst) {}

	EventGuard::~EventGuard() { Clear(); }

	class UObject* EventGuard::GetCaller() const
	{
		return m_event.Caller;
	}

	class UFunction* EventGuard::GetFunction() const
	{
		return m_event.Function;
	}

	bool EventGuard::IsFirst() const
	{
		return m_isFirst;
	}

	uint8_t& EventGuard::GetRefs() const
	{
		return m_refStore[EventPair(GetCaller(), GetFunction())];
	}

	void EventGuard::Store(class UObject* caller, class UStruct* function)
	{
		Store(caller, static_cast<UFunction*>(function));
	}

	void EventGuard::Store(class UObject* caller, class UFunction* function)
	{
		if (caller || function)
		{
			m_event.Caller = caller;
			m_event.Function = function;
			AddRef();
		}
	}

	void EventGuard::ClearRefStore()
	{
		m_refStore.clear();
	}

	void EventGuard::Clear()
	{
		RemoveRef();
		m_event = EventPair();
	}

	void EventGuard::AddRef()
	{
		if (GetCaller() && GetFunction())
		{
			uint8_t& refs = GetRefs();
			m_isFirst = !refs;
			refs++;
		}
	}

	void EventGuard::RemoveRef()
	{
		if (GetCaller() && GetFunction())
		{
			uint8_t& refs = GetRefs();

			if (refs)
			{
				refs--;
			}

			if (!refs)
			{
				m_refStore.erase(EventPair(GetCaller(), GetFunction()));
			}
		}
	}

	EventGuard& EventGuard::operator=(const EventGuard& eventGuard)
	{
		m_event = eventGuard.m_event;
		m_isFirst = eventGuard.m_isFirst;
		return *this;
	}
}