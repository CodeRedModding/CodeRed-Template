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
	bool bGamepad : 1;
	bool ReturnValue : 1;
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

public:
	static TArray<class UObject*>* GObjObjects() { return GObjects; };

	std::string GetName() { return "CodeRed"; };
	std::string GetNameCPP() { return "CodeRed"; };
	std::string GetFullName() { return "CodeRed"; };
	struct FName GetPackageName() { return L"None"; };
	class UObject* GetPackageObj()  { return nullptr; };
	bool IsA(class UClass* uClass) { return false; };
	bool IsA(int objInternalInteger) { return false; };

	static UClass* StaticClass()
	{
		return nullptr;
	};

	struct FColor MakeColor(int32_t r, int32_t g, int32_t b, int32_t a) { return FColor { static_cast<uint8_t>(b), static_cast<uint8_t>(g), static_cast<uint8_t>(r), static_cast<uint8_t>(a) }; }
	struct FLinearColor ColorToLinearColor(struct FColor color) { return FLinearColor{ static_cast<float>(color.R), static_cast<float>(color.G), static_cast<float>(color.B), static_cast<float>(color.A) }; }
	struct FColor LinearColorToColor(struct FLinearColor linearColor) { return FColor{ static_cast<uint8_t>(linearColor.B), static_cast<uint8_t>(linearColor.G), static_cast<uint8_t>(linearColor.R), static_cast<uint8_t>(linearColor.A) }; }
};

class UClass : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
};

class UFunction : public UObject
{
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

	class UAudioDevice* GetAudioDevice() { return nullptr;  }
	class AWorldInfo* GetCurrentWorldInfo() { return nullptr; }
};

class AActor : public UObject
{
public:
	static UClass* StaticClass()
	{
		return nullptr;
	};
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

/*
# =================================================================================================== #
# This just just a placeholder sdk so the project can actually compile! Replace this with your own!
# =================================================================================================== #
*/