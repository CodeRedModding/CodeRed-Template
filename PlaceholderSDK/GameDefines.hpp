#pragma once
#include <algorithm>
#include <locale>
#include <stdlib.h>
#include <xlocale>
#include <ctype.h>
#include <chrono>
#include <thread>
#include <vector>
#include <map>

// GObjects
#define GObjects_Pattern		(const uint8_t*)"\x00\x00\x00\x00\x00"
#define GObjects_Mask			(const char*)"xxxxx"
// GNames
#define GNames_Pattern			(const uint8_t*)"\x00\x00\x00\x00\x00"
#define GNames_Mask				(const char*)"xxxxx"
// Process Event
#define ProcessEvent_Pattern	(const uint8_t*)"\x00\x00\x00\x00\x00"
#define ProcessEvent_Mask		(const char*)"xxxxx"

/*
# =================================================================================================== #
# This just just a placeholder sdk so the project can actually compile! Replace this with your own!
# =================================================================================================== #
*/

template<typename TArray>
struct TIterator
{
public:
	using ElementType = typename TArray::ElementType;
	using ElementPointer = ElementType*;
	using ElementReference = ElementType&;
	using ElementConstReference = const ElementType&;

private:
	ElementPointer IteratorData;

public:
	TIterator(ElementPointer inElementPointer)
	{
		IteratorData = inElementPointer;
	}

public:
	TIterator& operator++()
	{
		IteratorData++;
		return *this;
	}

	TIterator operator++(int32_t)
	{
		TIterator iteratorCopy = *this;
		++(*this);
		return iteratorCopy;
	}

	TIterator& operator--()
	{
		IteratorData--;
		return *this;
	}

	TIterator operator--(int32_t)
	{
		TIterator iteratorCopy = *this;
		--(*this);
		return iteratorCopy;
	}

	ElementReference operator[](int32_t index)
	{
		return *(IteratorData[index]);
	}

	ElementPointer operator->()
	{
		return IteratorData;
	}

	ElementReference operator*()
	{
		return *IteratorData;
	}

	bool operator==(const TIterator& other) const
	{
		return (IteratorData == other.IteratorData);
	}

	bool operator!=(const TIterator& other) const
	{
		return !(*this == other);
	}
};

template<typename InElementType>
struct TArray
{
public:
	using ElementType = InElementType;
	using ElementPointer = ElementType*;
	using ElementReference = ElementType&;
	using ElementConstReference = const ElementType&;
	using Iterator = TIterator<TArray<ElementType>>;

private:
	ElementPointer ArrayData;
	int32_t ArrayCount;
	int32_t ArrayMax;

public:
	TArray()
	{
		ArrayData = nullptr;
		ArrayCount = 0;
		ArrayMax = 0;

		//ReAllocate(sizeof(ElementType));
	}

	~TArray()
	{
		//Clear();
		//::operator delete(ArrayData, ArrayMax * sizeof(ElementType));
	}

public:
	ElementConstReference operator[](int32_t index) const
	{
		if (index <= ArrayCount)
		{
			return ArrayData[index];
		}
	}

	ElementReference operator[](int32_t index)
	{
		if (index <= ArrayCount)
		{
			return ArrayData[index];
		} 
	}

	ElementConstReference At(const int32_t index) const
	{
		if (index <= ArrayCount)
		{
			return ArrayData[index];
		} 
	}

	ElementReference At(const int32_t index)
	{
		if (index <= ArrayCount)
		{
			return ArrayData[index];
		} 
	}

	void Add(ElementConstReference newElement)
	{
		if (ArrayCount >= ArrayMax)
		{
			ReAllocate(sizeof(ElementType) * (ArrayCount + 1));
		}

		new(&ArrayData[ArrayCount]) ElementType(newElement);
		ArrayCount++;
	}

	void Add(ElementReference& newElement)
	{
		if (ArrayCount >= ArrayMax)
		{
			ReAllocate(sizeof(ElementType) * (ArrayCount + 1));
		}

		new(&ArrayData[ArrayCount]) ElementType(newElement);
		ArrayCount++;
	}

	void PopBack()
	{
		if (ArrayCount > 0)
		{
			ArrayCount--;
			ArrayData[ArrayCount].~ElementType();
		}
	}

	void Clear()
	{
		for (int32_t i = 0; i < ArrayCount; i++)
		{
			ArrayData[i].~ElementType();
		}

		ArrayCount = 0;
	}

	int32_t Num() const
	{
		return ArrayCount;
	}

	int32_t Max() const
	{
		return ArrayMax;
	}

	Iterator begin()
	{
		return Iterator(ArrayData);
	}

	Iterator end()
	{
		return Iterator(ArrayData + ArrayCount);
	}

private:
	void ReAllocate(int32_t newArrayMax)
	{
		ElementPointer newArrayData = (ElementPointer)::operator new(newArrayMax * sizeof(ElementType));

		int32_t newNum = ArrayCount;

		if (newArrayMax < newNum)
		{
			newNum = newArrayMax;
		}

		for (int32_t i = 0; i < newNum; i++)
		{
			new(newArrayData + i) ElementType(std::move(ArrayData[i]));
		}

		for (int32_t i = 0; i < ArrayCount; i++)
		{
			ArrayData[i].~ElementType();
		}

		::operator delete(ArrayData, ArrayMax * sizeof(ElementType));
		ArrayData = newArrayData;
		ArrayMax = newArrayMax;
	}
};

/*
# =================================================================================================== #
# This just just a placeholder sdk so the project can actually compile! Replace this with your own!
# =================================================================================================== #
*/

extern TArray<class UObject*>* GObjects;
extern TArray<struct FNameEntry*>* GNames;

/*
# =================================================================================================== #
# This just just a placeholder sdk so the project can actually compile! Replace this with your own!
# =================================================================================================== #
*/

struct FNameEntry
{
public:
	int64_t			Flags;										// 0x0000 (0x08)
	int32_t			Index;										// 0x0008 (0x04)
	wchar_t			Name[0x400];								// 0x000C (0x00)

public:
	int32_t GetIndex() const
	{
		return Index;
	}

	const std::string GetName() const
	{
		std::wstring ws(Name);
		const std::string str(ws.begin(), ws.end());
		return str;
	}

	const wchar_t* GetWideName() const
	{
		return Name;
	}
};

struct FName
{
public:
	using ElementType = const wchar_t;
	using ElementPointer = ElementType*;

private:
	int32_t	FNameEntryId;
	int32_t	InstanceNumber;

public:
	FName()
	{
		FNameEntryId = 0;
		InstanceNumber = 0;
	}

	FName(int32_t i)
	{
		FNameEntryId = i;
		InstanceNumber = 0;
	}

	FName(ElementPointer nameToFind)
	{
		static std::vector<int32_t> nameCache{};

		FNameEntryId = 0;
		InstanceNumber = 0;

		for (int32_t name : nameCache)
		{
			if (Names()->At(name))
			{
				if (!wcscmp(Names()->At(nameCache[name])->Name, nameToFind))
				{
					FNameEntryId = nameCache[name];
					return;
				}
			}
		}

		for (int i = 0; i < Names()->Num(); i++)
		{
			if (Names()->At(i))
			{
				if (!wcscmp(Names()->At(i)->Name, nameToFind))
				{
					nameCache.push_back(i);
					FNameEntryId = i;
				}
			}
		}
	}

	~FName() { }

public:
	static TArray<FNameEntry*>* Names()
	{
		TArray<struct FNameEntry*>* GNamesArray = reinterpret_cast<TArray<struct FNameEntry*>*>(GNames);
		return GNamesArray;
	}

	int32_t GetDisplayIndex() const
	{
		return FNameEntryId;
	}

	const struct FNameEntry GetDisplayNameEntry() const
	{
		if (IsValid())
		{
			return *Names()->At(FNameEntryId);
		}
	}

	struct FNameEntry* GetEntry()
	{
		if (IsValid())
		{
			return Names()->At(FNameEntryId);
		}
	}

	int32_t GetNumber() const
	{
		return InstanceNumber;
	}

	void SetNumber(int32_t newNumber)
	{
		InstanceNumber = newNumber;
	}

	std::string ToString()
	{
		if (IsValid())
		{
			return GetEntry()->GetName();
		}

		return std::string("UnknownName");
	}

	bool IsValid() const
	{
		if (FNameEntryId < 0 || FNameEntryId > Names()->Num())
		{
			return false;
		}

		return true;
	}

	FName operator=(const FName& other)
	{
		FNameEntryId = other.FNameEntryId;
		InstanceNumber = other.InstanceNumber;
		return *this;
	}

	bool operator==(const FName& other) const
	{
		return (FNameEntryId == other.FNameEntryId);
	}

	bool operator!=(const FName& other) const
	{
		return (FNameEntryId != other.FNameEntryId);
	}
};

struct FString
{
public:
	using ElementType = const wchar_t;
	using ElementPointer = ElementType*;

private:
	ElementPointer ArrayData;
	int32_t ArrayCount;
	int32_t ArrayMax;

public:
	FString()
	{
		ArrayData = nullptr;
		ArrayCount = 0;
		ArrayMax = 0;
	}

	FString(ElementPointer other)
	{
		ArrayData = nullptr;
		ArrayCount = 0;
		ArrayMax = 0;

		ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

		if (ArrayCount > 0)
		{
			ArrayData = other;
		}
	}

	~FString() { }

public:
	const std::string ToString() const
	{
		if (IsValid())
		{
			std::wstring wideStr(ArrayData);
			std::string str(wideStr.begin(), wideStr.end());
			return str;
		}

		return std::string("null");
	}

	bool IsValid() const
	{
		return !!ArrayData;
	}

	FString operator=(ElementPointer other)
	{
		if (ArrayData != other)
		{
			ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

			if (ArrayCount > 0)
			{
				ArrayData = other;
			}
		}

		return *this;
	}

	bool operator==(const FString& other)
	{
		return (!wcscmp(ArrayData, other.ArrayData));
	}

	bool operator!=(const FString& other)
	{
		return (wcscmp(ArrayData, other.ArrayData));
	}
};

struct FPointer
{
	uintptr_t Dummy;
};

struct FScriptDelegate
{
	uint8_t UnknownData00[0x18];
};

struct FVector
{
	float X;
	float Y;
	float Z;
};

struct FVector2D
{
	float X;
	float Y;
};

struct FRotator
{
	int32_t Pitch;
	int32_t Yaw;
	int32_t Roll;
};

/*
# =================================================================================================== #
# This just just a placeholder sdk so the project can actually compile! Replace this with your own!
# =================================================================================================== #
*/