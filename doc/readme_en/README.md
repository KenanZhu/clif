English | [简体中文](../../README.md)

# CLIF: Functional Command-Line-Interface Framework Based on C/C++

---
## Why Choose CLIF?

✅ **Single File Dependency**
 : Requires only the C++17 standard library with no external dependencies

✅ **Functional Programming**
 : Direct function binding eliminates manual argument parsing

✅ **Strong Type Safety**
 : Catches type conflicts during compilation to prevent runtime errors

✅ **Generic Container Support**
 : Native support for nested containers with custom parsers and validators

CLIF leverages C++17 features like **type inference** and **functional programming** to provide robust type safety and generic container support for command-line applications.

## Quick Start

### From Example

The [example directory](../../src/example/) contains a complete CLIF implementation sample demonstrating subcommand binding, option configuration, and custom parameter validation.

Build using either method:

<details>
<summary>Using VS2022:</summary>

1. Open VS2022 and load the solution file: [CLIF_Example.sln](../../build/vs/CLIF_Example/CLIF_Example.sln) in the [VS solution directory](../../build/vs/)
2. Build the solution

</details>

<details>
<summary>CMake build for Cross-platform:</summary>

1. Install CMake (3.10+) on Windows or Linux
2. In terminal at [CMAKE build directory](../../build/cmake/):
    ```bash
    cmake ../../src
    cmake --build .
    ```
3. Executable generated at [here](../../build/cmake/CLIF_example/).

Please note: You can also run the corresponding batch file directly in the root directory of the target system environment (Windows: [autobuild.bat](autobuild.bat); Linux: [autobuild.sh](autobuild.sh)) to automatically build the project.

</details>

### Integrate into Projects

You can add CLIF to your project to create feature-rich CLI tool quickly.

#### Step 0: Project Setup

You directly copy ['The CLIF Folder'](../../src/CLIF/) into your own project. Here, for the convenience of explanation, we assume that the project directory is as follows
```bash
<proj-root>/
├── src
    ├── main.hpp        /// Entry point declarations
    ├── funcs.cpp       /// Custom functions for CLI binding
    ├── CLIF
        ├── CLIF.hpp    /// CLIF declarations and partial implementations
        ├── CLIF.cpp    /// CLIF implementation code
```
> [!NOTE]
> Note: The sample code here is for illustration purposes only. You can adjust it according to the structure of your project.

#### Step 1: Include CLIF

Import the CLIF header file in the entry file and ensure that the compiler enables C++17 or higher standards:
```cpp
/// main.cpp
#include <CLIF/CLIF.hpp>
```
#### Step 2: Define Functions

Functional functions are user-defined collections of functions with a return value of void, and they will be bound to the CLIF for execution. Users can flexibly define any parameter type and parameter quantity according to actual needs. Typically, the 'funcs.hpp' header file is used to centrally manage the logic of the functional functions implemented by users.

The following is an example of usage:
```cpp
/// funcs.hpp
#include <UserLib>

void matrix_op(vector<vector<int>> matrix) {
    /// Process 2D matrix
}
```

#### Step 3: Build CLIF Interface

The CLIF parses user input through the command parser and matches functional functions based on the command name and parameters. Users need to register the binding relationship between commands and functional functions in the entry source file.

The following is the sample code:
```cpp
/// main.cpp
#include <CLIF/CLIF.hpp>
#include "funcs.hpp"

int main(int argc, char *argv[])
{
    /// Enable logging (optional)
    CLIF::FLog logger(&argc, argv);
    CLIF::FApplication app("Scientific Computing Tool");

    /// Register container type
    app.addNewContainerConverter<vector<vector<int>>>();

    /// Bind subcommand with validated parameters
    app.addSubCommand("matrix", "Process a two-dimensional matrix",
    {
        CLIF::FOption()
            .longName("data")
            .shortName("d")
            .help("Two-dimensional matrix data")
            .type<vector<vector<int>>>()
            .validator<vector<vector<int>>>(
                [](vector<vector<int>> m)
                {
                    return m.size() > 0;
                })
            .required()
            .uniqueed()
    }, matrix_op);

    return app.run(argc, argv);
}
```

#### Step 4: Build and Run Help

Open the system terminal in the directory where the generated executable file is located and execute any of the following commands to obtain help information:
```bash
./app.exe ? || ./app.exe h || ./app.exe help || ./app.exe -h || ./app.exe --help
```

If the logging function was enabled when instantiating logger in the previous step, the following command parameters can be specified:
```bash
./app.exe --log-file <log-file-path> --log-level <log-level>
```

• Default log file: `<app-name>.log`
• Log levels: `1:info`, `2:warn`, `3:error`, `4:fatal`

#### Step 5: Execute User Commands

```bash
./app.exe matrix --data "[1,2],[3,4]"

# Automatically parses to -> std::vector<std::vector<int>>
```

## Need Help?
## Contact

- **Maintainer**: [KenanZhu (Nanoki)](https://github.com/KenanZhu)
- **Email**: <nanoki_zh@163.com>
- **Discussion**: Open [Issues](https://github.com/KenanZhu/CLIF/issues) or [PR](https://github.com/KenanZhu/CLIF/pulls)
---

_**Free to use** —— CLIF_