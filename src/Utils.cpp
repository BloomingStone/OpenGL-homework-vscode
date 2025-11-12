#include "Utils.h"

namespace fs = std::filesystem;

// ========== 工具函数 ==========

fs::path Utils::getExecutableDir() {
#ifdef _WIN32
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    return fs::path(exePath).parent_path();
#elif __APPLE__
    char exePath[PATH_MAX];
    uint32_t size = sizeof(exePath);
    if (_NSGetExecutablePath(exePath, &size) != 0)
        throw std::runtime_error("Buffer too small for executable path");
    return fs::canonical(fs::path(exePath)).parent_path();
#elif __linux__
    return fs::canonical(fs::read_symlink("/proc/self/exe")).parent_path();
#else
    static_assert(false, "Unsupported platform");
#endif
}

std::string Utils::readShaderFile(const fs::path& filePath) {
    std::ifstream file{filePath};
    if (!file.is_open()) {
        throw std::runtime_error(std::format("Failed to open shader file: {}", filePath.string()));
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// ========== OpenGL 错误检查与日志 ==========

bool Utils::checkOpenGLError() {
    auto foundError{false};
    for (GLenum err; (err = glGetError()) != GL_NO_ERROR;) {
        std::cerr << std::format("glError: {}\n", err);
        foundError = true;
    }
    return foundError;
}

void Utils::printShaderLog(GLuint shader) {
    GLint len{0};
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        std::string log(len, '\0');
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << std::format("Shader Info Log:\n{}\n", log);
    }
}

void Utils::printProgramLog(GLuint prog) {
    GLint len{0};
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        std::string log(len, '\0');
        glGetProgramInfoLog(prog, len, nullptr, log.data());
        std::cerr << std::format("Program Info Log:\n{}\n", log);
    }
}

// ========== Shader 编译与链接 ==========

GLuint Utils::compileShader(GLenum shaderType, const char* shaderPath) {
	// load shader file from executable directory
    auto shaderFullPath = getExecutableDir() / shaderPath;
    auto shaderSrc = readShaderFile(shaderFullPath);
    auto src = shaderSrc.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        std::cerr << std::format("Shader compilation failed: {}\n", shaderPath);
        printShaderLog(shader);
    }
    return shader;
}

GLuint Utils::linkShapderProgram(GLuint sprogram) {
    glLinkProgram(sprogram);
    GLint linked = 0;
    glGetProgramiv(sprogram, GL_LINK_STATUS, &linked);
    if (!linked) {
        std::cerr << "Shader linking failed\n";
        printProgramLog(sprogram);
    }
    return sprogram;
}

// ========== Shader Program 生成 ==========

GLuint Utils::createShaderProgramImpl(std::initializer_list<std::pair<GLenum, const char*>> shaders) {
    GLuint program = glCreateProgram();
    for (auto&& [type, path] : shaders) {
        GLuint shader = compileShader(type, path);
        glAttachShader(program, shader);
    }
    return linkShapderProgram(program);
}


GLuint Utils::createShaderProgram(const char* vp, const char* fp) {
	return createShaderProgramImpl({
		{GL_VERTEX_SHADER, vp},
		{GL_FRAGMENT_SHADER, fp}
	});
}

GLuint Utils::createShaderProgram(const char* vp, const char* gp, const char* fp) {
    return createShaderProgramImpl({
        {GL_VERTEX_SHADER, vp},
        {GL_GEOMETRY_SHADER, gp},
        {GL_FRAGMENT_SHADER, fp}
    });
}

GLuint Utils::createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp) {
	return createShaderProgramImpl({
		{GL_VERTEX_SHADER, vp},
		{GL_TESS_CONTROL_SHADER, tCS},
		{GL_TESS_EVALUATION_SHADER, tES},
		{GL_FRAGMENT_SHADER, fp}
	});
}

GLuint Utils::createShaderProgram(const char* vp, const char* tCS, const char* tES, char* gp, const char* fp) {
    return createShaderProgramImpl({
        {GL_VERTEX_SHADER, vp},
        {GL_TESS_CONTROL_SHADER, tCS},
        {GL_TESS_EVALUATION_SHADER, tES},
        {GL_GEOMETRY_SHADER, gp},
        {GL_FRAGMENT_SHADER, fp}
    });
}

// ========== Texture Loader ==========

GLuint Utils::loadTexture(const char* texImagePath) {
    auto texFullPath = getExecutableDir() / texImagePath;

    GLuint texID = SOIL_load_OGL_texture(texFullPath.string().c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!texID)
        std::cerr << std::format("Texture not found: {}\n", texFullPath.string());

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat aniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    }
    return texID;
}

// GOLD material - ambient, diffuse, specular, and shininess
float* Utils::goldAmbient() { static float a[4] = { 0.2473f, 0.1995f, 0.0745f, 1 }; return (float*)a; }
float* Utils::goldDiffuse() { static float a[4] = { 0.7516f, 0.6065f, 0.2265f, 1 }; return (float*)a; }
float* Utils::goldSpecular() { static float a[4] = { 0.6283f, 0.5559f, 0.3661f, 1 }; return (float*)a; }
float Utils::goldShininess() { return 51.2f; }

// SILVER material - ambient, diffuse, specular, and shininess
float* Utils::silverAmbient() { static float a[4] = { 0.1923f, 0.1923f, 0.1923f, 1 }; return (float*)a; }
float* Utils::silverDiffuse() { static float a[4] = { 0.5075f, 0.5075f, 0.5075f, 1 }; return (float*)a; }
float* Utils::silverSpecular() { static float a[4] = { 0.5083f, 0.5083f, 0.5083f, 1 }; return (float*)a; }
float Utils::silverShininess() { return 51.2f; }

// BRONZE material - ambient, diffuse, specular, and shininess
float* Utils::bronzeAmbient() { static float a[4] = { 0.2125f, 0.1275f, 0.0540f, 1 }; return (float*)a; }
float* Utils::bronzeDiffuse() { static float a[4] = { 0.7140f, 0.4284f, 0.1814f, 1 }; return (float*)a; }
float* Utils::bronzeSpecular() { static float a[4] = { 0.3936f, 0.2719f, 0.1667f, 1 }; return (float*)a; }
float Utils::bronzeShininess() { return 25.6f; }
