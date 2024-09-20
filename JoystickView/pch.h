#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include "bakkesmod/plugin/bakkesmodplugin.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "imgui/imgui.h"

#include "fmt/core.h"
#include "fmt/ranges.h"

extern std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

template <typename... Args>
void LOG(std::string_view format_str, Args&&... args)
{
	_globalCvarManager->log(std::vformat(format_str, std::make_format_args(args...)));
}

template <typename... Args>
void LOG(std::wstring_view format_str, Args&&... args)
{
	_globalCvarManager->log(std::vformat(format_str, std::make_wformat_args(args...)));
}