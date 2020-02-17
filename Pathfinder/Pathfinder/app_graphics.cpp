/**
  ******************************************************************************
  * @file    app_graphics.cpp
  * @author  Ali Batuhan KINDAN
  * @date    15.02.2020
  * @brief   This file contains the implementation of application graphics
  ******************************************************************************
  */
#include "app_graphics.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

bool CompileShader(const GLchar* source, GLuint* outShader, GLenum shaderType)
{
	if (!(shaderType == GL_FRAGMENT_SHADER || shaderType == GL_VERTEX_SHADER)) {
		return false;
	}
	int success;
	char infoLog[512];

	*outShader = glCreateShader(shaderType);
	glShaderSource(*outShader, 1, &source, NULL);
	glCompileShader(*outShader);
	glGetShaderiv(*outShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(*outShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	
	return true;
}

bool Build_ShaderProgram(GLuint* vtxShader, GLuint* fragShader, GLuint* outProgram)
{
	int success;
	char infoLog[512];
	*outProgram = glCreateProgram();
	glAttachShader(*outProgram, *vtxShader);
	glAttachShader(*outProgram, *fragShader);
	glLinkProgram(*outProgram);

	glGetProgramiv(*outProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*outProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERS::LINKER_FAILED\n" << infoLog << std::endl;
		return false;
	}

	return true;
}

