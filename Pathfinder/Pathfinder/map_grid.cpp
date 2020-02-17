/**
  ******************************************************************************
  * @file    map_grid.cpp
  * @author  Ali Batuhan KINDAN
  * @date    10.02.2020
  * @brief   This file contains the implementation of methods for A Star Algorithm
  ******************************************************************************
  */
#include "map_grid.h"
#include <iostream>
#include <queue>
#include <list>
#include <algorithm>

MapGrid::MapGrid(int SizeX = 10, int SizeY = 10)
{
	_GridSizeX = SizeX;
	_GridSizeY = SizeY;
	ResetMap();
	// set initial start and target
	_Start = &_Nodes[0];
	_Target = &_Nodes[(_GridSizeX * _GridSizeY) - 1];
}

MapGrid::Node* MapGrid::GetGridArray(void)
{
	return _Nodes;
}

MapGrid::GridSize MapGrid::GetGridSize(void)
{
	return GridSize(_GridSizeX, _GridSizeY);
}

MapGrid::Node* MapGrid::GetTargetNode(void)
{
	return _Target;
}

MapGrid::Node* MapGrid::GetStartNode(void)
{
	return _Start;
}

MapGrid::GridPos MapGrid::GetTargetPos(void)
{
	return GridPos(_Target->x, _Target->y);
}

void MapGrid::SetTargetPos(GridPos targetPos)
{
	int idx = targetPos.second * _GridSizeX + targetPos.first;
	if (idx > (_GridSizeX * _GridSizeY) || idx < 0) return;
	_Target = &_Nodes[idx];
}

MapGrid::GridPos MapGrid::GetStartPos(void)
{
	return GridPos(_Start->x, _Start->y);
}


void MapGrid::SetStartPos(GridPos startPos)
{
	int idx = startPos.second * _GridSizeX + startPos.first;
	if (idx > (_GridSizeX * _GridSizeY - 1) || idx < 0) return;
	_Start = &_Nodes[idx];
}

void MapGrid::ToggleObstacle(GridPos obstaclePos)
{
	int idx = obstaclePos.second * _GridSizeX + obstaclePos.first;
	if (idx > (_GridSizeX * _GridSizeY - 1) || idx < 0) return; // if index is outside the array then return
	if (&_Nodes[idx] == _Target || &_Nodes[idx] == _Start) return; // if index hits target or start then return
	_Nodes[idx].isObstacle ^= true;
}

void MapGrid::ResetMap()
{
	if (_Nodes == nullptr) {
		_Nodes = new Node[_GridSizeX * _GridSizeY];
	}

	for (int x = 0; x < _GridSizeX; x++)
	{
		for (int y = 0; y < _GridSizeY; y++)
		{
			_Nodes[y * _GridSizeX + x].x = x;
			_Nodes[y * _GridSizeX + x].y = y;
			_Nodes[y * _GridSizeX + x].parent = nullptr;
			//_Nodes[y * _GridSizeX + x].isObstacle = false; // keep the obstacle value for new calculation
			_Nodes[y * _GridSizeX + x].isVisited = false;
			_Nodes[y * _GridSizeX + x].globalGoal = INFINITY;
			_Nodes[y * _GridSizeX + x].localGoal = INFINITY;
			_Nodes[y * _GridSizeX + x].neighbours.clear();
		}
	}

	for (int x = 0; x < _GridSizeX; x++)
	{
		for (int y = 0; y < _GridSizeY; y++)
		{
			if (x > 0) {
				_Nodes[y * _GridSizeX + x].neighbours.push_back(&_Nodes[y * _GridSizeX + (x - 1)]);
			}
			if (x < (_GridSizeX - 1)) {
				_Nodes[y * _GridSizeX + x].neighbours.push_back(&_Nodes[y * _GridSizeX + (x + 1)]);
			}
			if (y > 0) {
				_Nodes[y * _GridSizeX + x].neighbours.push_back(&_Nodes[(y - 1) * _GridSizeX + x]);
			}
			if (y < (_GridSizeY - 1)) {
				_Nodes[y * _GridSizeX + x].neighbours.push_back(&_Nodes[(y + 1) * _GridSizeX + x]);
			}
		}
	}
}

float MapGrid::Distance(Node* a, Node* b)
{
	return sqrtf(((float)a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

float MapGrid::Heuristic(Node* a, Node* b)
{
	return Distance(a, b);
}

std::vector<MapGrid::Node*> MapGrid::Find_AStar_Path()
{
	std::vector<Node*> path;
	Node* current = _Start;
	_Start->localGoal = 0.0f;
	_Start->globalGoal = Heuristic(_Start, _Target);
	// declare a list for non tested _Nodes and put start node to list initially 
	std::list<Node*> _NodesToBeTested;
	_NodesToBeTested.push_back(_Start);

	while (!_NodesToBeTested.empty() && current != _Target) {
		// sort _Nodes to be tested by global value (lowest first!)
		_NodesToBeTested.sort([](const Node* a, const Node* b){ return a->globalGoal < b->globalGoal; });

		// after the sort if the front of the list is already visited remove it
		while (!_NodesToBeTested.empty() && _NodesToBeTested.front()->isVisited) {
			_NodesToBeTested.pop_front();
		}

		// if there is no node to be tested break
		if (_NodesToBeTested.empty()) break;

		current = _NodesToBeTested.front();
		current->isVisited = true;

		// check neighbours of current node
		for (auto neighbourNode : current->neighbours) {
			if (!neighbourNode->isVisited && !neighbourNode->isObstacle) {
				_NodesToBeTested.push_back(neighbourNode);
			}

			if ((current->localGoal + Distance(current, neighbourNode)) < neighbourNode->localGoal) {
				neighbourNode->parent = current;
				neighbourNode->localGoal = current->localGoal + Distance(current, neighbourNode);
				neighbourNode->globalGoal = neighbourNode->localGoal + Heuristic(neighbourNode, _Target);
			}
		}
	}

	// assemble the path from target to start then reverse the vector
	if (_Target->isVisited) {
		MapGrid::Node* pathNode = _Target;
		while (pathNode != _Start) {
			path.push_back(pathNode);
			pathNode = pathNode->parent;
		}
		path.push_back(_Start);
		std::reverse(path.begin(), path.end());
	}

	return path;
}

