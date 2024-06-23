## CodeRed Template v1.4.4

A bare-bones template to get your started in Unreal Engine modding that can be used in any game.

To configure for your own game all that you have to do is replace "PlaceholderSDK" with your own and make sure it's included in `pch.hpp`, the included "PlaceholderSDK" is just an example and has basic structs/classes so the project can compile. Please read the comments in the `Core.cpp` file regaurding Process Event as well.

For instance storage, game state tracker, and function hooks, they are all game-specific; so you will need to add on to it depending on what classes/functions your game uses. There are comments throughout the project explaining how everything works as well as some examples of certain features.

#### Requirements

- Moderate understanding of how Unreal Engine works (Globals, objects, actors, reflection system, etc).
- Moderate to advanced understanding of modern C++ (Memory and allocation specifically).
- Visual Studio or another Windows based compiler (For Windows header files, along with library linking).
- ISO C++20 Standard.

#### Libraries

The following third party libraries are used for compiling, they are included in this project but can be found below.

- Microsoft Detours https://github.com/microsoft/Detours/

## Features

Included are the following features, each have their own examples in their own files retrospectively; along with comments on how to use them and how they work.

- Multicolored debug console, along with writing to a text file for offline logging/analyzing.
- Automatic initialization of globals, along with detouring of the Process Event function.
- Pre and post function hooking, bind your own functions that fire when an Unreal Engine function fires.
- Instance storage, static and dynamically store needed classes that can be grabbed at any time.
- Command, setting, and mod manager. Bind strings to activate mods or change certain settings dynamically (For separate UI/Console interaction like ImGui).
- GameState, track where the player is/what type of game they are in at all times.

## Issues & Feedback

If you notice something wrong or if anything can be improved please create a new issue [here](https://github.com/CodeRedRL/CodeRed-Universal/issues/), but do not submit a question to why something isn't working if you do not meet the requirements for this project listed in this README.

## License

This repository is licensed under the MIT License, feel free to do what you want but you are responsible for your own actions. Make sure the game you are using this project with allows third party modding.

## Screenshots

![](https://i.imgur.com/ofnaNVV.png)