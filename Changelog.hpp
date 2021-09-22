/*
    Changes in v1.1.6:
    - Changed how settings and commands names are stored, in their classes they are now identified by an enum value; their names are assigned in the constructor for the "ManagerComponent" in "Manager.cpp".
    - Changed how settings and commands are created to go along with the new enum identifier, as well as how their shared pointers are now retrieved.
    - Changed how the "UpdateSettings" function works in "Module.hpp", there you can retrieve setting values all at once from "ManagerComponent" when a setting updates.
    - Added examples for how the "ResetSetting", "PrintModule", and "UnrealCommand" functions from "ManagerComponent" can be used as actual commands.
    - Remade all the math classes to use templated base classes in "Math.hpp", along with changing their names to pair with their underlying types.
    - Added a "Recalculate" function for the "LinearColor" class in "Colors.hpp", this is auto called in one of the constructors to recalculate the values if the user inputs an incorrect color space. For example they mistakenly put in a value from 0-255 instead of 0f-1f.
    - Added a few const variables to operators in both "Colors.hpp" and "Math.hpp".

    Changes in v1.1.5:
    - Changed all function hooks to use the new "PreEvent" and "PostEvent" classes, they have neat template functions that auto recast for you as well as an option to not detour the function through Process Event.
    - Removed the "ParseArguments" and "SplitArguments" functions in "Manager.hpp" and instead moved it to the "Format" namespace in "Formatting.hpp".
    - Renamed the "Functions" namespace to "Hooks" in "Events.hpp".
    - Renamed the "Queue" vector in "Manager.hpp" to "CommandQueue".
    - Moved the function hook typedefs in "Events.hpp" to inside the "EventsComponent" class.

    Changes in v1.1.4:
    - Added the "CreateInstance" function in "Instances.hpp", this allows you to create new objects of any type and have it be added to the GObjects table.
    - Added the "ResetSetting" function to "Manager.hpp", resets a setting to its original default value.
    - Added the "PrintModule" function to "Manager.hpp", prints out a modules information to the console.
    - Remade how Modules, Settings, and Commands are created. Please see the changes in "Manager.hpp" and "Manager.cpp" for examples.
    - Renamed the unreal rotator function in "Math.hpp" to RotateUnreal.
    - Renamed some of the "States" enum names in "GameState.hpp" to include "STATE_" in front, because we are purposefully not making this enum an enum class.
    - Added template flags to the PlaceholderSDK.

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
    - Renamed all "CR" math classes to just normal structs in "Math.hpp", also added extra math functions and operators for already exsiting classes.
    - Renamed all "CR" color classes to just normal structs in "Colors.hpp", also added some extra operators for them.
    - Updated the placeholder sdk with the new math structs so the project can compile.

     Changes in v1.1.0:
     - Added a "WhitelistEvent" function in "Events.hpp".
     - Added the maps "StaticClasses" and "StaticFunctions" in "Instances.hpp", here all of these objects are mapped by their full name to their pointer.
     - Added extra saftey checks and warnings for hooking functions.
     - Changed how hooking functions, and binding them work. Now you can hook multiple voids to a singluar function, before it would be limited to only one.
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
    - Removed the "Enabled" bool in the module base class, you should be adding this in your custo module class itself.

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