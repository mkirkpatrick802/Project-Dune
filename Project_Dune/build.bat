@echo off

echo Deleting Binaries...
rmdir /s /q .vs
rmdir /s /q Binaries
rmdir /s /q Intermediate
rmdir /s /q Saved

REM These are diffrent for every computer, so they need to be changed accordingly
set MyUVS="C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"
set MyUBT="C:\Program Files\Epic Games\UE_5.2\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"

set MyFullPath="%cd%\Project_Dune"

echo Regenerating Visual Studio project files...
%MyUVS% /projectfiles %MyFullPath%.uproject
%MyUBT% Development Win64 -Project=%MyFullPath%.uproject -TargetType=Editor -Progress -NoEngineChanges -NoHotReloadFromIDE

echo Opening Unreal Project...
%MyFullPath%.uproject

exit /b