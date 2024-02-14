#pragma once

// // 包含 SDKDDKVer.h 可定义可用的最高版本的 Windows 平台。
// 如果希望为之前的 Windows 平台构建应用程序，在包含 SDKDDKVer.h 之前请先包含 WinSDKVer.h 并
// 将 _WIN32_WINNT 宏设置为想要支持的平台。
#include <WinSDKVer.h>

// 设置为 Windows 7 或更高版本
// ReSharper disable once CppInconsistentNaming
#define _WIN32_WINNT NTDDI_WIN7  // NOLINT(clang-diagnostic-reserved-macro-identifier)

#include <SDKDDKVer.h>
