if exists('g:loaded_hidecmd')
	finish
endif
let g:loaded_hidecmd = 1

if exists('g:hidecmd_exclude') && g:hidecmd_exclude != ''
	exec 'let $HIDECMD_EXCLUDE = "' . escape(g:hidecmd_exclude, '\"') . '"'
endif

if exists('g:hidecmd_include') && g:hidecmd_include != ''
	exec 'let $HIDECMD_INCLUDE = "' . escape(g:hidecmd_include, '\"') . '"'
endif

let s:dll = expand('<sfile>:p:h') . '/hidecmd-loader-x' . (has('win64') ? '64' : '86')
call libcall(s:dll, 'load', getpid())
