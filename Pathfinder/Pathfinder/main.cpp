/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Ali Batuhan KINDAN
  * @date    10.02.2020
  * @brief   This file contains the test application which demonstrates the AStar
  *          pathfinding algorithm
  ******************************************************************************
  */

#include <iostream>
#include "a_star_search.h"

int main()
{
	std::cout << "Test\n";
	Init_Grid();
	FindPath();
	return EXIT_SUCCESS;
}
