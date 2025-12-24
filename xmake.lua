add_rules("mode.debug", "mode.release")

-- 设置 C++ 标准为 C++20
set_languages("c++20")

-- 添加依赖包，使用静态链接
add_requires("glfw", {configs = {shared = false}})
add_requires("glew", {configs = {shared = false}})
add_requires("glm", {configs = {shared = false}})
add_requires("soil2", {configs = {shared = false}})

target("opengl-xmake-clang-template")
    set_kind("binary")
    add_files("src/*.cpp")
    
    -- 添加包含目录
    add_includedirs("src")
    
    -- 添加包依赖
    add_packages("glfw", "glew", "glm", "soil2")
    
    -- 添加系统库依赖 (OpenGL)
    if is_plat("mingw") then
        add_syslinks("opengl32", "gdi32", "user32", "kernel32")
    elseif is_plat("linux") then
        add_syslinks("GL", "dl", "pthread")
    elseif is_plat("macosx") then
        add_frameworks("OpenGL", "CoreFoundation", "CoreGraphics", "IOKit")
    end
    
    -- 复制着色器文件夹到构建目录(如果有其他资源文件或文件夹需要复制到构建目录，可以在这里添加)
    after_build(function (target)
        os.cp("shaders", target:targetdir())
    end)

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--
