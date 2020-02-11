/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Ali Batuhan KINDAN
  * @date    10.02.2020
  * @brief   This file contains the test application which demonstrates the AStar
  *          pathfinding algorithm
  ******************************************************************************
  */

#include "a_star_search.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include <iostream>

int main()
{
	std::cout << "Test\n";
	Init_Grid();
	FindPath();
	return EXIT_SUCCESS;
}
