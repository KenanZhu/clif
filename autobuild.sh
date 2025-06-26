#
# Shell script to automatically build CLIF example on Linux.
#

# Directories
#------------------------------------------------------------------------------
ROOT_DIR=$(dirname "$(realpath "$0")")
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/build/cmake"
BIN_DIR="$BUILD_DIR/bin/CLIF_example"

# Check default build directory.
echo "autobuild : Checking default build directory..."
#------------------------------------------------------------------------------

mkdir -p "$BUILD_DIR" || {
    echo "autobuild : Error * Failed to create default build directory: $BUILD_DIR ."
    exit 1
}

# Check CMake environment.
echo "autobuild : Checking CMake environment..."
#------------------------------------------------------------------------------

# Check CMake existence.
if ! command -v cmake &> /dev/null; then
    echo "autobuild : Error * CMake not found in PATH."
    echo "autobuild : Please install CMake 3.10+ with:"
    echo "          - sudo apt install cmake    # Debian/Ubuntu"
    echo "          - sudo yum install cmake    # RHEL/CentOS"
    exit 1
fi

# Check CMake version.
CURRENT_CMAKE_VERSION=$(cmake --version | awk 'NR==1{print $3}')
REQUIRE_CMAKE_VERSION="3.12"

if printf '%s\n' "$REQUIRE_CMAKE_VERSION" "$CURRENT_CMAKE_VERSION" | sort -V -C; then
    :
else
    echo "autobuild : Error * Requires CMake $REQUIRE_CMAKE_VERSION+, found version $CURRENT_CMAKE_VERSION ."
    exit 1
fi

# Auto build process.
echo "autobuild : All required tools found. Starting auto build process..."
#------------------------------------------------------------------------------

# Auto generate build system.
echo "autobuild : Generating build system..."
cd "$BUILD_DIR" || {
    echo "autobuild : Error * Failed to access directory: $BUILD_DIR"
    exit 1
}
cmake "$SRC_DIR" || {
    echo "autobuild : CMake generation failed !"
    exit 1
}
cmake --build . || {
    echo "autobuild : Error * Build failed !"
    exit 1
}

# Auto build success message.
echo "autobuild :"
echo "    AUTO BUILD SUCCESSFUL !"
echo "    Executables located at:"
echo "        \"$BIN_DIR\""
echo ""
#------------------------------------------------------------------------------

# Start terminal in target directory.
echo "autobuild : Starting new terminal in target directory..."
#------------------------------------------------------------------------------

if [ -d "$BIN_DIR" ]; then
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
        echo "autobuild : Warning * No terminal emulator found. Manually navigate to:"
        echo "  cd \"$BIN_DIR\""
    fi
else
    echo "autobuild : Warning * Build directory not found at:"
    echo "    \"$BIN_DIR\""
fi

# End of script.
echo "autobuild : Auto build process completed."
#------------------------------------------------------------------------------

echo "autobuild :"
echo "    Batch script completed. Terminal window is open."
echo "    This window will remain active until you close it."
echo ""
