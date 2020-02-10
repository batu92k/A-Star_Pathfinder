/**
  ******************************************************************************
  * @file    a_star_search.cpp
  * @author  Ali Batuhan KINDAN
  * @date    10.02.2020
  * @brief   This file contains the implementation of methods for A Star Algorithm
  ******************************************************************************
  */
#include "a_star_search.h"
#include <iostream>
#include <queue>
#include <list>
#include <algorithm>

const int GRID_SIZE_X = 8; // num of nodes along x
const int GRID_SIZE_Y = 8; // num of nodes along y
const int GRID_SPACE = 50; // space between nodes on 1 axis
Node* nodes = nullptr;
Node* start = nullptr;
Node* target = nullptr;

void Init_Grid()
{
	nodes = new Node[GRID_SIZE_X * GRID_SIZE_Y];
	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int y = 0; y < GRID_SIZE_Y; y++)
		{
			nodes[y * GRID_SIZE_X + x].x = x;
			nodes[y * GRID_SIZE_X + x].y = y;
			nodes[y * GRID_SIZE_X + x].parent = nullptr;
			nodes[y * GRID_SIZE_X + x].isObstacle = false;
			nodes[y * GRID_SIZE_X + x].isVisited = false;
			nodes[y * GRID_SIZE_X + x].globalGoal = INFINITY;
			nodes[y * GRID_SIZE_X + x].localGoal = INFINITY;
		}
	}

	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int y = 0; y < GRID_SIZE_Y; y++)
		{
			if (x > 0) {
				nodes[y * GRID_SIZE_X + x].neighbours.push_back(&nodes[y * GRID_SIZE_X + (x - 1)]);
			}
			if (x < (GRID_SIZE_X - 1)) {
				nodes[y * GRID_SIZE_X + x].neighbours.push_back(&nodes[y * GRID_SIZE_X + (x + 1)]);
			}
			if (y > 0) {
				nodes[y * GRID_SIZE_X + x].neighbours.push_back(&nodes[(y - 1) * GRID_SIZE_X + x]);
			}
			if (y < (GRID_SIZE_Y - 1)) {
				nodes[y * GRID_SIZE_X + x].neighbours.push_back(&nodes[(y + 1) * GRID_SIZE_X + x]);
			}
		}
	}

	start = &nodes[0];
	target = &nodes[(GRID_SIZE_X * GRID_SIZE_Y) - 1];
}

float Distance(Node* a, Node* b)
{
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

float Heuristic(Node* a, Node* b)
{
	return Distance(a, b);
}

void FindPath()
{
	Node* current = start;
	start->localGoal = 0.0f;
	start->globalGoal = Heuristic(start, target);
	// declare a list for non tested nodes and put start node to list initially 
	std::list<Node*> nodesToBeTested;
	nodesToBeTested.push_back(start);

	while (!nodesToBeTested.empty() && current != target) {
		// sort nodes to be tested by global value (lowest first!)
		nodesToBeTested.sort([](const Node* a, const Node* b){ return a->globalGoal < b->globalGoal; });

		// after the sort if the front of the list is already visited remove it
		while (!nodesToBeTested.empty() && nodesToBeTested.front()->isVisited) {
			nodesToBeTested.pop_front();
		}

		// if there is no node to be tested break
		if (nodesToBeTested.empty()) break;

		current = nodesToBeTested.front();
		current->isVisited = true;

		// check neighbours of current node
		for (auto neighbourNode : current->neighbours) {
			if (!neighbourNode->isVisited && !neighbourNode->isObstacle) {
				nodesToBeTested.push_back(neighbourNode);
			}

			if ((current->localGoal + Distance(current, neighbourNode)) < neighbourNode->localGoal) {
				neighbourNode->parent = current;
				neighbourNode->localGoal = current->localGoal + Distance(current, neighbourNode);
				neighbourNode->globalGoal = neighbourNode->localGoal + Heuristic(neighbourNode, target);
			}
		}
	}	
}

