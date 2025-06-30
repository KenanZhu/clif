::
:: Batch script to automatically build CLIF example on Windows.
::
@echo off
setlocal enabledelayedexpansion

:: Directories.
::------------------------------------------------------------------------------
set "ROOT_DIR=%~dp0"
set "SRC_DIR=%ROOT_DIR%src"
set "BUILD_DIR=%ROOT_DIR%build\cmake"
set "BIN_DIR=%BUILD_DIR%\bin\CLIF_example"

:: Create default build directory.
echo autobuild : Checking default build directory...
::------------------------------------------------------------------------------

if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%" || (
        echo autobuild : Error * Failed to create default build directory: "%BUILD_DIR% ."
        pause
        exit /b 1
    )
)

:: Check CMake environment.
echo autobuild : Checking CMake environment...
::------------------------------------------------------------------------------

:: Check CMake existence.
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo autobuild : Error * CMake not found in PATH.
    echo autobuild : Please install CMake 3.12+ from:
    echo           - https://cmake.org/download/
    pause
    exit /b 1
)

:: Check CMake version.
for /f "tokens=3" %%a in ('cmake --version 2^>^&1 ^| findstr /r /c:"[0-9][0-9]*\.[0-9][0-9]*"') do (
    set "version=%%a"
)
for /f "tokens=1-3 delims=." %%a in ("%version%") do (
    if %%a lss 3 (
        echo ERROR: Requires CMake 3.12+, found version %version% .
        pause
        exit /b 1
    )
    if %%a equ 3 if %%b lss 12 (
        echo ERROR: Requires CMake 3.12+, found version %version% .
        pause
        exit /b 1
    )
)

:: Auto build process.
echo autobuild : All required tools found. Starting auto build process...
::-----------------------------------------------------------------------------

echo autobuild : Generating build system...
cd /d "%BUILD_DIR%"
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug "%SRC_DIR%"
if %errorlevel% neq 0 (
    echo autobuild : Error * CMake generation failed !
    pause
    exit /b 1
)
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo autobuild : Error * Build failed !
    pause
    exit /b 1
)

:: Auto build success message.
echo autobuild :
echo     BUILD SUCCESSFUL !
echo     Executables located at:
echo         "%BIN_DIR%"

:: Start PowerShell in target directory
echo autobuild : Starting new PowerShell instance in target directory...
::-----------------------------------------------------------------------------

if exist "%BIN_DIR%" (
    echo autobuild : Starting PowerShell in target directory...
    start "" powershell -NoExit -Command "Set-Location -LiteralPath '%BIN_DIR:\=\\%'; Write-Host 'You are now in the build directory' -ForegroundColor Green"
) else (
    echo autobuild : Warning * Build directory not found at:
    echo   "%BIN_DIR%"
    echo Launching PowerShell in build folder instead.
    start "" powershell -NoExit -Command "Write-Host 'Build directory not found at expected location' -ForegroundColor Yellow"
)

:: End of script.
echo autobuild : Auto build process completed.
::------------------------------------------------------------------------------

echo autobuild :
echo     Batch script completed. PowerShell window is open.
echo     This window will remain active until you close it.
pause

endlocal