
#include<vector>
#include <algorithm>
#include "Utilities.h"
#include "Node.h"


float pf::distanceEuclidean(pf::Point start, pf::Point end)
{
	return sqrt(pow(start.x - end.x, 2) + pow(start.y - end.y, 2));
}

int pf::distanceManhattan(pf::Point start, pf::Point end)
{
	return abs(start.x - end.x) + abs(start.y - end.y);
}

void pf::defaultGrid(std::vector<std::vector<pf::Node>> &grid)
{
	std::vector<std::vector<pf::Node>>::iterator G_It;
	std::vector<pf::Node>::iterator g_It;

	for (G_It = grid.begin(); G_It != grid.end(); G_It++)
		for (g_It = G_It->begin(); g_It != G_It->end(); g_It++)
		{
			g_It->setDefault();
			g_It->setPassable();
		}
}

void pf::passableGrid(std::vector<std::vector<pf::Node>> &grid)
{
	std::vector<std::vector<pf::Node>>::iterator G_It;
	std::vector<pf::Node>::iterator g_It;

	for (G_It = grid.begin(); G_It != grid.end(); G_It++)
		for (g_It = G_It->begin(); g_It != G_It->end(); g_It++)
		{
			if(g_It->getNodeType() != obsticle)
				g_It->setPassable();
		}
}

void pf::initGrid(std::vector<std::vector<pf::Node>> &grid, const sf::RenderWindow &windowRes)
{
	std::vector<pf::Node> tmp;

	unsigned int windowResX = windowRes.getSize().x;
	unsigned int windowResY = windowRes.getSize().y - 20;

	for (unsigned int i = 0; i < windowResX / pf::Node::resolution; i++)
	{
		for (unsigned int j = 0; j < windowResY / pf::Node::resolution; j++)
			tmp.push_back(pf::Node(pf::Point(i, j), pf::passable));

		grid.push_back(tmp);
		tmp.clear();
	}
}

pf::Node* pf::leastF(const std::vector<pf::Node*> &list)
{
	return *std::min_element(list.begin(), list.end(), [](pf::Node *a, pf::Node *b)->bool { return a->getCost() < b->getCost(); });
}

void pf::removeNode(std::vector<pf::Node*> &list, const pf::Node *q)
{
	list.erase(std::remove(list.begin(), list.end(), q), list.end());
}

void pf::Dirs8(std::vector<pf::Point> &directions)
{
	directions.push_back(Point(0, -1));
	directions.push_back(Point(0, 1));
	directions.push_back(Point(-1, 0));
	directions.push_back(Point(1, 0));

	directions.push_back(Point(-1, -1));
	directions.push_back(Point(1, 1));
	directions.push_back(Point(-1, 1));
	directions.push_back(Point(1, -1));
}

void pf::Dirs4(std::vector<pf::Point> &directions)
{
	directions.push_back(Point(0, -1));
	directions.push_back(Point(0, 1));
	directions.push_back(Point(-1, 0));
	directions.push_back(Point(1, 0));
}

void pf::generateSuccessors(std::vector<std::vector<pf::Node>> &grid, std::vector<pf::Point> &dirs, pf::Node *q, std::vector<pf::Node *> &successors)
{
	// directions iterator.
	successors.clear();
	std::vector<pf::Point>::iterator dirIt;

	// grid limitations.
	int limitX = grid.size();
	int limitY = grid[0].size();

	// for each direction
	for (dirIt = dirs.begin(); dirIt != dirs.end(); dirIt++)
	{
		// calculate naighbour
		Point tmp(q->getCoord().x + dirIt->x, q->getCoord().y + dirIt->y);

		// if that naighbour is in range of grid
		if ((tmp.x >= 0 && tmp.y >= 0) && (tmp.x < limitX && tmp.y < limitY))
		{
			// check if isn't: obsticle, start and without a parent
			if (grid[tmp.x][tmp.y].getNodeType() != obsticle && grid[tmp.x][tmp.y].getNodeType() != start && grid[tmp.x][tmp.y].getNodeType() != onClosedList && grid[tmp.x][tmp.y].getParent() == NULL)
			{
				// if true, set him a parent and push to successors vector.
				grid[tmp.x][tmp.y].setParent(q);
				successors.push_back(&grid[tmp.x][tmp.y]);
			}
		}
	}
}

std::vector<pf::Node *>::iterator pf::findNode(std::vector<pf::Node *> &vector, const pf::Point &position)
{
	return std::find_if(vector.begin(), vector.end(), [position](pf::Node *n)->bool {return n->getCoord() == position; });
}

void pf::replaceNode(std::vector<pf::Node *> &vector, pf::Node &n)
{
	// n, n because: overloaded operator compares Nodes by coord, but assings normally.
	std::replace(vector.begin(), vector.end(), &n, &n);
}

void pf::drawPath(pf::Node *path)
{
	path->setEnd();
	path = path->getParent();
	while (path != NULL)
	{
		path->setOnOpenList();

		if (path->getParent() == NULL)
			path->setStart();

		path = path->getParent();
	}
}