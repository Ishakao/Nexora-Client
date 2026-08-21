#pragma once

#include <string>

#if defined(_WIN32)
#ifdef API_LIB_EXPORT
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

    API const char* getLayout();
    API bool capsLock();
    API float GetMouseScreenPositionX();
    API float GetMouseScreenPositionY();
#ifdef _WIN32
    API const wchar_t* GetFile();
#elif defined(__linux__)
    API const char* GetFile();
#endif

#ifdef __cplusplus
}
#endif