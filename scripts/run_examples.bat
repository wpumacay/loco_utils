@echo off
:: Make sure we are given the build config
if [%1] == [] (goto :proc_error) else (goto :proc_run)
:proc_error
echo Usage: .\run_examples.bat [config=Debug,Release,RelWithDebInfo,MinSizeRel]
exit /b 1
:proc_run
echo Running examples for configuration [%1]
setlocal enabledelayedexpansion
:: List of paths to the examples we have so far
set examples=logger_console
set examples=%examples%;logger_file
set examples=%examples%;path_handling
set examples=%examples%;perlin_noise
set examples=%examples%;profiler_external_chrome
set examples=%examples%;smart_pointers_macro
set examples=%examples%;timing
set root_folder=%~dp0..
set binary_folder=%root_folder%\build\bin\%1
for %%x in (%examples%) do (
    echo Running example "%binary_folder%\example_%%x.exe"
    %binary_folder%\example_%%x
    echo Done! ------------------------
)
exit /b 0
