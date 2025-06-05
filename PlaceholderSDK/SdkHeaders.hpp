#pragma once

/*
# =================================================================================================== #
# This just just a placeholder sdk so the project can actually compile! Replace this with your own!
# =================================================================================================== #
*/

#include "GameDefines.hpp"

struct FLinearColor
{
	float R;
	float G; 
	float B;
	float A;
};

struct FColor
{
	uint8_t B;
	uint8_t G;
	uint8_t R;
	uint8_t A;
};

struct UGameViewportClient_TA_execHandleKeyPress_Params
{
	int32_t ControllerId;
	struct FName Key;
	uint8_t EventType;
	float AmountDepressed;
	uint32_t bGamepad : 1;
	uint32_t ReturnValue : 1;
};

enum class EInputEvent : uint8_t
{
	IE_Pressed = 0,
	IE_Released = 1,
	IE_Repeat = 2,
	IE_DoubleClick = 3,
	IE_Axis = 4,
	IE_END = 5
};

class UObject
{
public:
	struct FPointer VfTableObject;
	int32_t ObjectInternalInteger;
	uint64_t ObjectFlags;

public:
	static TArray<class UObject*>* GObjObjects() { return GObjects; };

	std::string GetName() { return "CodeRed"; };
	std::string GetNameCPP() { return "CodeRed"; };
	std::string GetFullName() { return "CodeRed"; };
	struct FName GetPackageName() { return L"None"; };
	class UObject* GetPackageObj()  { return nullptr; };
	bool IsA(class UClass* uClass) { return false; };
	bool IsA(int objInternalInteger) { return false; };
	template<typename T> bool IsA()
	{
		return IsA(T::StaticClass());
	}
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UClass : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UField : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UStruct : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UFunction : public UStruct
{
public:
	uint64_t FunctionFlags;
	FPointer Func;

public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UAudioDevice : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UDateTime : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class AHUD : public UObject
{
public:
	class UCanvas* Canvas;

public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class ULocalPlayer : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UGameViewportClient : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UEngine : public UObject
{
public:
	TArray<class ULocalPlayer*> GamePlayers;

public:
	static UClass* StaticClass()
	{
		return nullptr;
	};

	static class UEngine* GetEngine() { return nullptr; }
	static class UAudioDevice* GetAudioDevice() { return nullptr; }
	static class AWorldInfo* GetCurrentWorldInfo() { return nullptr; }
};

class AActor : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};

	void ConsoleCommand(const std::string& cmd) {}
};

class AWorldInfo : public AActor
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class APlayerController : public AActor
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

struct FFrame
{
	class UStruct* Node;
	class UObject* Object;
	uint8_t* Code;
	uint8_t* Locals;
};

/*
# =================================================================================================== #
# This just just a placeholder sdk so the project can actually compile! Replace this with your own!
# =================================================================================================== #
*/