@echo off
set BUILD_TYPE=Release
set BUILD_DIR=build
set PLATFORM=Win32

for %%x in (%*) do (
    if "%%x" == "debug" (
        set BUILD_TYPE=Debug
    ) else if "%%x" == "release" (
        set BUILD_TYPE=Release
    ) else if "%%x" == "win32" (
        set PLATFORM=Win32
    ) else if "%%x" == "x64" (
        set PLATFORM=x64
    ) else (
        goto :usage
    )
)

if "%VisualStudioVersion%" == "" (
    echo "Error: cannot detect visual studio version, please run this file in \"Visual Studio Command Prompt\"."
	exit /B 1
)

set TOOLSET="v%VisualStudioVersion:.=%_xp"
echo "TOOLSET: %TOOLSET%"

echo "BUILD_TYPE: %BUILD_TYPE%"
echo "remove build directory"
del /s /q %BUILD_DIR%
mkdir %BUILD_DIR%

pushd %BUILD_DIR%
if "%PLATFORM%" == "Win32" (
   cmake -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DPLATFORM_TYPE=%PLATFORM% ..
   msbuild net_io.sln /p:Configuration=%BUILD_TYPE% /p:Platform=Win32 /p:MultiProcessCompilation=true /p:PlatformToolset=%TOOLSET%
) else (
    cmake -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCPU_COUNT=%CPU_COUNT% -DPLATFORM_TYPE=%PLATFORM% -G "Visual Studio 14 2015 Win64" ..
    msbuild net_io.sln /p:Configuration=%BUILD_TYPE% /p:Platform=x64 /p:MultiProcessorCompilation=true /p:PlatformToolset=%TOOLSET%
)

popd

exit /B 0

:usage
    echo "Usage: %0 [ debug | release ] [ win32 | x64 ]" >&2
    echo "Default: release win32" >&2
    exit /B 1
