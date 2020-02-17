/**
  ******************************************************************************
  * @file    app_window.cpp
  * @author  Ali Batuhan KINDAN
  * @date    12.02.2020
  * @brief   This file contains the implementation of App Window components
  ******************************************************************************
  */

#include "app_window.h"
#include "app_graphics.h"
#include "map_grid.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define APP_GL_VER_MAJOR 3
#define APP_GL_VER_MINOR 3

const int APP_WINDOW_SIZE = 800;
const int GRID_SIZE = 10;

const float DRAW_FRAME_OFFSET = APP_WINDOW_SIZE * 0.05f;
const float GRID_CELL_SIZE = (APP_WINDOW_SIZE - 2.0f * DRAW_FRAME_OFFSET) / GRID_SIZE;

MapGrid newMap(GRID_SIZE, GRID_SIZE);  // create 10x10 square map grid
float gridVertiColor[GRID_SIZE * GRID_SIZE * 24];
unsigned int gridIndices[GRID_SIZE * GRID_SIZE * 6];

const float COLOR_EMPTY[3] = { 52.0f / 255.0f, 198.0f / 255.0f ,235.0f / 255.0f };
const float COLOR_TARGET[3] = { 1.0f, 0.0f, 0.0f };
const float COLOR_START[3] = { 0.0f, 1.0f, 0.0f };
const float COLOR_OBSTACE[3] = { 169.0f / 255.0f, 169.0f / 255.0f ,169.0f / 255.0f };
const float COLOR_VISITED[3] = { 235.0f / 255.0f , 153.0f / 255.0f, 52.0f / 255.0f };

const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   vec3 orthoPos = (aPos - 400) / 400.0f;\n"
"   gl_Position = vec4(orthoPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(ourColor, 1.0);\n"
"}\0";

unsigned int VBO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;
unsigned int VAO;
unsigned int EBO; // element array buffer

static void UpdateGridVertices(void);
static void glfw_error_callback(int error, const char* description);
static void glfw_mouse_btn_callback(GLFWwindow* window, int button, int action, int mods);


void GenerateTestBuffers() 
{
	// generate arrays in the beginning
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertiColor), gridVertiColor, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	// position has attribute 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color has attribute 1 (has offset of 3 floats from beginning)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gridIndices), gridIndices, GL_STATIC_DRAW);
}


void Start_AppWindow()
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, APP_GL_VER_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, APP_GL_VER_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(APP_WINDOW_SIZE, APP_WINDOW_SIZE, "A-Star Pathfinder", NULL, NULL);
	if (window == NULL) {
		return;
	}

	glfwSetMouseButtonCallback(window, glfw_mouse_btn_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // vsync enabled

	if (gl3wInit() != 0) {
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return;
	}

	if (!gl3wIsSupported(APP_GL_VER_MAJOR, APP_GL_VER_MINOR)) {
		std::cout << "GL version: "<< APP_GL_VER_MAJOR << "." << APP_GL_VER_MINOR <<  " Not Supported!" << std::endl;
		return;
	}

	std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	UpdateGridVertices();

	GenerateTestBuffers();
	CompileShader(vertexShaderSource, &vertexShader, GL_VERTEX_SHADER);
	CompileShader(fragmentShaderSource, &fragmentShader, GL_FRAGMENT_SHADER);
	Build_ShaderProgram(&vertexShader, &fragmentShader, &shaderProgram);

	while (!glfwWindowShouldClose(window))
	{	
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw Here!
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, GRID_SIZE * GRID_SIZE * 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
}

static void UpdateGridVertices(void)
{
	newMap.ResetMap();
	auto path = newMap.Find_AStar_Path();
	MapGrid::Node* mapGrid = newMap.GetGridArray();
	const MapGrid::Node* startNode = newMap.GetStartNode();
	const MapGrid::Node* targetNode = newMap.GetTargetNode();

	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
	{
		float centerX = DRAW_FRAME_OFFSET + GRID_CELL_SIZE * mapGrid[i].x + (GRID_CELL_SIZE / 2.0f);
		float centerY = DRAW_FRAME_OFFSET + GRID_CELL_SIZE * mapGrid[i].y + (GRID_CELL_SIZE / 2.0f);
		int vtxIdx = 0;
		int indiceIdx = 0;
		const float* cellColor = COLOR_EMPTY;
		if (&mapGrid[i] == startNode) cellColor = COLOR_START;
		else if (&mapGrid[i] == targetNode) cellColor = COLOR_TARGET;
		else if (mapGrid[i].isObstacle) cellColor = COLOR_OBSTACE;
		else if (mapGrid[i].isVisited) cellColor = COLOR_VISITED;

		gridVertiColor[i * 24 + vtxIdx++] = centerX + (GRID_CELL_SIZE / 2.0f) * 0.8f; // x0
		gridVertiColor[i * 24 + vtxIdx++] = centerY + (GRID_CELL_SIZE / 2.0f) * 0.8f; // y0
		gridVertiColor[i * 24 + vtxIdx++] = 100.0f;  // z0
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[0]; // color
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[1]; // color
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[2];  // color
		gridVertiColor[i * 24 + vtxIdx++] = centerX + (GRID_CELL_SIZE / 2.0f) * 0.8f; // x1
		gridVertiColor[i * 24 + vtxIdx++] = centerY - (GRID_CELL_SIZE / 2.0f) * 0.8f; // y1
		gridVertiColor[i * 24 + vtxIdx++] = 100.0f;  // z1
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[0]; // color
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[1]; // color
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[2];  // color
		gridVertiColor[i * 24 + vtxIdx++] = centerX - (GRID_CELL_SIZE / 2.0f) * 0.8f; // x2
		gridVertiColor[i * 24 + vtxIdx++] = centerY - (GRID_CELL_SIZE / 2.0f) * 0.8f; // y2
		gridVertiColor[i * 24 + vtxIdx++] = 100.0f;  // z2
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[0]; // color
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[1]; // color
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[2];  // color
		gridVertiColor[i * 24 + vtxIdx++] = centerX - (GRID_CELL_SIZE / 2.0f) * 0.8f; // x3
		gridVertiColor[i * 24 + vtxIdx++] = centerY + (GRID_CELL_SIZE / 2.0f) * 0.8f; // y3
		gridVertiColor[i * 24 + vtxIdx++] = 100.0f;  // z3
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[0]; // color
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[1]; // color
		gridVertiColor[i * 24 + vtxIdx++] = cellColor[2];  // color

		gridIndices[i * 6 + indiceIdx++] = i * 4;
		gridIndices[i * 6 + indiceIdx++] = i * 4 + 1;
		gridIndices[i * 6 + indiceIdx++] = i * 4 + 3;
		gridIndices[i * 6 + indiceIdx++] = i * 4 + 1;
		gridIndices[i * 6 + indiceIdx++] = i * 4 + 2;
		gridIndices[i * 6 + indiceIdx++] = i * 4 + 3;
	}
	// update grid VBO after modification
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertiColor), gridVertiColor, GL_STATIC_DRAW);
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void glfw_mouse_btn_callback(GLFWwindow* window, int button, int action, int mods)
{
	int width, height;
	double curPosX, curPosY;
	glfwGetFramebufferSize(window, &width, &height);
	glfwGetCursorPos(window, &curPosX, &curPosY);
	// adjust Y origin from top left to bottom left
	curPosY -= height;
	curPosY = -curPosY;

	if (curPosX > DRAW_FRAME_OFFSET&& curPosX < ((double)width - DRAW_FRAME_OFFSET) &&
		curPosY > DRAW_FRAME_OFFSET&& curPosY < ((double)height - DRAW_FRAME_OFFSET)) {
		int cellX = (curPosX - DRAW_FRAME_OFFSET) / GRID_CELL_SIZE;
		int cellY = (curPosY - DRAW_FRAME_OFFSET) / GRID_CELL_SIZE;

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			newMap.ToggleObstacle(MapGrid::GridPos(cellX, cellY));
			UpdateGridVertices();
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			std::cout << "Left Click Clicked\n";
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			std::cout << "Middle Click Clicked\n";
		}
	}
}

