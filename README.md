[English](./doc/readme_en/README.md) | 简体中文

# CLIF ：基于C/C++的函数式命令行框架

---
## 为什么选择CLIF？

✅ **单文件依赖** \
 : 仅依赖C++17标准库, 无需任何外部依赖

✅ **函数式编程** \
 : 命令直接绑定到函数，避免手动解析参数

✅ **强类型安全** \
 : 构建期捕获类型冲突，避免运行时错误

✅ **泛容器支持** \
 ：原生支持嵌套容器，支持自定义解析器和验证器

CLIF充分利用了C++17的**类型推断**和**函数式编程**等特性，为命令行应用程序提供强大的类型安全和泛容器支持。

## 快速开始

### 从示例开始

在[示例目录](./src/example/)中包含了一套完整的CLIF构建示例。这个示例比较详细的展现了CLIF的一些使用方法，包含子命令绑定，选项参数设置，参数自定义验证器...

要构建示例，可以使用多种方式，项目中我们给出两种方式：

<details>
<summary>使用VS2022构建项目：</summary>

1. 打开VS2022，打开项目：选择[VS解决方案目录](./build/vs/)中的[`CLIF_example.sln`](./build/vs/CLIF_example/CLIF_example.sln)文件。
2. 加载项目，生成解决方案即可。

</details>

<details>
<summary>使用CMake跨平台构建：</summary>

1. 在Windows或Linux中安装CMake，版本要求3.10+。
2. 在[CMAKE构建目录](./build/cmake/)中打开终端，执行以下命令：
    ```bash
    cmake ../../src
    cmake --build .
    ```

3. 可执行文件生成于[此处](./build/cmake/bin/CLIF_example/)。

你还可以直接在对应系统环境下在根目录运行其对应的批处理文件（Windows：[autobuild.bat](autobuild.bat); Linux：[autobuild.sh](autobuild.sh)），自动构建项目。

</details>

### 集成到项目

你可以将CLIF集成到你的项目中，快速构建一个功能完善的命令行工具。

#### 步骤0：创建项目

你可以将[CLIF文件夹](./src/CLIF/)直接复制到你的项目中。这里我们为了方便说明，假设项目目录如下所示：
```
<proj-root>/
├── src
    ├── main.hpp        /// 包含主程序入口函数。
    ├── funcs.cpp       /// 自定义的供CLIF使用的接口函数。
    ├── CLIF
        ├── CLIF.hpp    /// 包含了CLIF的类，函数，类型定义等的声明和部分实现。
        ├── CLIF.cpp    /// 包含了CLIF的类，函数，类型定义等的实现。
```
> [!NOTE]
> 注意：这里的示例代码是为了便于说明，你可以根据自己的项目结构进行调整。

#### 步骤1: 引入CLIF

在入口文件中引入CLIF头文件，确保编译器启用C++17或更高标准：
```cpp
/// main.cpp
#include <CLIF/CLIF.hpp>
```

#### 步骤2: 定义功能函数

功能函数是用户自定义的、返回值为void的函数集合，它们将被绑定到CLIF中执行。用户可以根据实际需求灵活定义任意参数类型和参数数量。通常，`funcs.hpp`头文件用于集中管理用户实现的功能函数逻辑。

以下是用法示例：
```cpp
/// funcs.hpp
#include <UserLib>

void matrix_op(vector<vector<int>> matrix) {
    /// 处理二维矩阵
}
```

#### 步骤3: 构建CLIF接口

CLIF通过命令解析器解析用户输入，根据命令名称和参数匹配功能函数。用户需要在入口源文件中注册命令和功能函数的绑定关系。

以下是示例代码：
```cpp
/// main.cpp
#include <CLIF/CLIF.hpp>
#include "funcs.hpp"

int main(int argc, char *argv[])
{
    /// 如果要启用日志功能，请实例化日志logger
    CLIF::FLog logger(&argc, argv);
    CLIF::FApplication app("科学计算工具");

    /// 注册容器类型
    app.registerContainerConverter<vector<vector<int>>>();

    /// 链接子命令和功能函数及带验证器的命令参数
    app.addSubCommand("matrix", "处理二维矩阵",
    {
        CLIF::FOption()
            .longName("data")
            .shortName("d")
            .help("二维矩阵数据")
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

#### 步骤4：构建并运行

在生成的可执行文件所在目录打开系统终端，执行下列命令任一获取帮助信息：
```bash
./app.exe ? || ./app.exe h || ./app.exe help || ./app.exe -h || ./app.exe --help
```

如果在上一步骤中实例化logger启用了日志功能，可以指定以下命令参数：
```bash
./app.exe --log-file <log-file-path> --log-level <log-level>
```
• 默认日志文件: `<app-name>.log`
• 日志级别对应: `1:info`, `2:warn`, `3:error`, `4:fatal`

#### 步骤5: 运行用户命令

```bash
./app.exe matrix --data "[1,2],[3,4]"

# 根据函数参数类型签名，自动解析为 -> std::vector<std::vector<int>>.
```

## 需要帮助？
## 联系我

- **维护**: [KenanZhu (Nanoki)](https://github.com/KenanZhu)
- **邮箱**: <nanoki_zh@163.com>
- **讨论**: 提交[Issues](https://github.com/KenanZhu/CLIF/issues)或[PR](https://github.com/KenanZhu/CLIF/pulls)
---

_**Free to use** —— CLIF_