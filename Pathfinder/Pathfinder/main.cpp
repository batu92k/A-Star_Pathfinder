/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Ali Batuhan KINDAN
  * @date    10.02.2020
  * @brief   This file contains the test application which demonstrates the AStar
  *          pathfinding algorithm
  ******************************************************************************
  */

#include "app_window.h"
#include "a_star_search.h"
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
	Start_AppWindow();
	return EXIT_SUCCESS;
}

