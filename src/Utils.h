#pragma once
#include <string>
#include <filesystem>
#include <optional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#include <limits.h>
#endif

namespace fs = std::filesystem;

class Utils {
private:
    static std::string readShaderFile(const fs::path& filePath);
    static void printShaderLog(GLuint shader);
    static void printProgramLog(GLuint prog);
    static GLuint compileShader(GLenum shaderType, const char* shaderPath);
    static GLuint linkShapderProgram(GLuint sprogram);
    static fs::path getExecutableDir();
    static std::optional<fs::path> tryFindShaderFile(const fs::path&& filePath);

public:
    Utils() = default;
    static bool checkOpenGLError();

	static GLuint createShaderProgramImpl(std::initializer_list<std::pair<GLenum, const char*>> shaders);
    static GLuint createShaderProgram(const char* vp, const char* fp);
    static GLuint createShaderProgram(const char* vp, const char* gp, const char* fp);
    static GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp);
    static GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, char* gp, const char* fp);

    static GLuint loadTexture(const char* texImagePath);
    static GLuint loadCubeMap(const char* mapDir);

    static float* goldAmbient();
    static float* goldDiffuse();
    static float* goldSpecular();
    static float goldShininess();

    static float* silverAmbient();
    static float* silverDiffuse();
    static float* silverSpecular();
    static float silverShininess();

    static float* bronzeAmbient();
    static float* bronzeDiffuse();
    static float* bronzeSpecular();
    static float bronzeShininess();
};
