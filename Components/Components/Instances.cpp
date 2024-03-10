#include "Instances.hpp"
#include "../Includes.hpp"

InstancesComponent::InstancesComponent() : Component("Instances", "Manages class instances and objects.") { OnCreate(); }

InstancesComponent::~InstancesComponent() { OnDestroy(); }

void InstancesComponent::OnCreate()
{
	I_UCanvas = nullptr;
	I_AHUD = nullptr;
	I_UGameViewportClient = nullptr;
	I_APlayerController = nullptr;
}

void InstancesComponent::OnDestroy()
{
	m_staticClasses.clear();
	m_staticFunctions.clear();

	for (UObject* uObject : m_createdObjects)
	{
		if (uObject)
		{
			MarkForDestory(uObject);
		}
	}

	m_createdObjects.clear();
}

class UClass* InstancesComponent::FindStaticClass(const std::string& className)
{
	if (m_staticClasses.empty())
	{
		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject)
			{
				if ((uObject->GetFullName().find("Class") == 0) && !(uObject->ObjectFlags & EObjectFlags::RF_Transient))
				{
					m_staticClasses[uObject->GetFullName()] = static_cast<UClass*>(uObject);
				}
			}
		}
	}

	if (m_staticClasses.contains(className))
	{
		return m_staticClasses[className];
	}

	return nullptr;
}

class UFunction* InstancesComponent::FindStaticFunction(const std::string& className)
{
	if (m_staticFunctions.empty())
	{
		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject && uObject->IsA<UFunction>())
			{
				m_staticFunctions[uObject->GetFullName()] = static_cast<UFunction*>(uObject);
			}
		}
	}

	if (m_staticFunctions.contains(className))
	{
		return m_staticFunctions[className];
	}

	return nullptr;
}

void InstancesComponent::MarkInvincible(class UObject* object)
{
	if (object)
	{
		object->ObjectFlags &= ~EObjectFlags::RF_Transient;
		object->ObjectFlags &= ~EObjectFlags::RF_TagGarbageTemp;
		object->ObjectFlags |= EObjectFlags::RF_Public;
		object->ObjectFlags |= EObjectFlags::RF_Standalone;
		object->ObjectFlags |= EObjectFlags::RF_MarkAsRootSet;
		object->ObjectFlags |= EObjectFlags::RF_KeepForCooker;
	}
}

void InstancesComponent::MarkForDestory(class UObject* object)
{
	if (object)
	{
		object->ObjectFlags = 0;
		object->ObjectFlags |= EObjectFlags::RF_Public;
		object->ObjectFlags |= EObjectFlags::RF_Transient;
		object->ObjectFlags |= EObjectFlags::RF_TagGarbageTemp;
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
	if (I_UCanvas != canvas)
	{
		I_UCanvas = canvas;
	}
}

void InstancesComponent::SetHUD(class AHUD* hud)
{
	if (I_AHUD != hud)
	{
		I_AHUD = hud;
	}
}

void InstancesComponent::SetGameViewportClient(class UGameViewportClient* viewportClient)
{
	if (I_UGameViewportClient != viewportClient)
	{
		I_UGameViewportClient = viewportClient;
	}
}

void InstancesComponent::SetPlayerController(class APlayerController* playerController)
{
	if (I_APlayerController != playerController)
	{
		I_APlayerController = playerController;
	}
}

void InstancesComponent::Initialize()
{
	Console.Write(GetNameFormatted() + "Initialized!");
}

class InstancesComponent Instances{};