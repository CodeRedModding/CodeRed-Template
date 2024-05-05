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

// Object Flags
enum EObjectFlags : uint64_t
{
	RF_NoFlags = 0x000000000000000,	// Object has no flags.
	RF_InSingularFunc = 0x0000000000000002,	// In a singular function.
	RF_StateChanged = 0x0000000000000004, // Object did a state change.
	RF_DebugPostLoad = 0x0000000000000008, // For debugging PostLoad calls.
	RF_DebugSerialize = 0x0000000000000010, // For debugging Serialize calls.
	RF_DebugFinishDestroyed = 0x0000000000000020, // For debugging FinishDestroy calls.
	RF_EdSelected = 0x0000000000000040,	// Object is selected in one of the editors browser windows.
	RF_ZombieComponent = 0x0000000000000080, // This component's template was deleted, so should not be used.
	RF_Protected = 0x0000000000000100, // Property is protected (may only be accessed from its owner class or subclasses).
	RF_ClassDefaultObject = 0x0000000000000200,	// this object is its class's default object.
	RF_ArchetypeObject = 0x0000000000000400, // this object is a template for another object (treat like a class default object).
	RF_ForceTagExp = 0x0000000000000800, // Forces this object to be put into the export table when saving a package regardless of outer.
	RF_TokenStreamAssembled = 0x0000000000001000, // Set if reference token stream has already been assembled.
	RF_MisalignedObject = 0x0000000000002000, // Object's size no longer matches the size of its C++ class (only used during make, for native classes whose properties have changed).
	RF_RootSet = 0x0000000000004000, // Object will not be garbage collected, even if unreferenced.
	RF_BeginDestroyed = 0x0000000000008000,	// BeginDestroy has been called on the object.
	RF_FinishDestroyed = 0x0000000000010000, // FinishDestroy has been called on the object.
	RF_DebugBeginDestroyed = 0x0000000000020000, // Whether object is rooted as being part of the root set (garbage collection).
	RF_MarkedByCooker = 0x0000000000040000,	// Marked by content cooker.
	RF_LocalizedResource = 0x0000000000080000, // Whether resource object is localized.
	RF_InitializedProps = 0x0000000000100000, // whether InitProperties has been called on this object
	RF_PendingFieldPatches = 0x0000000000200000, // @script patcher: indicates that this struct will receive additional member properties from the script patcher.
	RF_IsCrossLevelReferenced = 0x0000000000400000,	// This object has been pointed to by a cross-level reference, and therefore requires additional cleanup upon deletion.
	RF_Saved = 0x0000000080000000, // Object has been saved via SavePackage (temporary).
	RF_Transactional = 0x0000000100000000, // Object is transactional.
	RF_Unreachable = 0x0000000200000000, // Object is not reachable on the object graph.
	RF_Public = 0x0000000400000000, // Object is visible outside its package.
	RF_TagImp = 0x0000000800000000,	// Temporary import tag in load/save.
	RF_TagExp = 0x0000001000000000,	// Temporary export tag in load/save.
	RF_Obsolete = 0x0000002000000000, // Object marked as obsolete and should be replaced.
	RF_TagGarbage = 0x0000004000000000,	// Check during garbage collection.
	RF_DisregardForGC = 0x0000008000000000,	// Object is being disregard for GC as its static and itself and all references are always loaded.
	RF_PerObjectLocalized = 0x0000010000000000,	// Object is localized by instance name, not by class.
	RF_NeedLoad = 0x0000020000000000, // During load, indicates object needs loading.
	RF_AsyncLoading = 0x0000040000000000, // Object is being asynchronously loaded.
	RF_NeedPostLoadSubobjects = 0x0000080000000000, // During load, indicates that the object still needs to instance subobjects and fixup serialized component references.
	RF_Suppress = 0x0000100000000000, // @warning: Mirrored in UnName.h. Suppressed log name.
	RF_InEndState = 0x0000200000000000, // Within an EndState call.
	RF_Transient = 0x0000400000000000, // Don't save object.
	RF_Cooked = 0x0000800000000000, // Whether the object has already been cooked
	RF_LoadForClient = 0x0001000000000000, // In-file load for client.
	RF_LoadForServer = 0x0002000000000000, // In-file load for client.
	RF_LoadForEdit = 0x0004000000000000, // In-file load for client.
	RF_Standalone = 0x0008000000000000,	// Keep object around for editing even if unreferenced.
	RF_NotForClient = 0x0010000000000000, // Don't load this object for the game client.
	RF_NotForServer = 0x0020000000000000, // Don't load this object for the game server.
	RF_NotForEdit = 0x0040000000000000,	// Don't load this object for the editor.
	RF_NeedPostLoad = 0x0100000000000000, // Object needs to be postloaded.
	RF_HasStack = 0x0200000000000000, // Has execution stack.
	RF_Native = 0x0400000000000000, // Native (UClass only)
	RF_Marked = 0x0800000000000000,	// Marked (for debugging).
	RF_ErrorShutdown = 0x1000000000000000, // ShutdownAfterError called.
	RF_PendingKill = 0x2000000000000000, // Objects that are pending destruction (invalid for gameplay but valid objects).
	RF_MarkedByCookerTemp = 0x4000000000000000,	// Temporarily marked by content cooker (should be cleared).
	RF_CookedStartupObject = 0x8000000000000000, // This object was cooked into a startup package.

	RF_ContextFlags = (RF_NotForClient | RF_NotForServer | RF_NotForEdit), // All context flags.
	RF_LoadContextFlags = (RF_LoadForClient | RF_LoadForServer | RF_LoadForEdit), // Flags affecting loading.
	RF_Load = (RF_ContextFlags | RF_LoadContextFlags | RF_Public | RF_Standalone | RF_Native | RF_Obsolete | RF_Protected | RF_Transactional | RF_HasStack | RF_PerObjectLocalized | RF_ClassDefaultObject | RF_ArchetypeObject | RF_LocalizedResource), // Flags to load from Unrealfiles.
	RF_Keep = (RF_Native | RF_Marked | RF_PerObjectLocalized | RF_MisalignedObject | RF_DisregardForGC | RF_RootSet | RF_LocalizedResource), // Flags to persist across loads.
	RF_ScriptMask = (RF_Transactional | RF_Public | RF_Transient | RF_NotForClient | RF_NotForServer | RF_NotForEdit | RF_Standalone), // Script-accessible flags.
	RF_UndoRedoMask = (RF_PendingKill), // Undo/ redo will store/ restore these
	RF_PropagateToSubObjects = (RF_Public | RF_ArchetypeObject | RF_Transactional), // Sub-objects will inherit these flags from their SuperObject.

	RF_AllFlags = 0xFFFFFFFFFFFFFFFF,
};

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
class TArray
{
public:
	using ElementType = InElementType;
	using ElementPointer = ElementType*;
	using ElementReference = ElementType&;
	using ElementConstPointer = const ElementType*;
	using ElementConstReference = const ElementType&;
	using Iterator = TIterator<TArray<ElementType>>;

private:
	ElementPointer ArrayData;
	int32_t ArrayCount;
	int32_t ArrayMax;

public:
	TArray() : ArrayData(nullptr), ArrayCount(0), ArrayMax(0)
	{
		//ReAllocate(sizeof(ElementType));
	}

	~TArray()
	{
		//clear();
		//::operator delete(ArrayData, ArrayMax * sizeof(ElementType));
	}

public:
	ElementConstReference operator[](int32_t index) const
	{
		return ArrayData[index];
	}

	ElementReference operator[](int32_t index)
	{
		return ArrayData[index];
	}

	ElementConstReference at(int32_t index) const
	{
		return ArrayData[index];
	}

	ElementReference at(int32_t index)
	{
		return ArrayData[index];
	}

	ElementConstPointer data() const
	{
		return ArrayData;
	}

	void push_back(ElementConstReference newElement)
	{
		if (ArrayCount >= ArrayMax)
		{
			ReAllocate(sizeof(ElementType) * (ArrayCount + 1));
		}

		new(&ArrayData[ArrayCount]) ElementType(newElement);
		ArrayCount++;
	}

	void push_back(ElementReference& newElement)
	{
		if (ArrayCount >= ArrayMax)
		{
			ReAllocate(sizeof(ElementType) * (ArrayCount + 1));
		}

		new(&ArrayData[ArrayCount]) ElementType(newElement);
		ArrayCount++;
	}

	void pop_back()
	{
		if (ArrayCount > 0)
		{
			ArrayCount--;
			ArrayData[ArrayCount].~ElementType();
		}
	}

	void clear()
	{
		for (int32_t i = 0; i < ArrayCount; i++)
		{
			ArrayData[i].~ElementType();
		}

		ArrayCount = 0;
	}

	int32_t size() const
	{
		return ArrayCount;
	}

	int32_t capacity() const
	{
		return ArrayMax;
	}

	bool empty() const
	{
		if (ArrayData)
		{
			return (size() == 0);
		}

		return true;
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
	uint64_t Flags;													// 0x0000 (0x0008)
	int32_t Index;													// 0x0008 (0x0004)
	wchar_t Name[0x400];											// 0x000C (0x0000)

public:
	FNameEntry() : Flags(0), Index(-1) {}
	~FNameEntry() {}

public:
	uint64_t GetFlags() const
	{
		return Flags;
	}

	int32_t GetIndex() const
	{
		return Index;
	}

	const wchar_t* GetWideName() const
	{
		return Name;
	}

	std::wstring ToWideString() const
	{
		const wchar_t* wideName = GetWideName();

		if (wideName)
		{
			return std::wstring(wideName);
		}

		return L"";
	}

	std::string ToString() const
	{
		std::wstring wstr = ToWideString();
		return std::string(wstr.begin(), wstr.end());
	}
};

struct FName
{
public:
	using ElementType = const wchar_t;
	using ElementPointer = ElementType*;

private:
	int32_t			FNameEntryId;									// 0x0000 (0x04)
	int32_t			InstanceNumber;									// 0x0004 (0x04)

public:
	FName() : FNameEntryId(-1), InstanceNumber(0) {}

	FName(int32_t id) : FNameEntryId(id), InstanceNumber(0) {}

	FName(const ElementPointer nameToFind) : FNameEntryId(-1), InstanceNumber(0)
	{
		static std::vector<int32_t> foundNames{};

		for (int32_t entryId : foundNames)
		{
			if (Names()->at(entryId))
			{
				if (!wcscmp(Names()->at(entryId)->Name, nameToFind))
				{
					FNameEntryId = entryId;
					return;
				}
			}
		}

		for (int32_t i = 0; i < Names()->size(); i++)
		{
			if (Names()->at(i))
			{
				if (!wcscmp(Names()->at(i)->Name, nameToFind))
				{
					foundNames.push_back(i);
					FNameEntryId = i;
					return;
				}
			}
		}
	}

	FName(const FName& name) : FNameEntryId(name.FNameEntryId), InstanceNumber(name.InstanceNumber) {}

	~FName() {}

public:
	static TArray<FNameEntry*>* Names()
	{
		TArray<FNameEntry*>* recastedArray = reinterpret_cast<TArray<FNameEntry*>*>(GNames);
		return recastedArray;
	}

	int32_t GetDisplayIndex() const
	{
		return FNameEntryId;
	}

	const FNameEntry GetDisplayNameEntry() const
	{
		if (IsValid())
		{
			return *Names()->at(FNameEntryId);
		}

		return FNameEntry();
	}

	FNameEntry* GetEntry()
	{
		if (IsValid())
		{
			return Names()->at(FNameEntryId);
		}

		return nullptr;
	}

	int32_t GetInstance() const
	{
		return InstanceNumber;
	}

	void SetInstance(int32_t newNumber)
	{
		InstanceNumber = newNumber;
	}

	std::string ToString() const
	{
		if (IsValid())
		{
			return GetDisplayNameEntry().ToString();
		}

		return "UnknownName";
	}

	bool IsValid() const
	{
		if ((FNameEntryId < 0 || FNameEntryId > Names()->size()))
		{
			return false;
		}

		return true;
	}

public:
	FName& operator=(const FName& other)
	{
		FNameEntryId = other.FNameEntryId;
		InstanceNumber = other.InstanceNumber;
		return *this;
	}

	bool operator==(const FName& other) const
	{
		return ((FNameEntryId == other.FNameEntryId) && (InstanceNumber == other.InstanceNumber));
	}

	bool operator!=(const FName& other) const
	{
		return !(*this == other);
	}
};

class FString
{
public:
	using ElementType = const wchar_t;
	using ElementPointer = ElementType*;

private:
	ElementPointer	ArrayData;										// 0x0000 (0x08)
	int32_t			ArrayCount;										// 0x0008 (0x04)
	int32_t			ArrayMax;										// 0x000C (0x04)

public:
	FString() : ArrayData(nullptr), ArrayCount(0), ArrayMax(0) {}

	FString(ElementPointer other) : ArrayData(nullptr), ArrayCount(0), ArrayMax(0) { assign(other); }

	~FString() {}

public:
	FString& assign(ElementPointer other)
	{
		ArrayCount = (other ? (wcslen(other) + 1) : 0);
		ArrayMax = ArrayCount;
		ArrayData = (ArrayCount > 0 ? other : nullptr);
		return *this;
	}

	std::wstring ToWideString() const
	{
		if (!empty())
		{
			return std::wstring(c_str());
		}

		return L"";
	}

	std::string ToString() const
	{
		if (!empty())
		{
			std::wstring wstr = ToWideString();
			return std::string(wstr.begin(), wstr.end());
		}

		return "";
	}

	ElementPointer c_str() const
	{
		return ArrayData;
	}

	bool empty() const
	{
		if (ArrayData)
		{
			return (ArrayCount == 0);
		}

		return true;
	}

	int32_t length() const
	{
		return ArrayCount;
	}

	int32_t size() const
	{
		return ArrayMax;
	}

public:
	FString& operator=(ElementPointer other)
	{
		return assign(other);
	}

	FString& operator=(const FString& other)
	{
		return assign(other.c_str());
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
	uint8_t UnknownData00[0xC];
};

struct FVector
{
	float X;
	float Y;
	float Z;
};

struct FPlane : FVector
{
	float W;  
};

struct FVector2D
{
	float X;
	float Y;
};

struct FQuat
{
	float X;
	float Y;
	float Z;
	float W;
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