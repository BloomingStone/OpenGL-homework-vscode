#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "Utils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 2

//全局变量
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//导入着色器，初始化窗口
void init(GLFWwindow* window) 
{
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	float vertices[6] = {
		-1.0f, -1.0f,
		0.0f, 1.0f,
		1.0f, -1.0f
	};

	float vertexColors[12] = {
		1.0f, 0.0f, 0.0f, 1.0f,  // red
		0.0f, 1.0f, 0.0f, 1.0f,  // green
		0.0f, 0.0f, 1.0f, 1.0f   // blue
	};

	glGenVertexArrays(numVAOs, vao);
	glGenBuffers(numVBOs, vbo);

	//绑定当前VAO
	glBindVertexArray(vao[0]);
	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	GLuint vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	GLuint vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);

}
//函数绘制
void display(GLFWwindow* window, double currentTime) 
{
	glUseProgram(renderingProgram); 

	glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//绘制模型
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void) 
{
	//glfw初始化
	if (!glfwInit()) 
	{ 
		exit(EXIT_FAILURE); 
	}
	//窗口版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 800, "A triangle", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)//glew初始化
	{ 
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	//导入着色器，创建和绑定VAO和VBO
	init(window);

	//事件循环
	while (!glfwWindowShouldClose(window)) 
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}