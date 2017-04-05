#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "j1Input.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.assign("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	if(map != nullptr) RELEASE_ARRAY(map);
	if (node_map != nullptr) RELEASE_ARRAY(node_map);
	if (constructible_map_ally != nullptr) RELEASE_ARRAY(constructible_map_ally);
	if (constructible_map_neutral != nullptr) RELEASE_ARRAY(constructible_map_neutral);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	//TODO1
	//create a node_map
	//RELEASE_ARRAY(node_map);
	node_map = new PathNode[width*height];

	memcpy(map, data, width*height);
}
void j1PathFinding::SetConstructibleMaps(uint width, uint height, uchar* data, uchar* data2)
{
	constructible_map_ally = new uchar[width*height];
	memcpy(constructible_map_ally, data, width*height);

	constructible_map_neutral = new uchar[width*height];
	memcpy(constructible_map_neutral, data2, width*height);

}
// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}
bool j1PathFinding::IsConstructible_ally(const iPoint& pos) const
{
	uchar t = GetTileAtConstructible_ally(pos);
	return t != INVALID_WALK_CODE && t > 1;
}
bool j1PathFinding::IsConstructible_neutral(const iPoint& pos) const
{
	uchar t = GetTileAtConstructible_neutral(pos);
	return t != INVALID_WALK_CODE && t > 1;
}
void j1PathFinding::MakeNoConstruible_ally(const iPoint& pos)
{
	constructible_map_ally[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeNoConstruible_neutral(const iPoint& pos)
{
	constructible_map_neutral[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeConstruible_neutral(const iPoint& pos)
{
	constructible_map_neutral[pos.y*width + pos.x] = 10;
}
void j1PathFinding::MakeConstruible_ally(const iPoint& pos)
{
	constructible_map_ally[pos.y*width + pos.x] = 10;
}

void j1PathFinding::MakeNoWalkable(const iPoint& pos)
{
	map[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeWalkable(const iPoint& pos)
{
	map[pos.y*width + pos.x] = 34;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

uchar j1PathFinding::GetTileAtConstructible_ally(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return constructible_map_ally[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}
uchar j1PathFinding::GetTileAtConstructible_neutral(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return constructible_map_neutral[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}


// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(std::list<PathNode*>* list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill->size();
	bool northClose = false, southClose = false, eastClose = false, westClose = false;
	// south
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	else
	{
		southClose = true;
	}
	// north
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	else
	{
		northClose = true;
	}
	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// south-east
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell) && southClose == false && eastClose == false)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// south-west
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell) && southClose == false && westClose == false)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// north-east
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell) && northClose == false && eastClose == false)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	// north-west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell) && northClose == false && westClose == false)
	{
		PathNode* node = App->pathfinding->GetPathNode(cell.x, cell.y);
		if (node->pos != cell) {
			node->parent = this;
			node->pos = cell;
		}
		list_to_fill->push_back(node);
	}
	return list_to_fill->size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
float PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateFopt(const iPoint& destination)
{
	if (parent->pos.DistanceManhattan(pos) == 1) {
		g = parent->g + 11;
	}
	else if (parent->pos.DistanceManhattan(pos) == 2) {
		g = parent->g + 16;
	}
	else {
		g = parent->g + 10;
	}
	h = pos.Distanceh(destination);
	return  g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------

int j1PathFinding::CreatePath(const iPoint & origin, const iPoint & destination, std::list<iPoint>& list)
{
	int size = width*height;
	std::fill(node_map, node_map + size, PathNode(-1, -1, iPoint(-1, -1), nullptr));
	int ret = -1;

	if (IsWalkable(origin) && IsWalkable(destination))
	{
		ret = 1;
		std::priority_queue<PathNode*, std::vector<PathNode*>, compare> open;
		PathNode* firstNode = GetPathNode(origin.x, origin.y);
		firstNode->SetPosition(origin);
		firstNode->g = 0;
		firstNode->h = origin.Distanceh(destination);

		open.push(firstNode);
		PathNode* current = nullptr;
		while (open.size() != 0)
		{
			current = open.top();
			open.top()->on_close = true;
			open.pop();
			if (current->pos == destination)
			{

				std::vector<iPoint>* path = new std::vector<iPoint>;
				last_path.clear();
				for (; current->parent != nullptr; current = GetPathNode(current->parent->pos.x, current->parent->pos.y))
				{
					last_path.push_back(current->pos);
					list.push_front(current->pos);
				}
				list.push_front(current->pos);
				last_path.push_back(current->pos);
				;
				return last_path.size();
			}
			else
			{
				std::list<PathNode*> neighbours;
				current->FindWalkableAdjacents(&neighbours);
				for (std::list<PathNode*>::iterator item = neighbours.begin(); item != neighbours.end(); item++) {
					PathNode* temp = item._Mynode()->_Myval;
					if (temp->on_close == true)
					{
						continue;
					}
					else if (temp->on_open == true)
					{
						int last_g_value = temp->g;
						temp->CalculateFopt(destination);
						if (last_g_value < temp->g)
						{
							temp->parent = GetPathNode(current->pos.x, current->pos.y);
						}
						else {
							temp->g = last_g_value;
						}
					}
					else
					{
						temp->on_open = true;
						temp->CalculateFopt(destination);
						open.push(temp);
					}
				}

				neighbours.clear();
			}
		}
	}
	return -1;
}
float j1PathFinding::CreatePath(const iPoint & origin, const iPoint & destination)
{
	int size = width*height;
	std::fill(node_map, node_map + size, PathNode(-1, -1, iPoint(-1, -1), nullptr));
	int ret = -1;

	if (IsWalkable(origin) && IsWalkable(destination))
	{
		ret = 1;
		std::priority_queue<PathNode*, std::vector<PathNode*>, compare> open;
		PathNode* firstNode = GetPathNode(origin.x, origin.y);
		firstNode->SetPosition(origin);
		firstNode->g = 0;
		firstNode->h = origin.Distanceh(destination);

		open.push(firstNode);
		PathNode* current = nullptr;
		while (open.size() != 0)
		{
			current = open.top();
			open.top()->on_close = true;
			open.pop();
			if (current->pos == destination)
			{

				std::vector<iPoint>* path = new std::vector<iPoint>;
				last_path.clear();
				for (; current->parent != nullptr; current = GetPathNode(current->parent->pos.x, current->parent->pos.y))
				{
					last_path.push_back(current->pos);
				}
				last_path.push_back(current->pos);
				;
				return last_path.size();
			}
			else
			{
				std::list<PathNode*> neighbours;
				current->FindWalkableAdjacents(&neighbours);
				for (std::list<PathNode*>::iterator item = neighbours.begin(); item != neighbours.end(); item++) {
					PathNode* temp = item._Mynode()->_Myval;
					if (temp->on_close == true)
					{
						continue;
					}
					else if (temp->on_open == true)
					{
						int last_g_value = temp->g;
						temp->CalculateFopt(destination);
						if (last_g_value < temp->g)
						{
							temp->parent = GetPathNode(current->pos.x, current->pos.y);
						}
						else {
							temp->g = last_g_value;
						}
					}
					else
					{
						temp->on_open = true;
						temp->CalculateFopt(destination);
						open.push(temp);
					}
				}

				neighbours.clear();
			}
		}
	}
	return -1;
}

PathNode* j1PathFinding::GetPathNode(int x, int y)
{
	return &node_map[(y*width) + x];
}

void PathNode::SetPosition(const iPoint & value)
{
	pos = value;
}