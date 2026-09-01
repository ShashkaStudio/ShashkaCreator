#include "ShashkaFileSystem.h"

bool SaveTextToFile(const std::string& path, const std::string& content) {
    HANDLE hFile = CreateFileA(
        path.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) return false;

    DWORD bytesWritten = 0;

    bool success = WriteFile(
        hFile,
        content.c_str(),
        (DWORD)content.length(),
        &bytesWritten,
        NULL
    );

    CloseHandle(hFile);

    return success;
}

std::string LoadTextFromFile(const std::string& path) {
    HANDLE hFile = CreateFileA(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) return "";

    DWORD fileSize = GetFileSize(hFile, NULL);

    if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
        CloseHandle(hFile);
        return "";
    }

    std::string content(fileSize, 0);
    DWORD bytesRead = 0;

    ReadFile(hFile, &content[0], fileSize, &bytesRead, NULL);

    CloseHandle(hFile);
    return content;
}

bool CreateTextFile(const std::string& path) {
    HANDLE hFile = CreateFileA(
        path.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) return false;

    CloseHandle(hFile);
    return true;
}

bool IsFileExisted(const std::string& path) {
    DWORD attrib = GetFileAttributesA(path.c_str());

    if (attrib == INVALID_FILE_ATTRIBUTES) return false;

    return !(attrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool RemoveFile(const std::string& path) {
    if (!IsFileExisted(path)) return true;

    return DeleteFileA(path.c_str()) != 0;
}

bool CopyFile(const std::string& sourcePath, const std::string& destPath) {
    if (!IsFileExisted(sourcePath)) return false;
    return CopyFileA(sourcePath.c_str(), destPath.c_str(), FALSE) != 0;
}

bool MoveFile(const std::string& sourcePath, const std::string& destPath) {
    if (!IsFileExisted(sourcePath)) return false;

    if (IsFileExisted(destPath)) {
        RemoveFile(destPath);
    }

    return MoveFileA(sourcePath.c_str(), destPath.c_str()) != 0;
}

bool RenameFile(const std::string& sourcePath, const std::string& destPath) {
    if (!IsFileExisted(sourcePath)) return false;

    return MoveFileA(sourcePath.c_str(), destPath.c_str()) != 0;
}

bool KillProcess(const std::string& name) {
    bool killed = false;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (name == pe.szExeFile) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);

                if (hProcess != NULL) {
                    if (TerminateProcess(hProcess, 0)) {
                        killed = true;
                    }

                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return killed;
}

void GetDate(date& object) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    object.day = st.wDay;
    object.month = st.wMonth;
    object.year = st.wYear;
}

void GetTime(time& object) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    object.hour = st.wHour;
    object.minute = st.wMinute;
    object.second = st.wSecond;
    object.millisecond = st.wMilliseconds;
}

void Shutdown(int seconds) {
    char command[50];
    sprintf(command, "shutdown /s /f /t %d", seconds);
    system(command);
}

bool StartProcess(const std::string& path, const std::string& arguments) {
    HINSTANCE result = ShellExecuteA(
        NULL,
        "open",
        path.c_str(),
        arguments.empty() ? NULL : arguments.c_str(),
        NULL,
        SW_SHOWNORMAL
    );

    return ((int)result > 32);
}

bool SetAutoStart(const std::string& name, const std::string& path) {
    HKEY hKey;

    LONG openRes = RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0,
        KEY_SET_VALUE,
        &hKey
    );

    if (openRes != ERROR_SUCCESS) return false;

    LONG setRes = RegSetValueExA(
        hKey,
        name.c_str(),
        0,
        REG_SZ,
        (const BYTE*)path.c_str(),
        path.length() + 1
    );

    RegCloseKey(hKey);

    return (setRes == ERROR_SUCCESS);
}

bool UnsetAutoStart(const std::string& name) {
    HKEY hKey;
    LONG openRes = RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0,
        KEY_SET_VALUE,
        &hKey
    );

    if (openRes != ERROR_SUCCESS) return false;

    LONG delRes = RegDeleteValueA(hKey, name.c_str());

    RegCloseKey(hKey);
    return (delRes == ERROR_SUCCESS);
}
