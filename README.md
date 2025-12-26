# CodeRed Template v1.6.5

## 📌 About The Project

The CodeRed Template is a custom framework to get you started in Unreal Engine modding, designed to be used in any game, it features a lot of easy to use functions common in game modding. Such as function hooking, a module manager, storing class instances, saving and loading user settings, and more.

### 📋 Features

Included are the following features, each have their own examples in their own files retrospectively; along with comments on how to use them and how they work.

- Multicolored debug console, along with writing to a text file for offline analyzing.
- Automatic initialization of globals, along with detouring of the Process Event function.
- Pre and post function hooking, bind your own functions that fire when an Unreal Engine function fires even from multiple sources (ProcessEvent, ProcessInternal, etc.).
- Instance storage, static and dynamically store needed classes that can be grabbed at any time.
- Command, setting, and module manager. Bind strings to activate mods or change certain settings dynamically (For separate UI/Console interaction).
- Variable system, save and load settings changed by users such a booleans, integers, strings, and more. Supports  proper handling for multithreading, such as calling commands from a rendering thread like Dear ImGui.
- GameState tracker, track where the player is or what type of game mode they are in at all times.

## 🚀 Getting Started

To configure for your own game all that you have to do is replace `PlaceholderSDK` with your own SDK and make sure it's included in `pch.hpp`, the included `PlaceholderSDK` is just an example with basic classes so the project can compile without errors.

For instance storage, game state tracker, and function hooks, they are all game-specific; so you will need to add on to it depending on what classes/functions your game uses. Please read the comments throughout the project along with references to the `WALKTHROUGH` macro related to setting up hooks and file paths.

## 📦 Requirements

- Moderate understanding of how Unreal Engine works (Globals, objects, actors, reflection system, etc).
- Moderate to advanced understanding of modern C++ (Memory and allocation specifically).
- Visual Studio or another Windows based compiler (For Windows header files, along with library linking).
- ISO C++20 Standard.

## 🧰 Libraries

The following third party libraries are used for compiling, they are included in this project but can be found below.

This project supports both Detours and MinHook, you can pick which one to use with the `CR_DETOURS` and `CR_MINHOOK` macros in the `Types.hpp` file.

- Microsoft Detours https://github.com/microsoft/Detours/
- TsudaKageyu MinHook https://github.com/TsudaKageyu/minhook

## 🐛 Issues & Feedback

If you notice something wrong or if anything can be improved please create a new issue [here](https://github.com/CodeRedRL/CodeRed-Template/issues/), but do not submit a question to why something isn't working if you do not meet the requirements for this project listed in this README.

## 📄 License

This repository is licensed under the MIT License, feel free to do what you want but you are responsible for your own actions. Make sure the game you are using this project with allows third party modding.

## 📸 Screenshots

![](https://i.imgur.com/ofnaNVV.png)