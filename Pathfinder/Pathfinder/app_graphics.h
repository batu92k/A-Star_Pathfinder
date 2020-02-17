/**
  ******************************************************************************
  * @file    app_graphics.h
  * @author  Ali Batuhan KINDAN
  * @date    15.02.2020
  * @brief   This file contains the declaration of application graphics
  ******************************************************************************
  */
#ifndef APP_GRAPHICS_H
#define APP_GRAPHICS_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

bool CompileShader(const GLchar* source, GLuint* outShader, GLenum shaderType);
bool Build_ShaderProgram(GLuint* vtxShader, GLuint* fragShader, GLuint* outProgram);

#endif
