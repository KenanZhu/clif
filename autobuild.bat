@echo off
setlocal enabledelayedexpansion

:: Directories.
set "ROOT_DIR=%~dp0"
set "SRC_DIR=%ROOT_DIR%src"
set "BUILD_DIR=%ROOT_DIR%build\cmake"
set "BIN_DIR=%BUILD_DIR%\bin\CLIF_example"

:: Create build directory if needed.
if not exist "%BUILD_DIR%" (
    echo Creating build directory: "%BUILD_DIR%"
    mkdir "%BUILD_DIR%"
)

:: Check CMake existence.
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: CMake not found in PATH.
    echo Please install CMake 3.10+ from: https://cmake.org/download/
    pause
    exit /b 1
)

:: Check CMake version.
for /f "tokens=3" %%a in ('cmake --version 2^>^&1 ^| findstr /r /c:"[0-9][0-9]*\.[0-9][0-9]*"') do (
    set "version=%%a"
)
for /f "tokens=1-3 delims=." %%a in ("%version%") do (
    if %%a lss 3 (
        echo ERROR: Requires CMake 3.10+, found version %version%
        pause
        exit /b 1
    )
    if %%a equ 3 if %%b lss 10 (
        echo ERROR: Requires CMake 3.10+, found version %version%
        pause
        exit /b 1
    )
)

:: Build process.
cd /d "%BUILD_DIR%"
echo Generating build system...
cmake "%SRC_DIR%"
if %errorlevel% neq 0 (
    echo CMake generation failed!
    pause
    exit /b 1
)

echo Compiling project...
cmake --build .
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

:: Success message.
echo --------------------------------------------------
echo BUILD SUCCESSFUL
echo Executables located at: 
echo   "%BIN_DIR%"
echo --------------------------------------------------

:: Start PowerShell in target directory with error handling.
if exist "%BIN_DIR%" (
    echo Starting PowerShell in target directory...
    start "" powershell -NoExit -Command "Set-Location -LiteralPath '%BIN_DIR:\=\\%'; Write-Host 'You are now in the build directory' -ForegroundColor Green"
) else (
    echo WARNING: Build directory not found at:
    echo   "%BIN_DIR%"
    echo Launching PowerShell in build folder instead
    start "" powershell -NoExit -Command "Write-Host 'Build directory not found at expected location' -ForegroundColor Yellow"
)

:: Keep CMD window open after PowerShell starts.
echo --------------------------------------------------
echo Batch script completed. PowerShell window is open.
echo This window will remain active until you close it.
echo --------------------------------------------------
pause

endlocal