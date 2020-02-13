/**
  ******************************************************************************
  * @file    app_window.cpp
  * @author  Ali Batuhan KINDAN
  * @date    12.02.2020
  * @brief   This file contains the implementation of App Window components
  ******************************************************************************
  */

#include "app_window.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define APP_GL_VER_MAJOR 3
#define APP_GL_VER_MINOR 3

static void glfw_error_callback(int error, const char* description);
static void glfw_mouse_btn_callback(GLFWwindow* window, int button, int action, int mods);

void Start_AppWindow()
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		return;
	}

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, APP_GL_VER_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, APP_GL_VER_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "A-Star Pathfinder", NULL, NULL);
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

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw Here!

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void glfw_mouse_btn_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		std::cout << "Right Click Clicked\n";
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "Left Click Clicked\n";
	}
}

