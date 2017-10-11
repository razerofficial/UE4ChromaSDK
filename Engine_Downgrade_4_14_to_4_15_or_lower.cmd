@ECHO OFF
CALL clean_intermediate_files.cmd
SET APP="UnrealEngineVersionChanger\bin\Debug\UnrealEngineVersionChanger.exe"
IF NOT EXIST %APP% ECHO UnrealEngineVersionChanger needs to be compiled!
IF EXIST %APP% CALL "UnrealEngineVersionChanger\bin\Debug\UnrealEngineVersionChanger.exe" 4.15
PAUSE
