@echo off

if not defined DevEnvDir (
	call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
)

rem debug: /Zi (== -g)

cl main.cpp ./core/settings.cpp ./core/request.cpp ./core/http_server.cpp ./core/file_cache.cpp ./core/application.cpp ./rdn_application.cpp ./core/theme.cpp ^
	/Febin/game-vc.exe ^
	/EHsc /std:c++17 ^
	/Ilibs ^
	/link WSock32.lib Ws2_32.lib 

