#include <Windows.h>
#include "MinHook.h"
#include <regex>

template <typename T>
inline MH_STATUS MH_CreateHookApiEx(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, T** ppOriginal)
{
    return MH_CreateHookApi(pszModule, pszProcName, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

typedef BOOL (WINAPI *CREATEPROCESS)(LPCTSTR,
                                     LPTSTR,
                                     LPSECURITY_ATTRIBUTES,
                                     LPSECURITY_ATTRIBUTES,
                                     BOOL,
                                     DWORD,
                                     LPVOID,
                                     LPCTSTR,
                                     LPSTARTUPINFO,
                                     LPPROCESS_INFORMATION);

CREATEPROCESS fpCreateProcess = NULL;

BOOL WINAPI DetourCreateProcess(LPCTSTR lpApplicationName,
                                LPTSTR lpCommandLine,
                                LPSECURITY_ATTRIBUTES lpProcessAttributes,
                                LPSECURITY_ATTRIBUTES lpThreadAttributes,
                                BOOL bInheritHandles,
                                DWORD dwCreationFlags,
                                LPVOID lpEnvironment,
                                LPCTSTR lpCurrentDirectory,
                                LPSTARTUPINFO lpStartupInfo,
                                LPPROCESS_INFORMATION lpProcessInformation)
{
    TCHAR buffer[32767];
    bool hide = true;

    if (GetEnvironmentVariable("HIDECMD_EXCLUDE", buffer, sizeof(buffer)) > 0)
    {
        hide = ! std::regex_search(lpCommandLine, std::regex(buffer));
    }

    if (GetEnvironmentVariable("HIDECMD_INCLUDE", buffer, sizeof(buffer)) > 0)
    {
        hide = std::regex_search(lpCommandLine, std::regex(buffer));
    }

    if (hide)
    {
        lpStartupInfo->wShowWindow = SW_HIDE;
    }

    return fpCreateProcess(lpApplicationName,
                           lpCommandLine,
                           lpProcessAttributes,
                           lpThreadAttributes,
                           bInheritHandles,
                           dwCreationFlags,
                           lpEnvironment,
                           lpCurrentDirectory,
                           lpStartupInfo,
                           lpProcessInformation);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // MessageBox(NULL, "Detour DLL attached", "DllMain", 0);
            if (MH_Initialize() != MH_OK)
            {
                MessageBox(NULL, "MH_Initialize failed", "DllMain", 0);
                return FALSE;
            }
            if (MH_CreateHookApiEx(L"kernel32", "CreateProcessA", &DetourCreateProcess, &fpCreateProcess) != MH_OK)
            {
                MessageBox(NULL, "MH_CreateHook failed", "DllMain", 0);
                return FALSE;
            }
            if (MH_EnableHook(&CreateProcessA) != MH_OK)
            {
                MessageBox(NULL, "MH_EnableHook failed", "DllMain", 0);
                return FALSE;
            }
            // MessageBox(NULL, "Hook created", "DllMain", 0);
            break;
        case DLL_PROCESS_DETACH:
            if (MH_DisableHook(&CreateProcessA) != MH_OK)
            {
                MessageBox(NULL, "MH_DisableHook failed", "DllMain", 0);
                return FALSE;
            }
            if (MH_Uninitialize() != MH_OK)
            {
                MessageBox(NULL, "MH_Uninitialize failed", "DllMain", 0);
                return FALSE;
            }
            // MessageBox(NULL, "Detour DLL detached", "DllMain", 0);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }

    return TRUE;
}
