# vim-hidecmd
Hide console window opened by Vim.

# How does it work?
1. The plugin loads the loader dll
2. The loader dll executes [LoadLibrary](https://msdn.microsoft.com/en-us/library/windows/desktop/ms684175(v=vs.85).aspx)
   in the Vim process using [CreateRemoteThread](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682437(v=vs.85).aspx)
3. When loaded, the detour dll hooks the [CreateProcess](https://msdn.microsoft.com/en-us/library/windows/desktop/ms682425(v=vs.85).aspx)
   function and sets the value of `wShowWindow` to `SW_HIDE`. The API hooking is
   implemented using [MinHook](http://www.codeproject.com/Articles/44326/MinHook-The-Minimalistic-x86-x64-API-Hooking-Libra).

# Options

## `g:hidecmd_exclude`

Don't hide command line that matches `g:hidecmd_exclude`.

Example:  
Don't hide command window that runs `dir` e.g. when running `!dir`
`let g:hidecmd_exclude = 'dir'`

## `g:hidecmd_include`

Only hide command line that matches `g:hidecmd_include`

Example:  
Only hide command window that runs ctags.exe:  
`let g:hidecmd_include = 'ctags'`

`g:hidecmd_include` has precedence over `g:hidecmd_exclude`. The value of each option is a regex. The regex format are based on the C++ `regex` library (MSVC 2015).
