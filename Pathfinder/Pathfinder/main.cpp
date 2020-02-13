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
#include <chrono>

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	MapGrid newMap(10, 10);
	auto path = newMap.Find_AStar_Path();
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Time taken by process: " << duration.count() << " microseconds" << std::endl;
	//if (!path.empty()) {
	//	for (int i = 0; i < path.size(); i++) {
	//		std::cout << "step: " << i << " x: " << path[i]->x << " y: " << path[i]->y << "\n";
	//	}
	//}
	//else {
	//	std::cout << "path not found!\n";
	//}
	Start_AppWindow();
	return EXIT_SUCCESS;
}

