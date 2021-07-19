#include "Instances.hpp"
#include "Console.hpp"

InstancesComponent::InstancesComponent() : Component("Instances", "Manages class instances and objects.")
{
	I_UEngine = nullptr;
	I_UDateTime = nullptr;
	I_UCanvas = nullptr;
	I_AHUD = nullptr;
	I_UGameViewportClient = nullptr;
	I_APlayerController = nullptr;
}

InstancesComponent::~InstancesComponent() { }

class UEngine* InstancesComponent::IUEngine()
{
	return I_UEngine;
}

class UAudioDevice* InstancesComponent::IUAudioDevice()
{
	UEngine* engine = IUEngine();

	if (engine)
	{
		UAudioDevice* audioDevice = engine->GetAudioDevice();

		if (audioDevice)
		{
			return audioDevice;
		}
	}

	return nullptr;
}

class AWorldInfo* InstancesComponent::IAWorldInfo()
{
	UEngine* engine = IUEngine();

	if (engine)
	{
		AWorldInfo* worldInfo = engine->GetCurrentWorldInfo();

		if (worldInfo)
		{
			return worldInfo;
		}
	}

	return nullptr;
}

class UDateTime* InstancesComponent::IUDateTime()
{
	return I_UDateTime;
}

class UCanvas* InstancesComponent::IUCanvas()
{
	return I_UCanvas;
}

class AHUD* InstancesComponent::IAHUD()
{
	return I_AHUD;
}

class UGameViewportClient* InstancesComponent::IUGameViewportClient()
{
	return I_UGameViewportClient;
}

class ULocalPlayer* InstancesComponent::IULocalPlayer()
{
	UEngine* engine = IUEngine();

	if (engine && engine->GamePlayers[0])
	{
		return engine->GamePlayers[0];
	}

	return nullptr;
}

class APlayerController* InstancesComponent::IAPlayerController()
{
	return I_APlayerController;
}

void InstancesComponent::SetEngine(class UEngine* engine)
{
	I_UEngine = engine;
}

void InstancesComponent::SetDatTime(class UDateTime* dateTime)
{
	I_UDateTime = dateTime;
}

void InstancesComponent::SetCanvas(class UCanvas* canvas)
{
	I_UCanvas = canvas;
}

void InstancesComponent::SetHUD(class AHUD* hud)
{
	I_AHUD = hud;
}

void InstancesComponent::SetGameViewportClient(class UGameViewportClient* viewportClient)
{
	I_UGameViewportClient = viewportClient;
}

void InstancesComponent::SetPlayerController(class APlayerController* playerController)
{
	I_APlayerController = playerController;
}

void InstancesComponent::MapObjects()
{
	for (UObject* uObject : *UObject::GObjObjects())
	{
		if (uObject)
		{
			if (uObject->GetPackageName() != FName(L"Transient"))
			{
				std::string objectFullName = uObject->GetFullName();

				if (objectFullName.find("Class ") == 0)
				{
					StaticClasses.emplace(objectFullName, reinterpret_cast<UClass*>(uObject));
				}
				else if (objectFullName.find("Function ") == 0)
				{
					StaticFunctions.emplace(objectFullName, reinterpret_cast<UFunction*>(uObject));
				}
			}
		}
	}
}


void InstancesComponent::Initialize()
{
	MapObjects();
	SetEngine(GetInstanceOf<UEngine>());
	SetDatTime(GetDefaultInstanceOf<UDateTime>());

	Console.Write(GetNameFormatted() + "Instances Initialized!");
}

class InstancesComponent Instances;