#pragma once
#include "../Component.hpp"

// This is also largely dependent on your game, automatically stores active class instances here to be retrieved at any time.

class InstancesComponent : public Component
{
public:
	InstancesComponent();
	~InstancesComponent();

public:
	std::map<std::string, class UClass*> StaticClasses;
	std::map<std::string, class UFunction*> StaticFunctions;

public:
	// All GetInstanceOf related functions loop through the entire GObject TArray, which can be resource heavy if you're using them everywhere.
	// So rely on hooking functions and storing classes here instead, only use GetInstanceOf functions them when needed.

	// Get the default constructor of a class type. Example: UGameData_TA* gameData = GetDefaultInstanceOf<UGameData_TA>();
	template<typename T> T* GetDefaultInstanceOf()
	{
		UClass* staticClass = T::StaticClass();

		for (int32_t i = UObject::GObjObjects()->Num(); i > 0; i--)
		{
			UObject* uObject = UObject::GObjObjects()->At(i);

			if (uObject && uObject->IsA(staticClass))
			{
				if (uObject->GetFullName().find("Default") != std::string::npos)
				{
					return reinterpret_cast<T*>(uObject);
				}
			}
		}

		return nullptr;
	}

	// Get the most current/active instance of a class. Example: UEngine* engine = GetInstanceOf<UEngine>();
	template<typename T> T* GetInstanceOf()
	{
		UClass* staticClass = T::StaticClass();

		for (int32_t i = UObject::GObjObjects()->Num(); i > 0; i--)
		{
			UObject* uObject = UObject::GObjObjects()->At(i);

			if (uObject && uObject->IsA(staticClass))
			{
				if (uObject->GetFullName().find("Default") == std::string::npos)
				{
					return reinterpret_cast<T*>(uObject);
				}
			}
		}

		return nullptr;
	}

	template<typename T> // Get all active instances of a class type. Example: std::vector<APawn*> pawns = GetAllInstancesOf<APawn>();
	std::vector<T*> GetAllInstancesOf()
	{
		std::vector<T*> objectInstances;
		UClass* staticClass = T::StaticClass();

		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject && uObject->IsA(staticClass))
			{
				if (uObject->GetFullName().find("Default") == std::string::npos)
				{
					objectInstances.push_back(reinterpret_cast<T*>(uObject));
				}
			}
		}

		return objectInstances;
	}

	// Get all default instances of a class type.
	template<typename T> std::vector<T*> GetAllDefaultInstancesOf()
	{
		std::vector<T*> objectInstances;
		UClass* staticClass = T::StaticClass();

		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject && uObject->IsA(staticClass))
			{
				if (uObject->GetFullName().find("Default") != std::string::npos)
				{
					objectInstances.push_back(reinterpret_cast<T*>(uObject));
				}
			}
		}

		return objectInstances;
	}

	// Get an object instance by it's name and class type. Example: UTexture2D* texture = FindObject<UTexture2D>("WhiteSquare");
	template<typename T> T* FindObject(const std::string& objectName)
	{
		UClass* staticClass = T::StaticClass();

		for (int32_t i = UObject::GObjObjects()->Num(); i > 0; i--)
		{
			UObject* uObject = UObject::GObjObjects()->At(i);

			if (uObject && uObject->IsA(staticClass))
			{
				std::string objectFullName = uObject->GetFullName();

				if (objectFullName == objectName || objectFullName.find(objectName) != std::string::npos)
				{
					return reinterpret_cast<T*>(uObject);
				}
			}
		}

		return nullptr;
	}

	// Get all object instances by it's name and class type. Example: std::vector<UTexture2D*> textures = FindAllObjects<UTexture2D>("Noise");
	template<typename T> std::vector<T*> FindAllObjects(const std::string& objectName)
	{
		std::vector<T*> objectInstances;
		UClass* staticClass = T::StaticClass();

		for (int32_t i = 0; i < UObject::GObjObjects()->Num(); i++)
		{
			UObject* uObject = UObject::GObjObjects()->At(i);

			if (uObject && uObject->IsA(staticClass))
			{
				std::string objectFullName = uObject->GetFullName();

				if (objectFullName == objectName || objectFullName.find(objectName) != std::string::npos)
				{
					objectInstances.push_back(reinterpret_cast<T*>(uObject));
				}
			}
		}

		return objectInstances;
	}

	// Creates a new transient instance of a class which then adds it to globals.
	// YOU are required to make sure these objects eventually get eaten up by the garbage collector in some shape or form.
	// Example: UObject* newObject = CreateInstance<UObject>();
	template<typename T> T* CreateInstance()
	{
		T* returnObject = nullptr;
		T* defaultObject = GetDefaultInstanceOf<T>();
		UClass* staticClass = T::StaticClass();

		if (defaultObject && staticClass)
		{
			returnObject = reinterpret_cast<T*>(defaultObject->DuplicateObject(defaultObject, defaultObject->Outer, staticClass));
		}

		// Making sure newly created object doesn't get randomly destoyed by the garbage collector when we don't want it do.
		if (returnObject)
		{
			returnObject->ObjectFlags &= ~EObjectFlags::RF_Transient;
			returnObject->ObjectFlags |= EObjectFlags::RF_Public;
			returnObject->ObjectFlags |= EObjectFlags::RF_Standalone;
			returnObject->ObjectFlags |= EObjectFlags::RF_MarkAsRootSet;
		}

		return returnObject;
	}

private: 
	class UEngine* I_UEngine;
	class UDateTime* I_UDateTime;
	class UCanvas* I_UCanvas;
	class AHUD* I_AHUD;
	class UGameViewportClient* I_UGameViewportClient;
	class APlayerController* I_APlayerController;

public: // Use these functions to access these specific class instances, they will be set automatically; always remember to null check!
	class UEngine* IUEngine();
	class UAudioDevice* IUAudioDevice();
	class AWorldInfo* IAWorldInfo();
	class UDateTime* IUDateTime();
	class UCanvas* IUCanvas();
	class AHUD* IAHUD();
	class UGameViewportClient* IUGameViewportClient();
	class ULocalPlayer* IULocalPlayer();
	class APlayerController* IAPlayerController();

public: // These should only be used by function hooks, do not manually call these.
	void SetEngine(class UEngine* engine);
	void SetDatTime(class UDateTime* dateTime);
	void SetCanvas(class UCanvas* canvas);
	void SetHUD(class AHUD* hud);
	void SetGameViewportClient(class UGameViewportClient* viewportClient);
	void SetPlayerController(class APlayerController* playerController);
	void MapObjects(); // Maps all static objects to "StaticClasses" and "StaticFunctions".
	void Initialize(); // Initialize classes that can't be grabbed from function hooks.
};

extern class InstancesComponent Instances;