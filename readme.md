# OpenGL xmake Clang/Mingw Template

一个使用 xmake 构建系统，从而可以支持 MSVC, Clang 和 MinGW 编译器的 OpenGL 项目模板。

## 项目概述

此项目是一个简单的 OpenGL 示例，展示了一个彩色三角形的渲染。它使用现代 OpenGL（4.3）和以下库：

- GLFW：窗口和输入管理
- GLEW：OpenGL 扩展加载
- GLM：数学库
- SOIL2：图像加载（用于纹理）

## 构建系统

项目使用 [xmake](https://xmake.io/) 作为构建系统，可选编译器配置为：

1. **MSVC**：visual sutdio 默认编译器
2. **Clang (clang, clang-cl)**：提供更好的报错信息
3. **MinGW**：GNU 工具链

### 前提条件

#### 1. 安装 xmake
请参考 [xmake 安装指南](https://xmake.io/#/guide/installation)

#### 2. 选择编译器

**MSVC**
官网下载 [Visual Studio](https://visualstudio.microsoft.com/zh-hans/)

**Clang (版本 ≥19.0.0)**
```bash
# Windows 上推荐使用 LLVM 发行版
# 下载地址：https://github.com/llvm/llvm-project/releases
# 确保 clang-cl 在 PATH 中
# 验证安装
clang-cl --version
```

**MinGW**
```bash
# 1. 下载 MinGW-w64
#    地址：https://github.com/niXman/mingw-builds-binaries/releases

# 2. 将 MinGW 的 bin 目录添加到 PATH
#    例如：C:\mingw64\bin

# 3. 验证安装
g++ --version
```

### 构建步骤

### 配置项目

```bash
# 1. 若指定使用 MSVC
xmake f -p windows -a x64 -m release --toolchain=msvc -y

# 2. 若指定使用 Clang
xmake f -p windows -a x64 -m release --toolchain=clang -y    # GNU ABI 或
xmake f -p windows -a x64 -m release --toolchain=clang-cl -y  # MSVC ABI

# 3. 若指定使用 MinGW
xmake f -p mingw -a x86_64 -m release --toolchain=mingw -y
```

若不指定则默认使用平台对应的编译器，对于 windows 平台默认使用 MSVC
下载库需要链接到github, 如有需要可设置代理或镜像，参考 [xmake 网络优化](https://xmake.io/zh/guide/package-management/network-optimization.html)

### 构建与运行

```bash
# 构建项目
xmake

# 运行程序
xmake run
```

### 常用命令

```bash
# 清理构建
xmake clean

# 重新配置并构建
xmake f -c
xmake

# 调试构建
xmake f -m debug
xmake

# 运行调试版本
xmake run -d

# 生成 compile_commands.json（用于 clangd）
xmake project -k compile_commands
```

## 项目结构

```
.
├── xmake.lua          # xmake 构建配置
├── src/
│   ├── main.cpp      # 主程序入口
│   ├── Utils.h       # 工具类头文件
│   └── Utils.cpp     # 工具类实现
├── shaders/
│   ├── vertShader.glsl  # 顶点着色器
│   └── fragShader.glsl  # 片段着色器
├── compile_commands.json # 自动生成，用于 IDE 智能提示
└── build/            # 构建输出目录（在 .gitignore 中）
```

## 依赖管理

所有依赖通过 xmake 的包管理自动下载和构建：

```lua
-- Clang 配置
add_requires("glfw", {configs = {shared = false}})    -- shared = false 表示静态链接
add_requires("glew", {configs = {shared = false}})
add_requires("glm", {configs = {shared = false}})
add_requires("soil2", {configs = {shared = false}})

-- MinGW 配置（指定平台）
add_requires("glfw", {configs = {shared = false, plat = "mingw"}})
add_requires("glew", {configs = {shared = false, plat = "mingw"}})
add_requires("glm", {configs = {plat = "mingw"}})
add_requires("soil2", {configs = {shared = false, plat = "mingw"}})
```

## IDE 配置（VS Code）

推荐安装 clangd 和 XMake 插件

1. **生成 compile_commands.json**
   - 手动生成：`xmake project -k compile_commands` 有相关include信息

2. **配置 clangd**
   - 安装 VS Code 的 clangd 扩展
   - 如果 `compile_commands.json` 存在，clangd 会自动使用它
   - 如果未成功识别，重启 clangd：Ctrl+P 输入 `clangd: restart`

3. vscode xmake 插件
   - 安装后 vscode 最下方会出现构建相关的工具栏，可以选择构架、编译模式等
   ![vscode-XMake-toolbar](./images/vscode-XMake-toolbar.png)
   - 如果安装后未出现，可以点击资源管理器中的`xmake.lua`，此后可能显示

### 参考链接
- [xmake IDE 集成插件](https://xmake.io/guide/extensions/ide-integration-plugins.html#configure-intellsence)
- [clangd 官方文档](https://clangd.llvm.org/)

## 许可证

本项目仅供学习使用。相关库的许可证请参考各自的项目页面。

## 可选分支配置
可选 `cmake+vcpkg+ninja+mingw` 分支以使用对应构建配置


## 故障排除

### Clang 版本问题
确保使用 Clang 19.0.0 或更高版本以避免与 MSVC STL 的兼容性问题。

