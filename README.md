# vim-hidecmd
Hide console window opened by vim

# How does it work?
1. The plugin loads the loader dll
2. The loader dll executes [LoadLibrary](https://msdn.microsoft.com/en-us/library/windows/desktop/ms684175(v=vs.85).aspx)
   in the Vim process using [CreateRemoteThread](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682437(v=vs.85).aspx)
3. When loaded, the _detour_ dll hooks the [CreateProcess](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682425(v=vs.85).aspx)
   function and sets the value of `wShowWindow` to `SW_HIDE`. The API hooking is
   implemented using [MinHook](http://www.codeproject.com/Articles/44326/MinHook-The-Minimalistic-x86-x64-API-Hooking-Libra).
