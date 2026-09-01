#ifndef SHASHKAFILESYSTEM_H
#define SHASHKAFILESYSTEM_H

#include "ShashkaCreator.h"
#include <tlhelp32.h>
#include <shellapi.h>

#ifndef SHTDN_REASON_MAJOR_OTHER
    #define SHTDN_REASON_MAJOR_OTHER 0x00000000
#endif

struct date {
    int day, month, year;
};

struct time {
    int hour, minute, second, millisecond;
};

bool SaveTextToFile(const std::string& path, const std::string& content);

std::string LoadTextFromFile(const std::string& path);

bool CreateTextFile(const std::string& path);

bool IsFileExisted(const std::string& path);

bool RemoveFile(const std::string& path);

bool CopyFile(const std::string& sourcePath, const std::string& destPath);

bool MoveFile(const std::string& sourcePath, const std::string& destPath);

bool RenameFile(const std::string& sourcePath, const std::string& destPath);

bool KillProcess(const std::string& name);

void GetDate(date& object);

void GetTime(time& object);

void Shutdown(int seconds);

bool StartProcess(const std::string& path, const std::string& arguments);

bool SetAutoStart(const std::string& name, const std::string& path);

bool UnsetAutoStart(const std::string& name);

#endif // SHASHKAFILESYSTEM_H
