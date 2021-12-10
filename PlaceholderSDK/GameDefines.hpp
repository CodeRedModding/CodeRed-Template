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

// Proprerty Flags
// https://docs.unrealengine.com/en-US/API/Runtime/CoreUObject/UObject/EPropertyFlags/index.html (The ones in this link are UE4 specific, so I had to modify accordingly here.)
enum EPropertyFlags
{
	CPF_Edit =								0x0000000000000001,	// Property is user-settable in the editor.
	CPF_Const =								0x0000000000000002,	// Actor's property always matches class's default actor property.
	CPF_Input =							    0x0000000000000004,	// Variable is writable by the input system.
	CPF_ExportObject =						0x0000000000000008,	// Object can be exported with actor.
	CPF_OptionalParm =						0x0000000000000010,	// Optional parameter (if CPF_Param is set).
	CPF_Net =								0x0000000000000020,	// Property is relevant to network replication.
	CPF_EditConstArray =					0x0000000000000040,	// Prevent adding/removing of items from dynamic a array in the editor.
	CPF_Parm =								0x0000000000000080,	// Function/When call parameter.
	CPF_OutParm =							0x0000000000000100,	// Value is copied out after function call.
	CPF_SkipParm =							0x0000000000000200,	// Property is a short-circuitable evaluation function parm.
	CPF_ReturnParm =						0x0000000000000400,	// Return value.
	CPF_CoerceParm =						0x0000000000000800,	// Coerce args into this function parameter.
	CPF_Native =							0x0000000000001000,	// Property is native: C++ code is responsible for serializing it.
	CPF_Transient =							0x0000000000002000,	// Property is transient: shouldn't be saved, zero-filled at load time.
	CPF_Config =							0x0000000000004000,	// Property should be loaded/saved as permanent profile.
	CPF_Localized =							0x0000000000008000,	// Property should be loaded as localizable text.
	CPF_Travel =							0x0000000000010000,	// Property travels across levels/servers.
	CPF_EditConst =							0x0000000000020000,	// Property is uneditable in the editor.
	CPF_GlobalConfig =						0x0000000000040000,	// Load config from base class, not subclass.
	CPF_Component =							0x0000000000080000,	// Property containts component references.
	CPF_NeedCtorLink =						0x0000000000400000,	// Fields need construction/destruction.
	CPF_NoExport =							0x0000000000800000,	// Property should not be exported to the native class header file.
	CPF_NoClear =							0x0000000002000000,	// Hide clear (and browse) button.
	CPF_EditInline =						0x0000000004000000,	// Edit this object reference inline.
	CPF_EdFindable =						0x0000000008000000,	// References are set by clicking on actors in the editor viewports.
	CPF_EditInlineUse =						0x0000000010000000,	// EditInline with Use button.
	CPF_Deprecated =						0x0000000020000000,	// Property is deprecated.  Read it from an archive, but don't save it.
	CPF_EditInlineNotify =					0x0000000040000000,	// EditInline, notify outer object on editor change.
	CPF_RepNotify =							0x0000000100000000,	// Notify actors when a property is replicated
	CPF_Interp =							0x0000000200000000,	// interpolatable property for use with matinee
	CPF_NonTransactional =					0x0000000400000000,	// Property isn't transacted
	CPF_EditorOnly =						0x0000000800000000,	// Property should only be loaded in the editor.
	CPF_NoDestructor =						0x0000001000000000,	// No destructor.
	CPF_AutoWeak =							0x0000004000000000,	// CPF_ = 0x0000002000000000, ///<.
	CPF_ContainsInstancedReference =        0x0000008000000000,	// Property contains component refuerences.
	CPF_AssetRegistrySearchable =           0x0000010000000000,	// Asset instances will add properties with this flag to the asset registry automatically
	CPF_SimpleDisplay =						0x0000020000000000,	// The property is visible by default in the editor details view.
	CPF_AdvancedDisplay =					0x0000040000000000,	// The property is advanced and not visible by default in the editor details view.
	CPF_Protected =							0x0000080000000000,	// Property is protected from the perspective of scrip
	CPF_BlueprintCallable =					0x0000100000000000,	// MC Delegates only. Property should be exposed for calling in blueprint code.
	CPF_BlueprintAuthorityOnly =			0x0000200000000000,	// MC Delegates only. This delegate accepts (only in blueprint) only events with BlueprintAuthorityOnly.
	CPF_TextExportTransient =				0x0000400000000000,	// Property shouldn't be exported to text format (e.g. copy/paste)
	CPF_NonPIEDuplicateTransient =			0x0000800000000000,	// Property should only be copied in PIE.
	CPF_ExposeOnSpawn =						0x0001000000000000,	// Property is exposed on spawn.
	CPF_PersistentInstance =				0x0002000000000000,	// A object referenced by the property is duplicated like a component. (Each actor should have an own instance.)
	CPF_UObjectWrapper =					0x0004000000000000,	// Property was parsed as a wrapper class like TSubclassOf , FScriptInterface etc., rather than a USomething*.
	CPF_HasGetValueTypeHash =				0x0008000000000000,	// This property can generate a meaningful hash value.
	CPF_NativeAccessSpecifierPublic =		0x0010000000000000,	// Public native access specifier.
	CPF_NativeAccessSpecifierProtected =	0x0020000000000000,	// Protected native access specifier.
	CPF_NativeAccessSpecifierPrivate =		0x0040000000000000,	// Private native access specifier.
	CPF_SkipSerialization =					0x0080000000000000	// Property shouldn't be serialized, can still be exported to text.
};

// https://docs.unrealengine.com/4.26/en-US/API/Runtime/CoreUObject/UObject/EObjectFlags/
// Object Flags
enum EObjectFlags
{
	RF_NoFlags = 0x00000000,
	RF_Public = 0x00000001,
	RF_Standalone = 0x00000002,
	RF_MarkAsNative = 0x00000004,
	RF_Transactional = 0x00000008,
	RF_ClassDefaultObject = 0x00000010,
	RF_ArchetypeObject = 0x00000020,
	RF_Transient = 0x00000040,
	RF_MarkAsRootSet = 0x00000080,
	RF_TagGarbageTemp = 0x00000100,
	RF_NeedInitialization = 0x00000200,
	RF_NeedLoad = 0x00000400,
	RF_KeepForCooker = 0x00000800,
	RF_NeedPostLoad = 0x00001000,
	RF_NeedPostLoadSubobjects = 0x00002000,
	RF_NewerVersionExists = 0x00004000,
	RF_BeginDestroyed = 0x00008000,
	RF_FinishDestroyed = 0x00010000,
	RF_BeingRegenerated = 0x00020000,
	RF_DefaultSubObject = 0x00040000,
	RF_WasLoaded = 0x00080000,
	RF_TextExportTransient = 0x00100000,
	RF_LoadCompleted = 0x00200000,
	RF_InheritableComponentTemplate = 0x00400000,
	RF_DuplicateTransient = 0x00800000,
	RF_StrongRefOnFrame = 0x01000000,
	RF_NonPIEDuplicateTransient = 0x02000000,
	RF_Dynamic = 0x04000000,
	RF_WillBeLoaded = 0x08000000,
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

		for (int32_t i = 0; i < Names()->Num(); i++)
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