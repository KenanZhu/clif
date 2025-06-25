#!/bin/bash

# Directories
ROOT_DIR=$(dirname "$(realpath "$0")")
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/build/cmake"
BIN_DIR="$BUILD_DIR/bin/CLIF_example/Debug"

# Create build directory if needed
mkdir -p "$BUILD_DIR" || {
    echo "Error: Failed to create build directory: $BUILD_DIR"
    exit 1
}

# Check CMake existence
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found in PATH."
    echo "Please install CMake 3.10+ with:"
    echo "  sudo apt install cmake    # Debian/Ubuntu"
    echo "  sudo yum install cmake    # RHEL/CentOS"
    exit 1
fi

# Check CMake version
CMAKE_VERSION=$(cmake --version | awk 'NR==1{print $3}')
MIN_VERSION="3.10"

if printf '%s\n' "$MIN_VERSION" "$CMAKE_VERSION" | sort -V -C; then
    :
else
    echo "ERROR: Requires CMake $MIN_VERSION+, found version $CMAKE_VERSION"
    exit 1
fi

# Build process
cd "$BUILD_DIR" || exit 1
echo "Generating build system..."
cmake "$SRC_DIR" || {
    echo "CMake generation failed!"
    exit 1
}

echo "Compiling project..."
cmake --build . || {
    echo "Build failed!"
    exit 1
}

# Success message
echo "--------------------------------------------------"
echo "BUILD SUCCESSFUL"
echo "Executables located at:"
echo "  \"$BIN_DIR\""
echo "--------------------------------------------------"

# Start terminal in target directory
if [ -d "$BIN_DIR" ]; then
    echo "Starting new terminal in target directory..."
    # Try different terminal emulators (fallback to default)
    if command -v x-terminal-emulator &> /dev/null; then
        x-terminal-emulator --working-directory="$BIN_DIR" &
    elif command -v gnome-terminal &> /dev/null; then
        gnome-terminal --working-directory="$BIN_DIR" &
    elif command -v konsole &> /dev/null; then
        konsole --workdir "$BIN_DIR" &
    elif command -v xterm &> /dev/null; then
        xterm -e "cd \"$BIN_DIR\"; bash" &
    else
        echo "WARNING: No terminal emulator found. Manually navigate to:"
        echo "  cd \"$BIN_DIR\""
    fi
else
    echo "WARNING: Build directory not found at:"
    echo "  \"$BIN_DIR\""
fi

echo "--------------------------------------------------"
echo "Batch script completed. PowerShell window is open."
echo "This window will remain active until you close it."
echo "--------------------------------------------------"