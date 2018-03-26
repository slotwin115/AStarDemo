#include "Node.h"
#include "Utilities.h"
using namespace pf;


Node::Node(Point p, NodeType type)
{
	this->coord = p;
	parent = NULL;
	f = h = g = 0;
	this->type = type;

	setSize(sf::Vector2f(resolution, resolution));
	setPosition(sf::Vector2f(coord.x * resolution, coord.y * resolution));
	setOutlineColor(sf::Color::Black);
	setOutlineThickness(-1);

	switch (type)
	{
	case pf::passable:
	{
		setPassable();
		break;
	}
	case pf::obsticle:
	{
		setObsticle();
		break;
	}
	case pf::start:
	{
		setStart();
		break;
	}
	case pf::end:
	{
		setEnd();
		break;
	}
	case pf::onOpenList:
	{
		setOnOpenList();
		break;
	}
	case pf::onClosedList:
	{
		setOnClosedList();
		break;
	}
	default:
	{
		setPassable();
		break;
	}
	}
}

Node::~Node() {}

Point Node::getCoord() const
{
	return coord;
}

Node* Node::getParent() const
{
	return parent;
}

int Node::getCost() const
{
	return f;
}

NodeType Node::getNodeType() const
{
	return type;
}

void Node::setParent(Node *parent)
{
	this->parent = parent;
}

void Node::setDefault()
{
	parent = NULL;
	f = h = g = 0;
	setPassable();
}

void Node::setPassable()
{
	type = passable;
	setFillColor(sf::Color::White);
}

void Node::setObsticle()
{
	type = obsticle;
	setFillColor(sf::Color::Black);
}

void Node::setStart()
{
	type = start;
	setFillColor(sf::Color::Blue);
}

void Node::setEnd()
{
	type = end;
	setFillColor(sf::Color::Red);
}

void Node::setOnOpenList()
{
	type = onOpenList;
	//zmienic na jasny odcien x.
	setFillColor(sf::Color::Green);
}

void Node::setOnClosedList()
{
	type = onClosedList;
	//zmienic na ciemny odcien x.
	setFillColor(sf::Color::Magenta);
}

void Node::computeG(Point Start)
{
	g = distanceEuclidean(this->coord, Start) * 10;
}

void Node::computeH(Point End)
{
	h = distanceEuclidean(this->coord, End) * 10;
}

void Node::computeF()
{
	f = g + h;
}

void Node::computeF(Point Start, Point End)
{
	computeG(Start);
	computeH(End);
	f = g + h;
}

bool pf::operator==(const Node &lv, const Node &rv)
{
	return lv.getCoord() == rv.getCoord();
}

bool pf::operator!=(const Node &lv, const Node &rv)
{
	return !(lv == rv);
}
