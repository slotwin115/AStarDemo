#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Utilities.h"
#include "Node.h"


const int pf::Node::resolution = 10;

void main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "A* algorithm demonstration", sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Font font;
	font.loadFromFile("sansation.ttf");
	sf::Text textToWrite;
	textToWrite.setFont(font);
	textToWrite.setCharacterSize(17);
	textToWrite.setFillColor(sf::Color::White);
	textToWrite.setPosition(0.0, 580.0);
	textToWrite.setString("");

	sf::Vector2i startPosition;
	sf::Vector2i endPosition;
	sf::Vector2i mousePosition;
	bool AStar, isStartSet, isEndSet, multipleRuns = false;
	AStar = isStartSet = isEndSet = false;

	std::vector<std::vector<pf::Node>> grid;
	std::vector<pf::Node *> openList;
	std::vector<pf::Node *> closedList;
	std::vector<pf::Point> directions;
	std::vector<pf::Node *> successors;
	pf::Node *q;
	pf::Point *startPoint = NULL;
	pf::Point *endPoint = NULL;


	pf::Dirs4(directions);
	pf::initGrid(grid, window);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				delete startPoint;
				delete endPoint;
				window.close();
			}
		}

		window.clear(sf::Color::Black);
		if (!AStar)
		{
			while (multipleRuns)
			{
				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						delete startPoint;
						delete endPoint;
						window.close();
						return;
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{
					openList.clear();
					closedList.clear();
					successors.clear();
					q = NULL;
					delete startPoint;
					delete endPoint;
					startPoint = NULL;
					endPoint = NULL;
					isStartSet = false;
					isEndSet = false;
					pf::defaultGrid(grid);
					multipleRuns = false;

					break;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			{
				openList.clear();
				closedList.clear();
				successors.clear();
				q = NULL;
				delete startPoint;
				delete endPoint;
				startPoint = NULL;
				endPoint = NULL;
				isStartSet = false;
				isEndSet = false;
				pf::defaultGrid(grid);
			}

			mousePosition = sf::Mouse::getPosition(window);
			std::cout << "mouse position = (" << mousePosition.x << ", " << mousePosition.y << ")\n";

			if ((mousePosition.x > 0 && mousePosition.x < 800) && (mousePosition.y > 0 && mousePosition.y < 580))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					{
						pf::NodeType previousType = grid[mousePosition.x / pf::Node::resolution][mousePosition.y / pf::Node::resolution].getNodeType();

						startPosition.x = mousePosition.x / pf::Node::resolution;
						startPosition.y = mousePosition.y / pf::Node::resolution;

						if (previousType == pf::NodeType::end)
						{
							endPosition = sf::Vector2i(0, 0);
							isEndSet = false;
						}

						grid[startPosition.x][startPosition.y].setStart();
						isStartSet = true;
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
					{
						pf::NodeType previousType = grid[mousePosition.x / pf::Node::resolution][mousePosition.y / pf::Node::resolution].getNodeType();

						endPosition.x = mousePosition.x / pf::Node::resolution;
						endPosition.y = mousePosition.y / pf::Node::resolution;

						if (previousType == pf::NodeType::start)
						{
							startPosition = sf::Vector2i(0, 0);
							isStartSet = false;
						}

						grid[endPosition.x][endPosition.y].setEnd();
						isEndSet = true;
					}
					else
					{
						pf::NodeType previousType = grid[mousePosition.x / pf::Node::resolution][mousePosition.y / pf::Node::resolution].getNodeType();

						if (previousType == pf::NodeType::start)
						{
							startPosition = sf::Vector2i(0, 0);
							isStartSet = false;
						}
						else if (previousType == pf::NodeType::end)
						{
							endPosition = sf::Vector2i(0, 0);
							isEndSet = false;
						}

						grid[mousePosition.x / pf::Node::resolution][mousePosition.y / pf::Node::resolution].setObsticle();
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
				{
					pf::NodeType tmpType = grid[mousePosition.x / pf::Node::resolution][mousePosition.y / pf::Node::resolution].getNodeType();

					if (tmpType == pf::NodeType::start)
					{
						startPosition = sf::Vector2i(0, 0);
						isStartSet = false;
					}
					else if (tmpType == pf::NodeType::end)
					{
						endPosition = sf::Vector2i(0, 0);
						isEndSet = false;
					}

					grid[mousePosition.x / pf::Node::resolution][mousePosition.y / pf::Node::resolution].setPassable();
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isStartSet && isEndSet)
			{
					startPoint = new pf::Point(startPosition.x, startPosition.y);
					endPoint = new pf::Point(endPosition.x, endPosition.y);
					openList.push_back((&grid[startPosition.x][startPosition.y]));
					AStar = true;
					multipleRuns = true;
			}
		}
		else
		{
			q = pf::leastF(openList);
			pf::removeNode(openList, q);
			pf::generateSuccessors(grid, directions, q, successors);

			for (std::vector<pf::Node *>::iterator sIt = successors.begin(); sIt != successors.end(); sIt++)
			{
				// if successor has the same position as end point
				if ((*sIt)->getCoord() == *endPoint)
				{
					// signal path conclusion, reset path to nice and clear state, then draw path.
					AStar = false;
					pf::passableGrid(grid);
					pf::drawPath(*sIt);
					textToWrite.setString("Press 'C' to clear...");
					break;
				}

				(*sIt)->computeF(*startPoint, *endPoint);

				// if successor isn't on the open list
				auto tmpOpenList = pf::findNode(openList, (*sIt)->getCoord());
				if (tmpOpenList == openList.end())
				{
					// if successor isn't on the closed list
					auto tmpClosedList = pf::findNode(closedList, (*sIt)->getCoord());
					if (tmpClosedList == closedList.end())
					{
						// push successor on the open list.
						(*sIt)->setOnOpenList();
						openList.push_back(*sIt);
					}
				}
				else
				{
					// if successor is in the open list and his heuristics is better or equal
					if (((*tmpOpenList)->getCoord() == (*sIt)->getCoord()) && ((*tmpOpenList)->getCost() >= (*sIt)->getCost()))
					{
						// if successor isn't on the closed list
						auto tmpClosedList = pf::findNode(closedList, (*sIt)->getCoord());
						if (tmpClosedList == closedList.end())
						{
							// replace old successor with new.
							(*sIt)->setOnOpenList();
							pf::replaceNode(openList, **sIt);
						}
					}
				}
			}

			// if path is concluded, do not interfere and change state.
			if(AStar)
				q->setOnClosedList();
			closedList.push_back(q);

			// if algorithm has ended and didn't coclude a path.
			if (openList.empty())
			{
				AStar = false;
				textToWrite.setString("Path not found.     Press 'C' to clear...");
			}
		}

		pf::drawGrid(grid, window);
		window.draw(textToWrite);

		window.display();
	}
}