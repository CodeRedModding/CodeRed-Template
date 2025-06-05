#pragma once
#include "../../pch.hpp"

namespace CodeRed
{
	// Used exclusively by the "EventGuard" class as a key for the "m_refStore" map.
	class EventPair
	{
	public:
		class UObject* Caller;
		class UFunction* Function;

	public:
		EventPair();
		EventPair(class UObject* caller, class UFunction* function);
		EventPair(const EventPair& eventPair);
		~EventPair();

	public:
		size_t Hash() const;

	public:
		bool operator==(const EventPair& eventPair) const;
		bool operator!=(const EventPair& eventPair) const;
		bool operator<(const EventPair& eventPair) const;
		bool operator>(const EventPair& eventPair) const;
		EventPair& operator=(const EventPair& eventPair);
	};
}

namespace std
{
	template<>
	struct hash<CodeRed::EventPair>
	{
		size_t operator()(const CodeRed::EventPair& eventPair) const
		{
			return eventPair.Hash();
		}
	};
}

namespace CodeRed
{
	// Used to prevent duplicate calls to different function hooks at the same time, similar logic to a scoped lock.
	class EventGuard
	{
	private:
		static inline std::unordered_map<EventPair, uint8_t> m_refStore;
		EventPair m_event;
		bool m_isFirst;

	public:
		EventGuard();
		EventGuard(class UObject* caller, class UStruct* function);
		EventGuard(class UObject* caller, class UFunction* function);
		EventGuard(const EventGuard& eventGuard);
		~EventGuard();

	public:
		class UObject* GetCaller() const;
		class UFunction* GetFunction() const;
		bool IsFirst() const;
		uint8_t& GetRefs() const;

	public:
		void Store(class UObject* caller, class UStruct* function);
		void Store(class UObject* caller, class UFunction* function);
		static void ClearRefStore();

	private:
		void Clear(); // This needs to be called in the deconstructor and not manually for RAII, ref counts will be properly removed even if you return early somewhere or the function ends.
		void AddRef();
		void RemoveRef();

	public:
		EventGuard& operator=(const EventGuard& eventGuard);
	};
}