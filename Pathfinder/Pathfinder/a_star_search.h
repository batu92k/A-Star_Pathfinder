/**
  ******************************************************************************
  * @file    a_star_search.h
  * @author  Ali Batuhan KINDAN
  * @date    10.02.2020
  * @brief   This file contains the declaration of methods for A Star Algorithm
  ******************************************************************************
  */

#ifndef A_STAR_SEARCH_H
#define A_STAR_SEARCH_H

#include<vector>

class Node {
public:
	bool isObstacle = false;
	bool isVisited = false;
	float globalGoal = INFINITY;
	float localGoal = INFINITY;
	int x;
	int y;
	std::vector<Node*> neighbours;
	Node* parent = nullptr;
};

void Init_Grid();
std::vector<Node*> FindPath();

#endif

