PLATFORM=x64
LIBS=

all: detour loader

detour: hidecmd-detour-$(PLATFORM).dll clean-intermediate

hidecmd-detour-$(PLATFORM).dll: hidecmd-detour-$(PLATFORM).obj
	link /nologo /dll /out:$@ /opt:ref /opt:icf /ltcg $** libMinHook-$(PLATFORM)-v140-mt.lib user32.lib

hidecmd-detour-$(PLATFORM).obj: detour.cpp
	cl /nologo /O2 /MT /GL /GS- /c /Fo:$@ $**

loader: hidecmd-loader-$(PLATFORM).dll clean-intermediate

hidecmd-loader-$(PLATFORM).dll: hidecmd-loader-$(PLATFORM).obj
	link /nologo /dll /out:$@ /def:loader.def /opt:ref /opt:icf /ltcg $** shlwapi.lib user32.lib

hidecmd-loader-$(PLATFORM).obj: loader.cpp
	cl /nologo /O2 /MT /GL /GS- /c /Fo:$@ $**

clean: clean-intermediate
	del *.dll

clean-intermediate:
	del *.obj *.exp hidecmd-*.lib
