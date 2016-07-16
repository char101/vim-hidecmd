if exists('g:loaded_hidecmd')
	finish
endif
let g:loaded_hidecmd = 1

let s:dll = expand('<sfile>:p:h') . '/hidecmd-loader-x' . (has('win64') ? '64' : '86')
call libcall(s:dll, 'load', getpid())
