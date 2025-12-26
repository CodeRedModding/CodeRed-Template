#pragma once
/*
    Changes in v1.6.5:
    - Made the "VariableComponent::GetCommand" and "VariableComponent::GetSetting" functions const in "Variables.hpp/cpp".
    - Improved the "Rotator::UnrealRotator" function in "Math.cpp" to avoid unnecessary math functions.
    - Fixed the "Rotator::FromUnrealRotator" function not working as intended outside of the "Rotator" constructor in "Math.cpp".

    Changes in v1.6.4:
    - Added a "WaitForSingleObject" check in the "CoreComponent::OnDestroy" function before calling "CloseHandle" in "Core.cpp".
    - Added two new variables "std::mutex m_logMutex" and "std::mutex m_outMutex" in "Console.hpp/cpp", to add protections on writing from different threads.
    - Replaced the "std::atomic<bool> m_queueLocked" variable in "Manager.hpp" to "std::mutex m_queueMutex", for better thread handling.
    - Changed some constant variables and class variables to be larger sizes to make converting to different versions easier, as well as different archetypes.

    Changes in v1.6.3:
    - Added a check if the given string was empty or not in the "Format::RemoveFileChars" function in "Formatting.cpp".
    - Changed the "VARIABLE_NAME_LENGTH" and "VARIABLE_VALUE_LENGTH" constant expressions to use "size_t" instead of "uint32_t" in "Variables.hpp".
    - More minor spelling fixes throughout the project.

    Changes in v1.6.2:
    - BREAKING: Moved the "ManagerComponent::OnTick" function call from "ManagerComponent::OnCanvasDraw" to "HUDPostRender" in "Events.cpp".
    - BREAKING: Removed the "multiplier" argument in one of the constructors for the "ManagerQueue" class in "Manager.hpp/cpp".
    - BREAKING: Changed the "ManagerQueue"'s delta variable to be a float instead of a uint64_t, also changed its delay variable from a uint64 to a uint32_t in "Manager.hpp/cpp".
    - BREAKING: Wrapped the "GRainbowColor" and "GColorList" classes in the "CodeRed" namespace in "Colors.hpp/cpp".
    - BREAKING: Renamed the "GRainbowColor::GetByte" function to "GRainbowColor::GetColor" in "Colors.hpp/cpp".
    - Changed the rng character sets in the "Format::GetCharacterSet" function to be static strings instead of char arrays in "Formatting.cpp".
    - Remade the "ManagerQueue" class in "Manager.hpp/cpp" completely, also changed all the functions in the "ManagerComponent" class to accommodate this.
    - Added three new functions "Variable::HasFlags", "Variable::AddFlags",and "Variable::RemoveFlags" in "Variables.hpp/cpp", and converted all functions in the "Setting" and "Command" classes to use them.
    - Added a new constant variable "QUEUE_TICK_MULTIPLIER" in "Manager.hpp", used to properly time async commands.
    - Added a new constant variable "QUEUE_TICK_RATE" in "Manager.hpp", used to properly calculate async commands.
    - Fixed a rare bug possible thread racing issue related to the thread queue in the "ManagerComponent::OnTick" function in "Manager.cpp".

    Changes in v1.6.1:
    - BREAKING: Renamed the "MandarianOrange" color in the "GColorList" class to "MandarinOrange" in "Colors.hpp".
    - Moved both the "VARIABLE_NAME_LENGTH" and "VARIABLE_VALUE_LENGTH" constants from "Variables.cpp" to its header file "Variables.hpp".
    - Changed the static colors in the "GColorList" class to use normal hex codes in "Colors.cpp", to make them easier to read and modify.

    Changes in v1.6.0:
    - BREAKING: Added a new "FunctionHook" class and changed the entire "EventsComponent" class to use these, they support both Detours and MinHook and are a much more dynamic way of hooking functions. In "Types.hpp" there are macros that let you choose between Detours and MinHook.
    - BREAKING: Added support to hook ProcessInternal on top of ProcessEvent, in the "EventsComponent" class in "Events.hpp/cpp".
    - BREAKING: Renamed the "EventsComponent::IsEventBlacklisted" function to just "EventsComponent::IsBlacklisted" in "Events.hpp/cpp".
    - BREAKING: Renamed several detour functions in "Events.hpp/cpp" to use the word "callable" instead, this applies to the classes "PreEvent", "PostEvent", and "EventsComponent".
    - Added a new "EventPair" class used by the new "EventGuard" class in "Events.hpp/cpp", which is used to prevent duplicate calls when hooking multiple functions (such as ProcessEvent, ProcessInternal, or even CallFunction).
    - Added several macros at the top of the "Events.cpp" file, that can help you with tracing function hooks or to debug the new guard system.
    - Minor spelling fixes and changes throughout the project, as well as some additional code reorganization.

    Changes in v1.5.9:
    - Added a new "EventTypes" enum in "Types.hpp", which is now used by the hook classes in "Events.hpp/cpp" to let you tell where a function was called from.
    - Changed the "ProcessBefore" and "ProcessAfter" functions to use the new "EventTypes" enum.

    Changes in v1.5.8:
    - BREAKING: Renamed the "QueueData" class in "Manager.hpp/cpp" to "ManagerQueue".
    - BREAKING: Changed the "ToHex" notion parameters for the functions in "Formatting.hpp/cpp".
    - Added two new virtual functions for the "Module" class in "Module.hpp/cpp", "OnCreate" and "OnDestroy", which work the same way as the ones from the "Component" class.
    - Added two new random string generator functions "RandomString" and "RandomCharacter" in "Formatting.hpp/cpp".
    - Added a new templated "Shuffle" function in "Formatting.hpp/cpp".
    - Improvements to the "VariableComponent::CreateSetting" and "VariableComponent::CreateCommand" functions in "Variables.hpp/cpp".
    - Improved the "Format::ReplaceString" function and fixed some bugs with it in "Formatting.hpp/cpp".
    - Renamed the "EventsComponent::IsDetoured" function to "EventsComponent::AreDetoursAttached" in "Events.hpp/cpp".
    - Various other improvements and bug fixes to the functions in "Formatting.hpp/cpp".

    Changes in v1.5.7:
    - BREAKING: Added a new "VariableComponent" and moved all command and setting related functions from "ManagerComponent" to this new class.
    - BREAKING: Changed all the "Initialize" functions return a bool value, as well as making them a virtual function for the "Component" class in "Component.hpp/cpp".
    - BREAKING: Renamed the toggle argument functions in the "Setting" class to string callback in "Variables.hpp/cpp".
    - BREAKING: Renamed the "CommandTypes" enum in "Manager.hpp" to "CommandResults".
    - Moved the "INSTANCES_INTERATE_OFFSET" constant from "Instances.hpp" to "Types.hpp".
    - Added a new system for saving and loading user settings, check out the "VariableComponent" class for all the new features.
    - Added new month constants in the "Time.hpp" file to be used with the "Time" class.
    - Added a new "Byte" setting type for the "Setting" class in "Manager.hpp/cpp".
    - Changed the underlying type of the "VariableIds" enum from "int32_t" to "uint32_t".
    - Reorganized a lot of the functions and variables for all the components.

    Changes in v1.5.6:
    - BREAKING: Moved the "DisableThreadLibraryCalls" function call from "Core.cpp" to "dllmain.cpp".
    - BREAKING: Renamed the function "CoreComponent::InitializeGlobals" to "CoreComponent::OnThreadCreated" in "Core.hpp/cpp", and added comments for better a explanation for why this is needed.
    - Made the "m_mainThread" property from "CoreComponent" to be both static and atomic, in "Core.hpp".

    Changes in v1.5.5:
    - BREAKING: Wrapped everything that's custom or unique to this project in the "CodeRed" namespace.
    - Added a new "Types.hpp" file, and moved the several enums such as "ThreadTypes" and "TextColors" to "Types.hpp" instead.
    - Added a new function "Setting::IsType" for the "Setting" class in "Manager.hpp/cpp".
    - Added extra range and safety checks for the setting functions in "Manager.cpp".
    - Added extra formatting checks for the "Setting::SetStringValue" function in "Manager.cpp".
    - Changed the "FindPattern" function in "Memory.cpp" and added an offset parameter with a default value of "0".
    - Changed the "INSTANCES_INTERATE_OFFSET" value from 100 to 10 in "Instances.hpp", I've since found anything above 1 works fine so 10 is just for safety.
    - Removed the default constructor for both the "Component" and "Module" classes itself, they should not be used.
    - Various code organizations and refactoring throughout the project.

    Changes in v1.5.4:
    - Added the "EObjectFlags::RF_RootSet" flag and removed "EObjectFlags::RF_Transient" for the "MarkInvincible/MarkForDestroy" functions in "Instances.hpp/cpp".

    Changes in v1.5.3:
    - Internal improvements and optimizations to setting functions in "Manager.cpp".

    Changes in v1.5.2:
    - Added a new "Time::FromEpoch" function in "Time.hpp/cpp" which converts a epoch timestamp into the "Time" class.
    - Fixed the rotate functions in "Math.hpp/cpp" using "RADIANS_TO_ROTATION" instead of "DEGREES_TO_RADIANS" for applying rotation, also remade them to be more clear of what they do.

    Changes in v1.5.1:
    - BREAKING: Renamed the "Timestamp::FormatStandard" function to "Timestamp::FormatClock" in "Time.hpp/cpp".
    - Added a new "Time::FromEpoch" function for the time class that parses an epoch timestamp in "Time.hpp/cpp".
    - Added two new timestamp format functions "Timestamp::FormatUTCSimple" and "Timestamp::FormatUTCExtended" in "Time.hpp/cpp".
    - Added two new helper functions "ConsoleComponent::CreateTimestampUTCSimple" and "ConsoleComponent::CreateTimestampUTCExtended" in "Console.hpp/cpp".

    Changes in v1.5.0:
    - Added a new "ManagerComponent::OnCanvasDraw" function to control all the individual canvas drawing functions for each module, this should be called by your hud render function.
    - Added a new virtual void "Module::OnCreateVariables" for modules where you can create module specific settings or commands instead of putting them all in the "ManagerComponent::Initialize()" function. This is called by the "ManagerComponent::CreateModule" function in "Manager.cpp".
    - Changed the "UpdateSettings" function from the placeholder mod to be a virtual void and renamed it to "OnSettingChanged", this should be called by your function when its value has changed.
    - Added a new virtual void "Module::OnCanvasDraw" for modules, this is called by the new "ManagerComponent::OnCanvasDraw" function in "Manager.cpp".
    - The "ManagerComponent::OnTick" function is now called by the new "ManagerComponent::OnCanvasDraw" function in "Manager.cpp".

     Changes in v1.4.9:
    - Added two new settings for commands called "IsLocked" and "NeedsArgs", locked prevents the command from being called, and needs args states that the command requires some sort of argument to be passed through. Setting this false means arguments are optional for the command.
    - Added a new "Variable" class which both the "Setting" and "Command" class now inherit from, this stores the variable and internal flags / permissions for said variable.
    - Added two new internal constants, "VARIABLE_NAME_LENGTH" and "VARIABLE_VALUE_LENGTH", in Manager.cpp" which let you set the max length for variable values and names.
    - BREAKING: Made the "ManagerComponent::CreateVariable" function private, as a result the "CreateCommand" and "CreateSetting" now require an additional argument for creating them for their names.
    - BREAKING: Got rid of all the optional constructors for the "Setting" class in "Manager.hpp/cpp", you should now call the setter functions to bind specific callbacks or set permissions.
    - BREAKING: Renamed the "IsSearchable" function for the command and setting classes to to "IsHidden" in "Manager.hpp/cpp".
    - BREAKING: Renamed the "Command::BindCallback" function to "Command::BindStringCallback" in "Manager.hpp/cpp".
    - BREAKING: Renamed the "Setting::IsModifiable" function and member fields to "IsLocked", which is now set by flags in the new "Variable" base class.

    Changes in v1.4.8:
    - Added a new internal queue system in "Manager.hpp/cpp" which allows console commands to be safely called from any thread.
    - Added extra safety checking for the "Setting::InRange" function in "Manager.cpp".
    - Changed the "ManagerComponent::UnrealCommand" in "Manager.cpp" to be more efficient.
    - BREAKING: Renamed all the "SettingTypes" enum values in "Manager.hpp", also added support for Int64's.
    - BREAKING: Renamed all the "CommandTypes" enum values in "Manager.hpp".
    - BREAKING: Renamed all the "Setting::SetInt" functions to "Setting::SetInt32" in "Manager.hpp/cpp", this applies to the get, set, and range functions.

    Changes in v1.4.7:
    - Added extra null checking and safety checks for both "Instances.hpp/cpp" and "Events.cpp".
    - Removed the "MarkForDestroy" loop in "Instances.cpp" in the deconstructor, if they were already deleted by the game then this would be accessing random memory addresses (which is bad).

    Changes in v1.4.6:
    - Added two new functions "ProcessBefore" and "ProcessAfter" to "Events.hpp/cpp" to more easily process function hooks, this makes detouring multiple things such as process event and call function easier.
    - Added a new atomic bool to "EventsComponent" to prevent a thread racing issue when hooking functions.

    Changes in v1.4.5:
    - Wrapped all the extensions and helper classes in a new "CodeRed" namespace, this is to prevent possible naming conflicts with other libraries.
    - BREAKING: Removed the "UnrealMemory.hpp/cpp" files from "Extensions" as it was unnecessary to have in the first place.
    - BREAKING: Renamed the "ThreadTypes" enum values as well as moved the whole enum from "Manager.hpp" to "Module.hpp".

    Changes in v1.4.4:
    - Changed the member variables and functions in "Manager.hpp" being static to just normal, I can't remember why I made them static in the first place.
    - Fixed a rare case of the "Color::FromHex" function in "Colors.cpp" not parsing a hex string properly.

    Changes in v1.4.3:
    - Added a new "HooksComponent" in "Events.hpp/cpp", this is to be used in place of the "Hooks" namespace.
    - Remade the examples for hooking functions in the "EventsComponent::Initialize()" function inside "Events.cpp".
    - Renamed all the class variables in "Manager.hpp/cpp" to be consistent with the rest of the project.

    Changes in v1.4.2:
    - Fixed the "MarkInvincible" and "MarkForDestroy" functions in "Instances.cpp" using the wrong flags.

    Changes in v1.4.1:
    - Added a new "GetOrCreateInstance" function in "Instances.hpp", if no instance of a class is found it will create one.
    - Improved all of the "GetInstanceOf" functions again to prevent a possible crash in "Instances.hpp".
    - Improved the "FindStaticClass" and "FindStaticFunction" functions in "Instances.cpp" to also be more safe.
    - The "MarkForDestroy" function will now also remove the object from the cached created object list if one is found.

    Changes in v1.4.0:
    - Revamped the entire project to use modern C++ standards, functions, and naming conventions.
    - Remade the console component in "Console.hpp/cpp", added macros for logging to std::cout, opening a console window, and using 24 hour or 12 hour timestamps.
    - Added new timestamp and clock utils in "Extensions/Time.hpp".
    - Added new random number and letter functions in "Math.hpp", as well as added a new "Plane" class.
    - Improved all the color classes and functions in "Colors.hpp/cpp", also added hashing and sorting operators.
    - Improved all the functions in "Formatting.hpp/cpp", as well as added inline functions and bug fixes.
    - Fixed a crash that could happen when using any of the "GetInstanceOf" functions.

    Changes in v1.3.9:
    - Added new functions for calling console commands with delays! See the "AsyncConsoleCommand" functions in "Manager.hpp/cpp".
    - Moved the "GRainbowColor::OnTick()" call to inside the "ManagerComponent::OnTick()" function in "Manager.cpp".
    - Optimized various instance functions in "Instances.hpp".
    - Optimized the color classes "FromHex" function in "Colors.cpp", also add support for parsing alpha channels.
    - Updated the "PlaceholderSDK" functions and classes.

    Changes in v1.3.8:
    - Remade and optimized a lot of the helper functions in "Formatting.hpp/cpp".

    Changes in v1.3.7:
    - Added extra radians, rotation, and degree conversion helpers in "Math.hpp".
    - Added a "FromUnrealRotator" function in the "Rotator" class in "Math.hpp/cpp", this helps auto convert unreal rotational units to degrees (0 to 360).
    - Fixed "IsStringDecimal" returning false for negative values in "Formatting.cpp".

    Changes in v1.3.6:
    - Added and improved constructors and operator overloads for the classes throughout "Manager.hpp/cpp".
    - Improved the "MarkInvincible" and "MarkForDestory" functions in "Instances.cpp".
    - Changed how variables are stored in "Manager.hpp", also variables names are now automatically set to lower case.

    Changes in v1.3.5:
    - Upgraded and improved a lot of the functions and classes in "Colors.hpp/cpp", as well as made the "Colors" namespace functions inline.
    - Added some inline functions for the string utils in "Formatting.hpp/cpp".

    Changes in v1.3.4:
    - Upgraded the project to ISO C++20.
    - Added an extra null check in "EventsComponent::ProcessEventDetour" in "Events.cpp".
    - Added three new functions, "Lerp", "Midpoint" and "DistanceTo", to all the vector classes in "Math.hpp/cpp".
    - Added a built in "VectorF::Rotate" function  in "Math.hpp/cpp".
    - Added a new templated "RandomRange" function in the "Math" namespace in "Math.hpp".
    - Added the "<numeric>" and "<random>" headers include to "pch.hpp" because that's needed for the new/updated functions in "Math.cpp".
    - Added new "Formatting::ReplaceAllChars" and "Formatting::Contains" functions in "Formatting.hpp/cpp".
    - Replaced all the "IsChar" functions in "Formatting.hpp/cpp" and replaced them with C++ library functions instead.
    - Changed the "ToLower" and "ToUpper" functions to use "std::transform" instead in "Formatting.cpp".
    - Fixed the "Formatting::RemoveFileChars" function missing a character in "Formatting.cpp".
    - Removed the "RotateUnreal" and "GetDistance" functions from the "Math" namespace, please use the built in functions in the vector classes instead.

    Changes in v1.3.3:
    - Revamped the console command and queue system, replaced it with "InternalCommand" and "ConsoleCommand" functions, they now automatically queue based on which thread you are calling them from!
    - Added a bunch of extra functions to the "Color" and "LinearColor" classes, they can convert the underlying color values to decimal or hex in "Colors.hpp/cpp".
    - Added some template safety checks in "Instances.hpp".
    - Added a "Conjugate" function to the "Quat" class in "Math.hpp/cpp"
    - Changed how the "UnrealCommand" function works in "Manager.cpp", added checks to remove the original command and only pass the arguments.
    - Changed how color values are stored in the "Setting" class in "Manager.cpp", they are now stored as their hex values instead of decimal.
    - Changed/added functions for how the "IsValueValid" function is determined for the "Setting" class in "Manager.cpp" as well how "SetStringValue" works in combination.
    - Fixed the "GetInverse" and modified the "Rotate" function which returns a vector rotated by the given quat.
    - Removed the "bOverride" option for the "Setting" class in "Manager.hpp/cpp".

    Changes in v1.3.2:
    - Added some console logging ifdefs to remind new people using this template how it works.
    - Added some extra null checks for the functions in "Instances.hpp".
    - Added a default bool argument for the "SetStringValue" function in the "Setting" class called "bOverride", if set to true when setting a string it skips and safety checks internally.
    - Changed the "ResetToDefault" function in "Setting" class to use the new bool argument set to true as explained above.
    - Fixed color values not resetting to default properly if you called the function from a different thread.
    - Fixed the "DecimalToHex" function in "Colors.cpp".

    Changes in v1.3.1:
    - Remade and added tons of extra functions to the "Setting" class in "Manager.hpp/cpp", the additions and changes include all new range and value functions, as well as extra safety checks.
    - Remade and added a bunch of extra helper functions in "Formatting.hpp", you should uh check them out if you want.
    - Changed the "AreGlobalsValid" function in "Core.cpp".

    Changes in v1.3.0:
    - Modified some of the arguments for function hooking in "Events.hpp", as well as added extra functions for using their index directly.
    - Improved and fixed various operators and functions for the color and math classes in both "Colors.hpp/cpp" and "Math.hpp/cpp".

    Changes in v1.2.9:
    - Fixed the "IsStringNumber" and "IsStringFloat" functions from returning false if the given string was negative in "Formatting.cpp".
    - Added two new functions, "ToLinear" and "ToColor", to the color structs in "Colors.hpp" which auto converts color types from one to the other.
    - Added a new setting type, "TYPE_ROTATOR", and added a "GetRotatorValue" to accommodate this for the "Setting" class in "Manager.hpp/cpp".
    - Added a "GetQuat" function which returns a quat version of a Rotator in "Math.hpp/cpp".
    - Changed the constructor for the "Setting" class in "Manager.hpp", moved the "SettingTypes" type to the front.
    - Changed the example for initializing module variables in the "Initialize" function in "Manager.cpp".

    Changes in v1.2.8:
    - Added the function "IsValueValid" for the "Setting" class, this checks if the given string value is valid for the settings type and is safe to parse. Both "InRange" and "SetVale" now use this function in "Manager.cpp".
    - Added a new enum called "ThreadTypes" in "Manager.hpp", used in settings for identifying when it is safe to call certain functions.
    - Added a default "ThreadTypes" argument for the functions "SetValue" and "ResetToDefault" for the "Setting" class in "Manager.hpp/cpp", if updating the setting is done outside of the game's thread (like ImGui) it will automatically queue the command for you.
    - Made certain members and functions in "Manager.hpp" static.

    Changes in v1.2.7:
    - Added four new functions in "Formatting.hpp", "IsStringNumber", "IsStringFloat", "EraseAllChars", and "RemoveAllChars".
    - Fixed a bug in the "SetValue" function that prevented some setting types from applying in "Manager.cpp".
    - Made the "DecimalToHex" function have a fill of 6 now in "Colors.cpp".
    - BREAKING: Renamed the function "AddToQueue" to "QueueCommand" and changed the "CommandQueue" to use a new custom class in "Manager.hpp/cpp".

    Changes in v1.2.6:
    - Fixed functions still going through Process Event if their detour was set to false if there were multiple voids bound to the same function.
    - Added the "IsEventBlacklisted" function in "Events.hpp" as part of the "ManagerComponent", and also made the "PostEvent" class call this in its "Detour" function in "Manager.cpp".
    - Added a special case for setting a boolean value in the "Setting" class function "SetValue".
    - Changed the "DecimalToHex" function in "Colors.cpp" to use streams instead of strings.

    Changes in v1.2.5:
    - Added an option "bLogToConsole" argument for the "ConsoleCommand" function in "Manager.hpp".
    - Added logging the old vs new value when changing a settings value for the "ConsoleCommand" function in "Manager.cpp".
    - Added a range feature for the "Setting" class, you can now specific a minimum and maximum range when setting values now. The new functions are "HasRange", "InRange", and "SetRange".
    - Added an example of how to use the new "SetRange" function in the "Initialize" function in "Manager.cpp".
    - Added a "Searchable" option for the "Command" class in "Manager.hpp", it has a default value of "true" in the classes constructor.
    - Added new functions, "GetId" and "GetDefaultValue", for the "Setting" class in "Manager.hpp".
    - Remade how binding and triggering setting/command callbacks work completely, and removed storing argument data in the "Command" class itself.
    - Changed/optimized some functions for the "Setting" class in "Manager.cpp".
    - Removed "CommandTypes" for the "Command" class in "Manager.hpp" as its no longer used.

    Changes in v1.2.4:
    - Made the static function/class maps in "Instances.hpp" private, to access their contents you should use the new "FindStaticClass" and "FindStaticFunction" functions in the same file.
    - Fixed "GameStateComponent" not having the new virtual "OnCreate" and "OnDestory" functions.
    - Changed the hook functions in "Events.cpp" to use the new "FindStaticFunction" functions from "Instances.hpp".
    - Changed grabbing the actor in the "UnrealCommand" to grab its default instance, instead of a random one in "Manager.cpp".
    - Changed the pointer check for setting instances to the "Instances.cpp" file, and modified "Events.cpp" to show this.
    - BREAKING: Added/renamed functions in the "GameState.hpp/cpp" files and made them more organized.

    Changes in v1.2.3:
    - Added an argument callback option for the "Setting" class in "Manager.hpp", works similar to the "Command" classes argument callback but the argument is the string value of the setting.
    - Added an optional argument for the "UnrealCommand" function in "Manager.hpp" called "bLogToConsole".
    - Fixed the "Lerp" function for the "VectorI" class taking a float instead of an integer in "Math.hpp".
    - Fixed the operators for the "Rotator" class in "Manager.cpp" saying struct instead of class.
    - Removed the "ShouldCallback" bool for the "Setting" class in "Manager.hpp", changed the check to see if the functions are null or not before calling instead of the bool value.
    - Reformatted the constructors in the "Colors.cpp", "Math.cpp", "Module.cpp", "Component.cpp", files.
    - Removed the "KeyPressed" function from "Manager.hpp", this should be handled by yourself in your own component or module.

    Changes in v1.2.2:
    - Changed function hooks in "Events.hpp" to use std::function, which also adds proper lambda support.
    - Fixed the "Detoured" bool prematurely getting set to true in "Events.cpp".

    Changes in v1.2.1:
    - Improved memory functions in "UnrealMemory.hpp", just look at them for yourself or something idk.
    - Improved the constructors for the "Setting" and "Command" classes in "Manager.cpp".
    - Added the virtual functions "OnCreate" and "OnDestroy" in "Component.hpp", these can be manually called outside of the classes constructor and deconstructor.
    - Added a "CreatedInstances" vector in "Instances.hpp", when a custom object instance is created it is added to this vector. On the components "OnDestory" function all custom instances are marked to be destroyed as well.
    - Added the "CreateVariable" function in "Manager.hpp" and moved creating variables from :ManagerComponent"'s constructor to the "Initialize" function in "Manager.cpp".
    - Added the "AttachDetour" and "DetachDetour" functions in "Events.hpp", this is now called from "InitializeGlobals" in "Core.cpp".
    - Moved the detaching Process Event from "CoreComponent"'s deconstructor to "EventsComponent"'s deconstuctor in "Events.cpp".
    - Made "WriteToLog" default to "true" in "Console.cpp".
    - BREAKING: "FColorList" to "GColorList" in "Colors.hpp".

    Changes in v1.2.0:
    - Added an extra null check for creating commands, settings, and modules in "Manager.cpp".
    - Changed/added constuctors for the "Color" and "LinearColor" classes in "Math.hpp/cpp" to be explicit, explicit because there are now auto conversions for different color types.
    - Remade the "Colors" namespace and added a ton of extra functions in "Math.hpp/cpp".
    - Reformatting/added extra comments in "Instances.hpp/cpp", and updated the PlaceholderSDK to accommodate this.
    - Changed all macros in "Math.hpp" to use static constant expressions.
    - Added includes for "Extensions/Includes.hpp" for both the "Component.hpp" and "Module.hpp" files.
    - Added null check for detaching detours in "Core.cpp".

    Changes in v1.1.9:
    - Removed the pointless additional constructors for the console component in "Console.hpp".
    - Improved functions for the console component such as writing and timestamp formatting in "Console.hpp".
    - Changed the null check for modules in the "Initialize" function to use ternaries in "Manager.cpp".
    - Improved/added extra comments in classes around the project.

    Changes in v1.1.8:
    - Added the "MarkInvincible" function in "Manager.hpp" which lets you easily prevent objects from being destroyed.
    - Added safety checks for the "invincible" functions in "Manager.cpp".
    - Added a null check for updating modules settings when the "Initialize" function is called in "Manager.cpp".
    - Added virtual deconstructors for components and modules.
    - Reformatted some of the constructors for the color classes in "Colors.cpp".

    Changes in v1.1.7:
    - Added the "MarkForDestroy" function in "Instances.hpp", and updated the PlaceholderSDK to accommodate this.
    - Fixed the "GetVector3DValue" function for the "Setting" class in "Manager.cpp", it still passed the type check even if it were a "TYPE_VECTOR_2D".
    - Changed some cast types for the template functions in "Instances.hpp".
    - BREAKING: some of the example settings in "Manager.cpp".

    Changes in v1.1.6:
    - Changed how settings and commands names are stored, in their classes they are now identified by an enum value; their names are assigned in the constructor for the "ManagerComponent" in "Manager.cpp".
    - Changed how settings and commands are created to go along with the new enum identifier, as well as how their shared pointers are now retrieved.
    - Changed how the "UpdateSettings" function works in "Module.hpp", there you can retrieve setting values all at once from "ManagerComponent" when a setting updates.
    - Added examples for how the "ResetSetting", "PrintModule", and "UnrealCommand" functions from "ManagerComponent" can be used as actual commands.
    - Remade all the math classes to use templated base classes in "Math.hpp", along with changing their names to pair with their underlying types.
    - Added a "Recalculate" function for the "LinearColor" class in "Colors.hpp", this is auto called in one of the constructors to recalculate the values if the user inputs an incorrect color space. For example they mistakenly put in a value from 0-255 instead of 0f-1f.
    - Added a few constant variables to operators in both "Colors.hpp" and "Math.hpp".

    Changes in v1.1.5:
    - Changed all function hooks to use the new "PreEvent" and "PostEvent" classes, they have neat template functions that auto recast for you as well as an option to not detour the function through Process Event.
    - Removed the "ParseArguments" and "SplitArguments" functions in "Manager.hpp" and instead moved it to the "Format" namespace in "Formatting.hpp".
    - Moved the function hook typedefs in "Events.hpp" to inside the "EventsComponent" class.
    - BREAKING: the "Functions" namespace to "Hooks" in "Events.hpp".
    - BREAKING: the "Queue" vector in "Manager.hpp" to "CommandQueue".

    Changes in v1.1.4:
    - Added the "CreateInstance" function in "Instances.hpp", this allows you to create new objects of any type and have it be added to the GObjects table.
    - Added the "ResetSetting" function to "Manager.hpp", resets a setting to its original default value.
    - Added the "PrintModule" function to "Manager.hpp", prints out a modules information to the console.
    - Remade how Modules, Settings, and Commands are created. Please see the changes in "Manager.hpp" and "Manager.cpp" for examples.
    - Added template flags to the PlaceholderSDK.
    - BREAKING: Renamed the unreal rotator function in "Math.hpp" to RotateUnreal.
    - BREAKING: Renamed some of the "States" enum names in "GameState.hpp" to include "STATE_" in front, because we are purposefully not making this enum an enum class.

    Changes in v1.1.3:
    - Renamed the project from CodeRedUniversal to CodeRedTemplate.
    - Added support for Vector and Vector2D settings to "Manager.hpp".
    - Added some unreal unit/cm constants in "Math.cpp" and reformatted some comments there.

    Changes in v1.1.2:
    - Added a "GetAllowedStates" function for Modules.
    - Added the "ColorToLinear" and "LinearToColor" converter functions to the "Colors" namespace.
    - Added a std::find check for blacklisting functions in "Events.cpp".
    - Fixed one of the constructors for the LinearColor class in "Colors.hpp".
    - Changed all the FColor structs in FColorList and FRainbowColor to use all the new Color classes in "Colors.hpp".
    - Removed the "ModuleTypes" enum and field in the Module constructor, because why would you really need it in the first place.

    Changes in v1.1.1:
    - Added a "Quat" class in "Math.cpp", along with a bunch of defines/math helpers.
    - Added "Get" functions to the FRainbowColor class, this copies the data but guarantees you can't modify the original structs by mistake.
    - Updated the placeholder sdk with the new math structs so the project can compile.
    - BREAKING: Renamed all "CR" math classes to just normal structs in "Math.hpp", also added extra math functions and operators for already existing classes.
    - BREAKING: Renamed all "CR" color classes to just normal structs in "Colors.hpp", also added some extra operators for them.

    Changes in v1.1.0:
    - Added a "WhitelistEvent" function in "Events.hpp".
    - Added the maps "StaticClasses" and "StaticFunctions" in "Instances.hpp", here all of these objects are mapped by their full name to their pointer.
    - Added extra safety checks and warnings for hooking functions.
    - Changed how hooking functions, and binding them work. Now you can hook multiple voids to a singular function, before it would be limited to only one.
    - Removed the toggle for logging functions in the EventsComponent, that was messy anyway.

    Changes in v1.0.9:
    - Added the Formatting extension files, includes helpful stream utils.
    - Made the console logging in "Core.cpp" use the new formatting extensions.
    - Made the console component to use std::ofstream and be more efficient in terms of initializing.
    - Improved the "UnrealMemory" files which are based on UE4 source.
    - Removed the "Filesystem" extension files as they have no real use (yet).

    Changes in v1.0.8:
    - Fixed the constructor for the "Setting" class not properly setting color values.
    - Fixed the "HexToDecimal" function in "Colors.hpp".
    - Updated the commenting in "Instances.hpp".

    Changes in v1.0.7:
    - Added constructors in the CRColor structs for their retrospective unreal engine structs.
    - Added the "Colors" namespace in "Colors.hpp" with some small color utils.
    - Made the Command and Setting vector a map in "Manager.hpp", using their Command/Setting names as the key.
    - Completely remade settings and commands, and changed the "Manager" module to match this, along with examples.
    - Added "CreateCommand" and "CreateSetting" functions in "Manager.hpp".
    - Remade how "GetCommand" and "GetSetting" work in "Manager.hpp".
    - Removed the "ModifySetting" function in "Manager.hpp" as it's no longer needed.
    - When an argument isn't provided for a setting, it now displays its current value to the console.

    Changes in v1.0.6:
    - Added a command queue, for executing Process Event functions outside of the games main thread (like in ImGui render scenes).
    - Added "Math.hpp" & "Math.cpp" which include custom vector and rotator structs, along with some helper functions.
    - Changed how the unreal color for the structs in "Colors.hpp" is calculated.
    - Added some structs to the PlaceholderSDK to match the new operators in "Math.hpp".

    Changes in v1.0.5:
    - Added some extra operators for the custom color structs in "Colors.hpp".
    - Added example of where to put the FRainbowColor hook.
    - Added examples on how to bind commands to module functions, as well as toggling bools.
    - Added an example on how to blacklist events from going through process event in Initialize function.
    - Added function to split an argument string by spaces (getting each word from a string and inserting into a vector).
    - Added hex color phrasing to the ModifySetting function.
    - Added option for the bool setting to toggle true/false with 1/0.
    - Moved the null check for functions in process event.
    - Fixed the constructor for StringSetting.
    - Removed the "Enabled" bool in the module base class, you should be adding this in your custom module class itself.

    Changes in v1.0.4:
    - Added an option to blacklist events from going through Process Event.
    - Added a global rainbow color class in "Colors.hpp", and remade "FColorList".
    - Remade how function hooks are found in "EventsComponent", it now maps out all functions internal index and their name.
    - Made console logging more efficient if you were using multiple functions that used different colors.
    - Made the "GetTimestamp" function static.

    Changes in v1.0.3:
    - Changed typedef for post function hooks.
    - Made the States enum a normal enum instead of an enum class to reduce the need to cast to its underlying type.
    - Added an "Initialized" private member for modules.
    - Made modules initialize first, then function hooks/instances.
*/