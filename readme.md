# 介绍

一个使用 vcpkg + cmake + ninja 配置的 OpenGL 示例项目. 
目前使用的是 MinGW 编译器. 如果使用其他编译器需要自行修改 cmake 与 vcpkg 配置：
1. `.vscode/settings.json` 中修改 为 MSVC 或其他编译器，对应后续 构建与运行 中的方法 2 3 4. 也可以在左侧 Vcpkg CMake Tools （如果安装了） 的 PORTS EXPLORER 窗格中选择对应的配置
   
   ```json
   "vcpkg.target.defaultTriplet": "x64-mingw-static",
   "vcpkg.target.hostTriplet": "x64-mingw-static",
   ```

2. `CMakeUserPresets.json` 同样修改为对应编译器，对应后续 构建与运行 中的方法 1 
   ```json
   "VCPKG_TARGET_TRIPLET": "x64-mingw-static"
   ```

# 使用

## 安装

1. [安装 vcpkg](https://learn.microsoft.com/zh-cn/vcpkg/get_started/get-started-vscode?pivots=shell-powershell)
2. [安装 cmake](https://cmake.org/download/)
3. [安装 ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages)
4. [安装 MinGW](https://github.com/skeeto/w64devkit)
5. 设置环境变量：
   - 设置 VCPKG_ROOT 为 vcpkg 安装目录
   - 设置 将 MinGW 的 bin 目录添加到 PATH 环境变量中
6. 推荐安装 VSCode 插件：
   1. Vcpkg CMake Tools;
   2. CMake Tools;
   3. CMake;

## 构建与运行

1. 可以直接 shell / CMD 中进入当前目录，或者在 VSCode 中进入终端，然后运行 `cmake --preset=default; cmake --build .\build --clean-first; .\build\MyGLProject.exe`. 相关设定在 `CMakeLists.txt`, `CMakePressets.json`, `CMakeUserPresets.json` 中
2. 在 VSCode 中 ctl+p 打开命令面板，输入 `cmake: build` 安装项目，然后`.\build\MyGLProject.exe` 运行项目
3. 在 VSCode 中 ctl+p 打开命令面板, 输入 `Run Task` 选择需要运行的项目
4. 在 VSCode 中 通过运行和调试 (ctl + shift + d) 构建 debug 项目并运行或调试
   - 以上 2 3 4 的配置在 `.vscode/tasks.json` 和 `.vscode/launch.json` 中
