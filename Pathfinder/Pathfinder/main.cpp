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
	auto path = FindPath();
	if (!path.empty()) {
		for (int i = 0; i < path.size(); i++) {
			std::cout << "step: " << i << " x: " << path[i]->x << " y: " << path[i]->y << "\n";
		}
	}
	else {
		std::cout << "path not found!\n";
	}
	return EXIT_SUCCESS;
}

