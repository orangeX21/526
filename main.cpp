#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#pragma comment(lib, "winmm.lib")
#include <mmsystem. h>

using namespace std;

// 声明资源 ID
#define IDR_MYMP3 101

int main()
{
    // 查找资源
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_MYMP3), _T("MP3"));
    if (!hResource) {
        MessageBox(NULL, _T("Failed to find resource"), _T("Error"), MB_OK);
        return 1;
    }

    // 加载资源
    HGLOBAL hLoadedResource = LoadResource(NULL, hResource);
    if (!hLoadedResource) {
        MessageBox(NULL, _T("Failed to load resource"), _T("Error"), MB_OK);
        return 1;
    }

    // 锁定资源以获得指向资源数据的指针
    LPVOID pResourceData = LockResource(hLoadedResource);
    DWORD resourceSize = SizeofResource(NULL, hResource);

    if (!pResourceData) {
        MessageBox(NULL, _T("Failed to lock resource"), _T("Error"), MB_OK);
        return 1;
    }

    // 将资源数据保存到临时文件
    TCHAR tempPath[MAX_PATH];
    TCHAR tempFile[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    GetTempFileName(tempPath, _T("mp3"), 0, tempFile);

    HANDLE hFile = CreateFile(tempFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, _T("Failed to create temp file"), _T("Error"), MB_OK);
        return 1;
    }

    DWORD written;
    WriteFile(hFile, pResourceData, resourceSize, &written, NULL);
    CloseHandle(hFile);

    // 播放MP3文件
    mciSendString((L"open " + std::wstring(tempFile) + L" alias s1").c_str(), NULL, 0, NULL);
    mciSendString(L"play s1", NULL, 0, NULL);

    // 播放60秒
    Sleep(60000);

    mciSendString(L"close s1", NULL, 0, NULL);

    // 删除临时文件
    DeleteFile(tempFile);

    getchar();
    return 0;
}