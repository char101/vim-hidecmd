#include <Windows.h>
#include <Shlwapi.h>
#include <cstdio>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

// Source: http://resources.infosecinstitute.com/using-createremotethread-for-dll-injection-on-windows/
LONG load(LONG pid)
{
    char dllPath[MAX_PATH];
    GetModuleFileName((HINSTANCE) &__ImageBase, dllPath, sizeof(dllPath));
    PathRemoveFileSpec(dllPath);
#if _WIN64
    PathAppend(dllPath, "hidecmd-detour-x64.dll");
#else
    PathAppend(dllPath, "hidecmd-detour-x86.dll");
#endif

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (! process)
    {
        MessageBox(NULL, "OpenProcess failed", "load", 0);
        return 1;
    }

    LPVOID addr = (LPVOID) GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (! addr)
    {
        MessageBox(NULL, "GetProcAddress failed", "load", 0);
        return 1;
    }

    LPVOID arg = (LPVOID) VirtualAllocEx(process, NULL, strlen(dllPath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (! arg)
    {
        MessageBox(NULL, "VirtualAllocEx failed", "load", 0);
        return 1;
    }

    int n = WriteProcessMemory(process, arg, dllPath, strlen(dllPath), NULL);
    if (! n)
    {
        MessageBox(NULL, "WriteProcessMemory failed", "load", 0);
        return 1;
    }

    HANDLE threadId = CreateRemoteThread(process, NULL, 0,  (LPTHREAD_START_ROUTINE) addr, arg, NULL, NULL);
    if (! threadId)
    {
        MessageBox(NULL, "CreateRemoteThread failed", "load", 0);
        return 1;
    }

    CloseHandle(process);

    return 0;
}
