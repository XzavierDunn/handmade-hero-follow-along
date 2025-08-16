@echo off

@REM Set up CL
@REM https://learn.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170#create-your-own-command-prompt-shortcut
call "A:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

@REM Jump to project
A:
cd A:\workspace\personal\handmade-hero
