/*
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