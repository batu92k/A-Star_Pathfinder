/**
  ******************************************************************************
  * @file    map_grid.h
  * @author  Ali Batuhan KINDAN
  * @date    10.02.2020
  * @brief   This file contains the declaration of methods for A Star Algorithm
  ******************************************************************************
  */

#ifndef MAP_GRID_H
#define MAP_GRID_H

#include<vector>

class MapGrid {
public:
	struct Node {
	public:
		bool isObstacle = false;
		bool isVisited = false;
		float globalGoal = INFINITY;
		float localGoal = INFINITY;
		int x = -1;
		int y = -1;
		std::vector<Node*> neighbours;
		Node* parent = nullptr;
	};

	typedef std::pair<int, int> GridPos;
	typedef std::pair<int, int> GridSize;

	// public funcrtion prototypes
	MapGrid(int x, int y);
	Node* GetTargetNode(void);
	Node* GetStartNode(void);
	GridPos GetTargetPos(void);
	void SetTargetPos(GridPos targetPos);
	GridPos GetStartPos(void);
	void SetStartPos(GridPos startPos);
	void ResetMap();
	void ToggleObstacle(GridPos obstaclePos);
	std::vector<Node*> Find_AStar_Path();
	GridSize GetGridSize(void);
	Node* GetGridArray(void);

private:
	// private variables
	int _GridSizeX = 0;
	int _GridSizeY = 0;
	Node* _Nodes = nullptr;
	Node* _Target = nullptr;
	Node* _Start = nullptr;

	// private function prototypes
	float Distance(Node* a, Node* b);
	float Heuristic(Node* a, Node* b);
};

#endif

