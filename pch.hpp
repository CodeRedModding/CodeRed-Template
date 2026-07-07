#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <functional>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <mutex>
#include <thread>
#include <string>
#include <numeric>
#include <string_view>
#include <cmath>
#include <random>
#include <inttypes.h>
#include <filesystem>

// CodeRed Macros
#define WALKTHROUGH

#include "Types.hpp"
#include "PlaceHolderSDK/SdkHeaders.hpp"
#include "Framework/FunctionHook/FunctionHook.hpp"
#include "Framework/EventGuard/EventGuard.hpp"

#ifdef CR_DETOURS
#include <detours.h>
#endif
#ifdef CR_MINHOOK
#include "Framework/MinHook/MinHook.hpp"
#endif