@ECHO OFF
REM Visual Studio 2015 must be installed
CALL clean_intermediate_files.cmd
SET APP="UnrealEngineVersionChanger\bin\Debug\UnrealEngineVersionChanger.exe"
IF NOT EXIST %APP% ECHO UnrealEngineVersionChanger needs to be compiled!
IF EXIST %APP% CALL "UnrealEngineVersionChanger\bin\Debug\UnrealEngineVersionChanger.exe" 4.12
COPY /Y UE4ChromaSDK_4_12.uproject UE4ChromaSDK.uproject
CALL SwitchEngineVersion.cmd
START "" OpenPluginBinaries.cmd
"C:\Program Files\7-Zip\7zFM.exe" "%CD%\ChromaSDKPlugin_4_12.zip\ChromaSDKPlugin\Binaries\Win64\"
