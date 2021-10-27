#include "Instances.hpp"
#include "../Includes.hpp"

InstancesComponent::InstancesComponent() : Component("Instances", "Manages class instances and objects.")
{
	I_UCanvas = nullptr;
	I_AHUD = nullptr;
	I_UGameViewportClient = nullptr;
	I_APlayerController = nullptr;
}

InstancesComponent::~InstancesComponent() { }

void InstancesComponent::MarkInvincible(class UObject* object)
{
	if (object)
	{
		object->ObjectFlags &= ~EObjectFlags::RF_Transient;
		object->ObjectFlags |= EObjectFlags::RF_Public;
		object->ObjectFlags |= EObjectFlags::RF_Standalone;
		object->ObjectFlags |= EObjectFlags::RF_MarkAsRootSet;
	}
}

void InstancesComponent::MarkForDestory(class UObject* object)
{
	if (object)
	{
		object->ObjectFlags = 0;
		object->ObjectFlags |= EObjectFlags::RF_Transient;
		object->MarkPendingKill();
	}
}

class UEngine* InstancesComponent::IUEngine()
{
	return UEngine::GetEngine();
}

class UAudioDevice* InstancesComponent::IUAudioDevice()
{
	return UEngine::GetAudioDevice();
}

class AWorldInfo* InstancesComponent::IAWorldInfo()
{
	return UEngine::GetCurrentWorldInfo();
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
	Console.Write(GetNameFormatted() + "Instances Initialized!");
}

class InstancesComponent Instances{};