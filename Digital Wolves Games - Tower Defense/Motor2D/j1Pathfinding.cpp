#include "p2Defs.h"
#include "IsoPrimitives.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1EntityManager.h"
#include "QuadTree.h"
#include "Entity.h"
#include "Units.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.assign("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

bool j1PathFinding::Start()
{
	debug_tex = App->tex->Load("maps/path2.png", T_MAP);
	return true;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	last_path.clear();
	if(map != nullptr) RELEASE_ARRAY(map);
	if (node_map != nullptr) RELEASE_ARRAY(node_map);
	if (constructible_map_ally != nullptr) RELEASE_ARRAY(constructible_map_ally);
	if (constructible_map_neutral != nullptr) RELEASE_ARRAY(constructible_map_neutral);
	return true;
}

void j1PathFinding::CleanUpJPS()
{
	for (std::vector<PathNode*>::iterator i = visited.begin(); i != visited.end(); ++i)
		DELETE_PTR(*i)

	visited.clear();
	frontier.clear();
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
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
bool j1PathFinding::CheckBoundaries(const iPoint& tile) const
{
	return (tile.x >= 0 && tile.x <= (int)width &&
		tile.y >= 0 && tile.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& tile) const
{
	uchar t = GetTileAt(tile);
	return t != INVALID_WALK_CODE && t > 0;
}

bool j1PathFinding::IsConstructible_ally(const iPoint& tile) const
{
	uchar t = GetTileAtConstructible_ally(tile);
	return t != INVALID_WALK_CODE && t > 1;
}

bool j1PathFinding::IsConstructible_neutral(const iPoint& tile) const
{
	uchar t = GetTileAtConstructible_neutral(tile);
	return t != INVALID_WALK_CODE && t > 1;
}

void j1PathFinding::MakeNoConstruible_ally(const iPoint& tile)
{
	constructible_map_ally[tile.y*width + tile.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeNoConstruible_neutral(const iPoint& tile)
{
	constructible_map_neutral[tile.y*width + tile.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeConstruible_neutral(const iPoint& tile)
{
	constructible_map_neutral[tile.y*width + tile.x] = 10;
}

void j1PathFinding::MakeConstruible_ally(const iPoint& tile)
{
	constructible_map_ally[tile.y*width + tile.x] = 10;
}

void j1PathFinding::MakeNoWalkable(const iPoint& tile)
{
	map[tile.y*width + tile.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeWalkable(const iPoint& tile)
{
	map[tile.y*width + tile.x] = 34;
}

iPoint j1PathFinding::FindEmptyTile(iPoint from, Elipse collision) const
{
	IsoRect tile;
	iPoint start(from.x - 1, from.y - 1);
	iPoint pos;
	fPoint rect_center;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			pos = iPoint(start.x + i, start.y + j);
			if (IsWalkable(pos))
			{
				rect_center = fPoint(App->map->MapToWorld(pos.x, pos.y).x, App->map->MapToWorld(pos.x, pos.y).y);
				rect_center.x += App->map->data.tile_width / 2.0f;
				rect_center.x += App->map->data.tile_height / 2.0f;
				tile = IsoRect(rect_center, App->map->data.tile_width, App->map->data.tile_height);
				/*if (!tile.Overlaps(collision))
					return pos;*/
			}
		}		 
	return iPoint(-1,-1);
}

iPoint j1PathFinding::FindClosestEmptyAttackTile(iPoint objective_tile, int tile_range, Entity* attacker)
{	
	iPoint ret(-1, -1);

	if (tile_range < 1)
	{
		LOG("Tile range inferior to 1");
		return ret;
	}

	iPoint attacker_tile = attacker->GetPosition();
	std::vector<iPoint> empty_attack_tiles;

	while (tile_range > 0)
	{
		iPoint current_tile(objective_tile.x - tile_range, objective_tile.y - tile_range);

		//Left
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.x = objective_tile.x + i;
			if (IsWalkable(current_tile))
				empty_attack_tiles.push_back(current_tile);
		}
		current_tile.x++;

		//Down
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.y = objective_tile.y + i;
			if (IsWalkable(current_tile))
				empty_attack_tiles.push_back(current_tile);
		}
		current_tile.y++;

		//Right
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.x = objective_tile.x - i;
			if (IsWalkable(current_tile))
				empty_attack_tiles.push_back(current_tile);
		}
		current_tile.x--;

		//Up
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.y = objective_tile.y - i;
			if (IsWalkable(current_tile))
				empty_attack_tiles.push_back(current_tile);
		}
		current_tile.y--;

		tile_range--;
	}
	
	if (empty_attack_tiles.size() == 0)
		return ret;

	float shortest_distance = NUM_TILES;
	float distance_to_tile = 0.0f;

	for(std::vector<iPoint>::iterator it = empty_attack_tiles.begin(); it != empty_attack_tiles.end(); ++it)
	{
		distance_to_tile = it->DistanceTo(attacker_tile);
		if (shortest_distance > distance_to_tile)
		{
			shortest_distance = distance_to_tile;
			ret = *it;
		}
	}
	return ret;
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
const std::vector<iPoint>& j1PathFinding::GetLastPath() const
{
	return last_path;
}

PathNode* j1PathFinding::GetPathNode(int x, int y)
{
	return &node_map[(y*width) + x];
}

const iPoint& j1PathFinding::FindNearestWalkable(const iPoint& position, const iPoint& destination) const
{
	std::vector<iPoint> vector;
	int tile_range = 1;

	while (tile_range < 75)
	{
		iPoint current_tile(destination.x - tile_range, destination.y - tile_range);

		//Left
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.x = destination.x + i;
			if (IsWalkable(current_tile))
				vector.push_back(current_tile);
		}
		current_tile.x++;

		//Down
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.y = destination.y + i;
			if (IsWalkable(current_tile))
				vector.push_back(current_tile);
		}
		current_tile.y++;

		//Right
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.x = destination.x - i;
			if (IsWalkable(current_tile))
				vector.push_back(current_tile);
		}
		current_tile.x--;

		//Up
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.y = destination.y - i;
			if (IsWalkable(current_tile))
				vector.push_back(current_tile);
		}
		current_tile.y--;

		if (vector.size() != 0)
		{
			iPoint lowest = vector.front();
			for (std::vector<iPoint>::iterator it = vector.begin(); it != vector.end(); ++it)
				if (it->DistanceTo(position) < lowest.DistanceTo(position))
					lowest = *it;

			return lowest;
		}

		tile_range++;
	}
}

	 //----------------||----------------||----------------\\
	//----------------||A* + JPS algorithm||----------------\\
   //----------------||____________________||----------------\\

bool j1PathFinding::CalculatePath(const iPoint& start, const iPoint & end, std::vector<iPoint>& vec_to_fill)
{
	CleanUpJPS();
	vec_to_fill.clear();

	float max_destination_cost = App->map->data.height * App->map->data.width;
	origin = new PathNode(0.0f, 0.0f, start, iPoint(-1,-1));
	destination = new PathNode(max_destination_cost, 0.0f, end, iPoint(-2, -2));
	PathNode* check = new PathNode(*origin);

	if (origin->pos == destination->pos)
	{
		LOG("NOT MOVING");
		vec_to_fill.push_back(start);
		return false;
	}

	if (IsWalkable(start))
	{
		visited.push_back(origin);
		visited.push_back(destination);
	}
	else
	{
		LOG("Non-Walkable origin");
		return false;
	}

	if (!IsWalkable(end))
	{
		LOG("Non-Walkable Destination");
		iPoint new_destination = FindNearestWalkable(start, end);
		if(CalculatePath(start, new_destination, vec_to_fill) == false)
			GetPathToClosestReachable(start, end, vec_to_fill);
		return false;
	}

	X_DIRECTION dx = X_NO_DIR;
	Y_DIRECTION dy = Y_NO_DIR;

	OpenOrigin();

	//open all forced neighbours
	ForcedNeighbour forced_neighbour;

	while (GetLowestFN(forced_neighbour))
	{
		if (GetNodeFromVisited(forced_neighbour.after)->cost_so_far >= destination->cost_so_far && destination_reached == true)
			break;

		check = (PathNode*)GetNodeFromVisited(forced_neighbour.after);

		if (forced_neighbour.after.x - forced_neighbour.before.x > 0)
			dx = X_RIGHT;
		else if (forced_neighbour.after.x - forced_neighbour.before.x < 0)
			dx = X_LEFT;

		if (forced_neighbour.after.y - forced_neighbour.before.y > 0)
			dy = Y_DOWN;
		else if (forced_neighbour.after.y - forced_neighbour.before.y < 0)
			dy = Y_UP;	

		if (CheckForTiles(check, dx, dy))
			destination_reached = true;
	}

	if (destination_reached)
		FillPathVec(vec_to_fill);

	CleanUpJPS();

	if (destination_reached)
	{
		destination_reached = false;
		return true;
	}

	GetPathToClosestReachable(start, end, vec_to_fill);
	return false;
}

void j1PathFinding::CalculateCloseCombatFightPaths(Unit* first, Unit* second)
{
	fPoint vector(second->GetPixelPosition().x - first->GetPixelPosition().x, second->GetPixelPosition().y - first->GetPixelPosition().y);
	float speed_ratio = (first->GetSpeed() / (first->GetSpeed() + second->GetSpeed()));
	float x_collision_point = first->GetPixelPosition().x + vector.x * speed_ratio;
	float y_collision_point = first->GetPixelPosition().y + vector.y * speed_ratio;

	std::vector<PointPair> pairs;

	App->pathfinding->MakeWalkable(first->GetPosition());
	App->pathfinding->MakeWalkable(second->GetPosition());

	GetClosestCloseCombatFightPosition(fPoint(x_collision_point, y_collision_point), pairs);

	App->pathfinding->MakeNoWalkable(first->GetPosition());
	App->pathfinding->MakeNoWalkable(second->GetPosition());

	PointPair best_pair(pairs.front());

	for (std::vector<PointPair>::iterator it = pairs.begin(); it != pairs.end(); ++it)
	{
		if (*it != best_pair)
		{
			fPoint first_vec(it->first.x - first->GetPosition().x, it->first.y - first->GetPosition().y);
			fPoint second_vec(it->second.x - second->GetPosition().x, it->second.y - second->GetPosition().y);

			float distanse_to_first = sqrt(first_vec.x * first_vec.x + first_vec.y * first_vec.y);
			float distanse_to_second = sqrt(second_vec.x * second_vec.x + second_vec.y * second_vec.y);

			fPoint best_first_vec(best_pair.first.x - first->GetPosition().x, best_pair.first.y - first->GetPosition().y);
			fPoint best_second_vec(best_pair.second.x - second->GetPosition().x, best_pair.second.y - second->GetPosition().y);

			float best_distanse_to_first = sqrt(best_first_vec.x * best_first_vec.x + best_first_vec.y * best_first_vec.y);
			float best_distanse_to_second = sqrt(best_second_vec.x * best_second_vec.x + best_second_vec.y * best_second_vec.y);

			if (distanse_to_first < best_distanse_to_first && distanse_to_second <= best_distanse_to_second)
				best_pair = *it;
			if (distanse_to_first <= best_distanse_to_first && distanse_to_second < best_distanse_to_second)
				best_pair = *it;

			first_vec.x = it->second.x - first->GetPosition().x;
			first_vec.y = it->second.y - first->GetPosition().y;
			second_vec.x = it->first.x - second->GetPosition().x;
			second_vec.y = it->first.y - second->GetPosition().y;

			distanse_to_first = sqrt(first_vec.x * first_vec.x + first_vec.y * first_vec.y);
			distanse_to_second = sqrt(second_vec.x * second_vec.x + second_vec.y * second_vec.y);

			best_first_vec.x = best_pair.second.x - first->GetPosition().x;
			best_first_vec.y = best_pair.second.y - first->GetPosition().y;
			best_second_vec.x = best_pair.first.x - second->GetPosition().x;
			best_second_vec.y = best_pair.first.y - second->GetPosition().y;

			best_distanse_to_first = sqrt(best_first_vec.x * best_first_vec.x + best_first_vec.y * best_first_vec.y);
			best_distanse_to_second = sqrt(best_second_vec.x * best_second_vec.x + best_second_vec.y * best_second_vec.y);

			if (distanse_to_first < best_distanse_to_first && distanse_to_second <= best_distanse_to_second)
				best_pair = *it;
			if (distanse_to_first <= best_distanse_to_first && distanse_to_second < best_distanse_to_second)
				best_pair = *it;
		}
	}

	if (best_pair.first.DistanceTo(first->GetPosition()) <= best_pair.second.DistanceTo(first->GetPosition()))
	{
		first->GoTo(best_pair.first);
		second->GoTo(best_pair.second);
	}
	else
	{
		first->GoTo(best_pair.second);
		second->GoTo(best_pair.first);
	}
}

bool j1PathFinding::CalculateAttackPath(const iPoint & start, const iPoint & end, std::vector<iPoint>& vec_to_fill)
{
	CleanUpJPS();
	vec_to_fill.clear();

	float max_destination_cost = App->map->data.height * App->map->data.width;
	origin = new PathNode(0.0f, 0.0f, start, iPoint(-1, -1));
	destination = new PathNode(max_destination_cost, 0.0f, end, iPoint(-2, -2));
	PathNode* check = new PathNode(*origin);

	if (origin->pos == destination->pos)
	{
		LOG("NOT MOVING");
		return false;
	}

	if (IsWalkable(start))
	{
		visited.push_back(origin);
		visited.push_back(destination);
	}
	else
	{
		LOG("Non-Walkable origin");
		return false;
	}

	if (!IsWalkable(end))
	{
		LOG("Non-Walkable Destination");
		return false;
	}

	X_DIRECTION dx = X_NO_DIR;
	Y_DIRECTION dy = Y_NO_DIR;

	OpenOrigin();

	//open all forced neighbours
	ForcedNeighbour forced_neighbour;

	while (GetLowestFN(forced_neighbour))
	{
		if (GetNodeFromVisited(forced_neighbour.after)->cost_so_far >= destination->cost_so_far && destination_reached == true)
			break;

		check = (PathNode*)GetNodeFromVisited(forced_neighbour.after);

		if (forced_neighbour.after.x - forced_neighbour.before.x > 0)
			dx = X_RIGHT;
		else if (forced_neighbour.after.x - forced_neighbour.before.x < 0)
			dx = X_LEFT;

		if (forced_neighbour.after.y - forced_neighbour.before.y > 0)
			dy = Y_DOWN;
		else if (forced_neighbour.after.y - forced_neighbour.before.y < 0)
			dy = Y_UP;

		if (CheckForTiles(check, dx, dy))
			destination_reached = true;
	}

	if (destination_reached)
		FillPathVec(vec_to_fill);

	CleanUpJPS();

	if (destination_reached)
	{
		destination_reached = false;
		return true;
	}
	return false;
}

bool j1PathFinding::CheckForTiles(const PathNode* start, X_DIRECTION dx, Y_DIRECTION dy) //true when destination reached
{
	bool diagonal_end = false;
	PathNode* diagonal = new PathNode(*start);
	diagonal->parent = start->pos;

	while (diagonal_end == false)
	{
		//initial pos check
		if (diagonal->pos == start->pos)
		{
			if (CheckForForcedNeighbours(start, dx, dy))
				return true;
		}
		else
			if (CheckForForcedNeighbours(diagonal, dx, dy))
				return true;
		//X
		if (diagonal->pos == start->pos)
		{
			if (CheckX(start, dx) == true)
				return true;
		}
		else
			if(CheckX(diagonal, dx) == true)
				return true;
		//Y
		if (diagonal->pos == start->pos)
		{
			if (CheckY(start, dy) == true)
				return true;
		}
		else
			if (CheckY(diagonal, dy) == true)
				return true;
		//Diagonal
		MoveDiagonal(diagonal, dx, dy, diagonal_end);
	
	}
	DELETE_PTR(diagonal);
	return false;
}

void j1PathFinding::CheckForTilesAndStore(const PathNode * start, X_DIRECTION dx, Y_DIRECTION dy, std::vector<PathNode>& vec)
{
	bool diagonal_end = false;
	PathNode* diagonal = new PathNode(*start);
	diagonal->parent = start->pos;
	vec.push_back(*start);

	while (diagonal_end == false)
	{
		//initial pos check
		if (diagonal->pos == start->pos)
			CheckForForcedNeighbours(start, dx, dy);
		else
			CheckForForcedNeighbours(diagonal, dx, dy);
		//X
		if (diagonal->pos == start->pos)
			CheckXAndStore(start, dx, vec);
		else
			CheckXAndStore(diagonal, dx, vec);
		//Y
		if (diagonal->pos == start->pos)
			CheckYAndStore(start, dy, vec);
		else
			CheckYAndStore(diagonal, dy, vec);
		//Diagonal
		MoveDiagonalAndStore(diagonal, dx, dy, diagonal_end, vec);

	}
	DELETE_PTR(diagonal);
}

void j1PathFinding::GetPathToClosestReachable(const iPoint& start,const iPoint& end, std::vector<iPoint>& vec_to_fill)
{
	std::vector<PathNode> reachable_tiles;
	CleanUpJPS();

	float max_destination_cost = App->map->data.height * App->map->data.width;
	origin = new PathNode(0.0f, 0.0f, start, iPoint(-1, -1));
	destination = new PathNode(max_destination_cost, 0.0f, end, iPoint(-2, -2));
	PathNode* check = new PathNode(*origin);

	if (origin->pos == destination->pos)
	{
		LOG("NOT MOVING");
		vec_to_fill.push_back(start);
		return;
	}

	if (IsWalkable(start))
	{
		visited.push_back(origin);
		visited.push_back(destination);
	}
	else
	{
		LOG("Non-Walkable origin");
		return;
	}

	X_DIRECTION dx = X_NO_DIR;
	Y_DIRECTION dy = Y_NO_DIR;

	CheckForTilesAndStore(origin, X_RIGHT, Y_DOWN, reachable_tiles);
	CheckForTilesAndStore(origin, X_RIGHT, Y_UP, reachable_tiles);
	CheckForTilesAndStore(origin, X_LEFT, Y_DOWN, reachable_tiles);
	CheckForTilesAndStore(origin, X_LEFT, Y_UP, reachable_tiles);

	//open all forced neighbours
	ForcedNeighbour forced_neighbour;

	while (GetLowestFN(forced_neighbour))
	{
		if (GetNodeFromVisited(forced_neighbour.after)->cost_so_far >= destination->cost_so_far && destination_reached == true)
			break;

		check = (PathNode*)GetNodeFromVisited(forced_neighbour.after);

		if (forced_neighbour.after.x - forced_neighbour.before.x > 0)
			dx = X_RIGHT;
		else if (forced_neighbour.after.x - forced_neighbour.before.x < 0)
			dx = X_LEFT;

		if (forced_neighbour.after.y - forced_neighbour.before.y > 0)
			dy = Y_DOWN;
		else if (forced_neighbour.after.y - forced_neighbour.before.y < 0)
			dy = Y_UP;

		CheckForTilesAndStore(check, dx, dy, reachable_tiles);
	}

	//find closesr reachable and make it be destination
	PathNode new_destination = reachable_tiles[1];
	for (std::vector<PathNode>::iterator it = reachable_tiles.begin(); it != reachable_tiles.end(); ++it)
		if (it->distance == new_destination.distance && it->priority < new_destination.priority && it->pos != origin->pos)
			new_destination = *it;
		else if (it->distance < new_destination.distance && it->pos != origin->pos)
			new_destination = *it;

	destination = &new_destination;

	FillPathVec(vec_to_fill);

	CleanUpJPS();
}

const PathNode* j1PathFinding::PushToVisited(const PathNode* node)
{	
	PathNode* ret = nullptr;
	bool found = false;

	for (std::vector<PathNode*>::iterator it = visited.begin(); it != visited.end(); ++it)
	{
		if ((*it)->pos == node->pos)
		{
			if (node->cost_so_far < (*it)->cost_so_far && node->pos != origin->pos) //put = for debug
			{
				(*it)->parent = node->parent;
				ChangeCosts(it, node->cost_so_far);	
			}
			found = true;
			ret = *it;
			break;
		}
	}

	if (found == false)
	{	
		PathNode* copy = new PathNode(*node);
		visited.push_back(copy);
		ret = copy;
	}
	return (const PathNode*)ret;
}

void j1PathFinding::PushToFrontier(ForcedNeighbour & fn)
{
	bool found = false;

	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); it++)
		if (fn.before == it->before && fn.after == it->after)
			found = true;

	if (!found)
		frontier.push_back(fn);
}

bool j1PathFinding::FoundForcedNeighbour(const PathNode* before, iPoint after_pos,const PathNode* path_to)
{	
	PushToVisited(path_to);

	PathNode* after = new PathNode();
	after->pos = after_pos;
	after->parent = before->pos;
	after->cost_so_far = before->cost_so_far + DIAGONAL_COST;
	after->CalculatePriority(destination->pos);

	PushToVisited(before);
	PushToVisited(after);

	ForcedNeighbour corner(before->pos, after->pos, after->priority);

	PushToFrontier(corner);

	if (after->pos == destination->pos)
	{
		DELETE_PTR(after);
		return true;
	}
	DELETE_PTR(after);
	return false;
}

bool j1PathFinding::FoundForcedNeighbour(const PathNode* before, iPoint after_pos)
{
	PathNode* after = new PathNode();
	after->pos = after_pos;
	after->parent = before->pos;
	after->cost_so_far = before->cost_so_far + DIAGONAL_COST;
	after->CalculatePriority(destination->pos);

	PushToVisited(before);
	PushToVisited(after);
	
	ForcedNeighbour corner(before->pos, after->pos, after->priority);

	PushToFrontier(corner);

	if (after->pos == destination->pos)
	{
		DELETE_PTR(after);
		return true;
	}
	DELETE_PTR(after);
	return false;
}

bool j1PathFinding::CheckX(const PathNode* start, X_DIRECTION dir)
{
	PathNode* line = new PathNode(start->cost_so_far, start->distance, start->pos, start->parent);
	line->parent = start->pos;

	if (dir == X_RIGHT)
		while (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)))
		{
			line->pos.x++;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);

			//Check destination
			if (line->pos == destination->pos)
			{
				DestinationReached(line, start);
				DELETE_PTR(line);
				return true;
			}

			//UP
			if (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y - 1)) == true)
					if (FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y - 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
			//DOWN
			if (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y + 1)) == true)
					if(FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y + 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
		}

	if (dir == X_LEFT)
		while (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)))
		{
			line->pos.x--;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);

			if (line->pos == destination->pos)
			{
				DestinationReached(line, start);
				DELETE_PTR(line);
				return true;
			}

			//UP
			if (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y - 1)) == true)
					if(FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y - 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
			//DOWN
			if (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y + 1)) == true)
					if(FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y + 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
		}
	DELETE_PTR(line);
	return false;
}

void j1PathFinding::CheckXAndStore(const PathNode * start, X_DIRECTION dir, std::vector<PathNode>& vec)
{
	PathNode* line = new PathNode(start->cost_so_far, start->distance, start->pos, start->parent);
	line->parent = start->pos;

	if (dir == X_RIGHT)
		while (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)))
		{
			line->pos.x++;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);
			vec.push_back(*line);

			//UP
			if (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y - 1)) == true)
					FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y - 1), start);

			//DOWN
			if (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y + 1)) == true)
					FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y + 1), start);
		}

	if (dir == X_LEFT)
		while (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)))
		{
			line->pos.x--;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);
			vec.push_back(*line);

			//UP
			if (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y - 1)) == true)
					FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y - 1), start);

			//DOWN
			if (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y + 1)) == true)
					FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y + 1), start);
		}
	DELETE_PTR(line);
}

bool j1PathFinding::CheckY(const PathNode* start, Y_DIRECTION dir)
{
	PathNode* line = new PathNode(*start);
	line->parent = start->pos;

	if (dir == Y_DOWN)
		while (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)))
		{
			line->pos.y++;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);

			if (line->pos == destination->pos)
			{
				DestinationReached(line, start);
				DELETE_PTR(line);
				return true;
			}

			//Left
			if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y + 1)) == true)
					if(FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y + 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
			//Right
			if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y + 1)) == true)
					if(FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y + 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
		}

	//Up
	if (dir == Y_UP)
		while (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)))
		{
			line->pos.y--;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);

			if (line->pos == destination->pos)
			{
				DestinationReached(line, start);
				DELETE_PTR(line);
				return true;
			}

			//Left
			if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y - 1)) == true)
					if(FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y - 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
			//Right
			if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y - 1)) == true)
					if(FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y - 1), start))
					{
						DELETE_PTR(line);
						return true;
					}
		}
	DELETE_PTR(line);
	return false;
}

void j1PathFinding::CheckYAndStore(const PathNode * start, Y_DIRECTION dir, std::vector<PathNode>& vec)
{
	PathNode* line = new PathNode(*start);
	line->parent = start->pos;

	if (dir == Y_DOWN)
		while (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)))
		{
			line->pos.y++;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);
			vec.push_back(*line);

			//Left
			if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y + 1)) == true)
					FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y + 1), start);
					
			//Right
			if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y + 1)) == true)
					FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y + 1), start);
		}

	//Up
	if (dir == Y_UP)
		while (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)))
		{
			line->pos.y--;
			line->cost_so_far += STRAIGHT_COST;
			line->CalculatePriority(destination->pos);
			vec.push_back(*line);

				//Left
				if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)) == false)
					if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y - 1)) == true)
						FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y - 1), start);
					
			//Right
			if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)) == false)
				if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y - 1)) == true)
					FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y - 1), start);
		}
	DELETE_PTR(line);
}

void j1PathFinding::MoveDiagonal(PathNode* diagonal, X_DIRECTION dx, Y_DIRECTION dy, bool& diagonal_end)
{
	if (dx == X_RIGHT && dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1)))
		{
			diagonal->pos.x++;
			diagonal->pos.y++;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT && dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1)))
		{
			diagonal->pos.x--;
			diagonal->pos.y--;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_RIGHT && dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1)))
		{
			diagonal->pos.x++;
			diagonal->pos.y--;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT && dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1)))
		{
			diagonal->pos.x--;
			diagonal->pos.y++;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y)))
		{
			diagonal->pos.x--;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dx == X_RIGHT)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y)))
		{
			diagonal->pos.x++;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;

	else if (dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y - 1)))
		{
			diagonal->pos.y--;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;
			
	else if (dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y + 1)))
		{
			diagonal->pos.y++;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
		}
		else
			diagonal_end = true;
}

void j1PathFinding::MoveDiagonalAndStore(PathNode * diagonal, X_DIRECTION dx, Y_DIRECTION dy, bool & diagonal_end, std::vector<PathNode>& vec)
{
	if (dx == X_RIGHT && dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1)))
		{
			diagonal->pos.x++;
			diagonal->pos.y++;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
			vec.push_back(*diagonal);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT && dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1)))
		{
			diagonal->pos.x--;
			diagonal->pos.y--;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
			vec.push_back(*diagonal);
		}
		else
			diagonal_end = true;

	else if (dx == X_RIGHT && dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1)))
		{
			diagonal->pos.x++;
			diagonal->pos.y--;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
			vec.push_back(*diagonal);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT && dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1)))
		{
			diagonal->pos.x--;
			diagonal->pos.y++;
			diagonal->cost_so_far += DIAGONAL_COST;
			diagonal->CalculatePriority(destination->pos);
			vec.push_back(*diagonal);
		}
		else
			diagonal_end = true;

	else if (dx == X_LEFT)
		if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y)))
		{
			diagonal->pos.x--;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
			vec.push_back(*diagonal);
		}
		else
			diagonal_end = true;

	else if (dx == X_RIGHT)
		if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y)))
		{
			diagonal->pos.x++;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
			vec.push_back(*diagonal);
		}
		else
			diagonal_end = true;

	else if (dy == Y_UP)
		if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y - 1)))
		{
			diagonal->pos.y--;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
			vec.push_back(*diagonal);
		}
		else
			diagonal_end = true;

	else if (dy == Y_DOWN)
		if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y + 1)))
		{
			diagonal->pos.y++;
			diagonal->cost_so_far += STRAIGHT_COST;
			diagonal->CalculatePriority(destination->pos);
			vec.push_back(*diagonal);
		}
		else
			diagonal_end = true;
}

bool j1PathFinding::GetLowestFN(ForcedNeighbour& fn)
{
	bool ret = false;
	bool all_opened = true;
	fn = frontier.front();

	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
		if ((it->opened == false))
		{
			fn = *it;
			all_opened = false;
			break;
		}

	if (all_opened)
		return false;

	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
		if (it->opened == false && it->priority < fn.priority)
			fn = *it;

	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
		if (it->after == fn.after && it->before == fn.before && it->opened == false)
		{
			it->opened = true;
			return true;
		}
	return false;
}

const bool j1PathFinding::FrontierFinished() const
{
	if (frontier.empty())
		return true;

	for (std::list<ForcedNeighbour>::const_iterator i = frontier.begin(); i != frontier.end(); ++i)
		if (i->opened != true)
			return false;

	return true;
}

void j1PathFinding::FillPathVec(std::vector<iPoint>& vec)
{
	vec.clear();

	const PathNode* it = destination;

	while (it->parent != origin->parent)
	{
		vec.push_back(it->pos);
		it = GetNodeFromVisited(it->parent);
	}

	vec.push_back(origin->pos);

	last_path = vec;
}

void j1PathFinding::ChangeCosts(std::vector<PathNode*>::iterator from, float new_cost)
{	
	float difference = (*from)->cost_so_far - new_cost;

	(*from)->cost_so_far = new_cost;
	(*from)->CalculatePriority(destination->pos);
	
	for (std::vector<PathNode*>::iterator it = visited.begin(); it != visited.end(); ++it)
		if ((*it)->parent == (*from)->pos)
		{
			float cost = (*it)->cost_so_far - difference;
			ChangeCosts(it, cost);
		}
}

void j1PathFinding::OpenOrigin()
{
	//open origin in 4 diagonals
	if (CheckForTiles(origin, X_RIGHT, Y_DOWN) == true
		|| CheckForTiles(origin, X_RIGHT, Y_UP) == true
		|| CheckForTiles(origin, X_LEFT, Y_DOWN) == true
		|| CheckForTiles(origin, X_LEFT, Y_UP) == true)
		destination_reached = true;
}

const PathNode * j1PathFinding::GetNodeFromVisited(const iPoint & pos)
{
	for (std::vector<PathNode*>::iterator it = visited.begin(); it != visited.end(); ++it)
	{
		if ((*it)->pos == pos)
			return *it;
	}
	return nullptr;
}

void j1PathFinding::GetClosestCloseCombatFightPosition(const fPoint & pixel_pos, std::vector<PointPair>& pairs)
{
	iPoint start = App->map->WorldToMap(pixel_pos.x, pixel_pos.y);

	if (IsWalkable(start))
		AdjacentWalkable(start, pairs);
	else
	{
		int range = 0;

		while (pairs.size() == 0)
		{
			start.x--;
			start.y--;
			range++;

			for (int i = 0; i < range* 2; i++)
			{
				start.x++;
				AdjacentWalkable(start, pairs);
			}

			for (int i = 0; i < range * 2; i++)
			{
				start.y++;
				AdjacentWalkable(start, pairs);
			}

			for (int i = 0; i < range * 2; i++)
			{
				start.x--;
				AdjacentWalkable(start, pairs);
			}

			for (int i = 0; i < range * 2; i++)
			{
				start.y--;
				AdjacentWalkable(start, pairs);
			}
		}
	}
}

void j1PathFinding::AdjacentWalkable(const iPoint & start, std::vector<PointPair>& pairs) const
{
	iPoint pair = start;
	pair.x--;
	pair.y--;

	for (int i = 0; i < 2; i++)
	{
		pair.x++;
		if (IsWalkable(pair))
			pairs.push_back(PointPair(start,pair));
	}

	for (int i = 0; i < 2; i++)
	{
		pair.y++;
		if (IsWalkable(pair))
			pairs.push_back(PointPair(start, pair));
	}

	for (int i = 0; i < 2; i++)
	{
		pair.x--;
		if (IsWalkable(pair))
			pairs.push_back(PointPair(start, pair));
	}

	for (int i = 0; i < 2; i++)
	{
		pair.y--;
		if (IsWalkable(pair))
			pairs.push_back(PointPair(start, pair));
	}
}

void j1PathFinding::DestinationReached(const PathNode* destination,const PathNode* path_to)
{	
	PushToVisited(path_to);
	PushToVisited(destination);
}

void j1PathFinding::DestinationReached(const PathNode* destination)
{
	PushToVisited(destination);
}

bool j1PathFinding::CheckForForcedNeighbours(const PathNode * node, X_DIRECTION dx, Y_DIRECTION dy)
{
	//Check if in Destination
	if (node->pos == destination->pos)
	{
		DestinationReached(node);
		return true;
	}

	if (dx == X_LEFT)
	{
		//up
		if (IsWalkable(iPoint(node->pos.x, node->pos.y - 1)) == false)
			if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y - 1)))
				if(FoundForcedNeighbour(node, iPoint(node->pos.x - 1, node->pos.y - 1)))
					return true;
		//down
		if (IsWalkable(iPoint(node->pos.x, node->pos.y + 1)) == false)
			if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y + 1)))
				if(FoundForcedNeighbour(node, iPoint(node->pos.x - 1, node->pos.y + 1)))
					return true;
	}
	if (dx == X_RIGHT)
	{
		//up
		if (IsWalkable(iPoint(node->pos.x, node->pos.y - 1)) == false)
			if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y - 1)))
				if(FoundForcedNeighbour(node, iPoint(node->pos.x + 1, node->pos.y - 1)))
					return true;
		//down
		if (IsWalkable(iPoint(node->pos.x, node->pos.y + 1)) == false)
			if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y + 1)))
				if(FoundForcedNeighbour(node, iPoint(node->pos.x + 1, node->pos.y + 1)))
					return true;
	}
	if (dy == Y_UP)
	{
		//right
		if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y)) == false)
			if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y - 1)))
				if(FoundForcedNeighbour(node, iPoint(node->pos.x + 1, node->pos.y - 1)))
					return true;
		//left
		if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y)) == false)
			if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y - 1)))
				if(FoundForcedNeighbour(node, iPoint(node->pos.x - 1, node->pos.y - 1)))
					return true;
	}
	if (dy == Y_DOWN)
	{
		//right
		if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y)) == false)
			if (IsWalkable(iPoint(node->pos.x + 1, node->pos.y + 1)))
				if(FoundForcedNeighbour(node, iPoint(node->pos.x + 1, node->pos.y + 1)))
					return true;
		//left
		if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y)) == false)
			if (IsWalkable(iPoint(node->pos.x - 1, node->pos.y + 1)))
				if(FoundForcedNeighbour(node, iPoint(node->pos.x - 1, node->pos.y + 1)))
					return true;
	}
	return false;
}

void j1PathFinding::DeleteIfNotPushed(PathNode *& ptr)
{
	bool in_vec = false;

	for (int i = 0; i < visited.size(); i++)
		if (ptr == visited[i])
		{
			in_vec = true;
			break;
		}

	if (!in_vec)
	{
		delete ptr;
		ptr = nullptr;
	}
}

void j1PathFinding::AddPath(std::vector<iPoint>* path)
{
	allied_paths.push_back(path);
}

iPoint j1PathFinding::FindNearestEmpty(Unit* unit) const
{
	iPoint start_tile = App->map->WorldToMap(unit->GetPosition().x, unit->GetPosition().y);
	int tile_range = 1;

	while (tile_range < 75)
	{
		iPoint current_tile(start_tile.x - tile_range, start_tile.y - tile_range);

		//Left
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.x = start_tile.x + i;
			if (IsWalkable(current_tile))
				return current_tile;
		}
		current_tile.x++;

		//Down
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.y = start_tile.y + i;
			if (IsWalkable(current_tile))
				return current_tile;
		}
		current_tile.y++;

		//Right
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.x = start_tile.x - i;
			if (IsWalkable(current_tile))
				return current_tile;
		}
		current_tile.x--;

		//Up
		for (int i = -tile_range; i < tile_range; i++)
		{
			current_tile.y = start_tile.y - i;
			if (IsWalkable(current_tile))
				return current_tile;
		}
		current_tile.y--;

		tile_range++;
	}
}

bool j1PathFinding::IsInPath(const iPoint & tile, const std::vector<iPoint>& path) const
{
	X_DIRECTION x_dir = X_NO_DIR;
	Y_DIRECTION y_dir = Y_NO_DIR;

	for (int i = 0; i < path.size() - 1; i++)
	{
		if (path[i + 1].x - path[i].x > 0)
			x_dir = X_LEFT;
		else if(path[i + 1].x - path[i].x < 0)
			x_dir = X_RIGHT;
		else if (path[i + 1].x - path[i].x == 0)
			x_dir = X_NO_DIR;

		if (path[i + 1].y - path[i].y > 0)
			y_dir = Y_DOWN;
		else if (path[i + 1].y - path[i].y < 0)
			y_dir = Y_UP;
		else if (path[i + 1].y - path[i].y == 0)
			y_dir = Y_NO_DIR;

		iPoint current_tile(path[i]);

		while (current_tile != path[i + 1])
		{
			if (current_tile == tile)
				return true;

			if (x_dir == X_LEFT)
				current_tile.x++;
			if (x_dir == X_RIGHT)
				current_tile.x--;

			if (y_dir == Y_DOWN)
				current_tile.y++;
			if (y_dir == Y_UP)
				current_tile.y--;
		}
	}
	return false;
}

void j1PathFinding::Debug()
{
	iPoint pos;

	for (std::vector<iPoint>::const_iterator item = last_path.begin();
		item != last_path.end();
		++item)
	{
		pos = App->map->MapToWorld(item->x, item->y);
		App->render->PushInGameSprite(debug_tex, pos.x - App->map->data.tile_width / 2.0f, pos.y);
	}
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------

PathNode::PathNode() : cost_so_far(0.0f), distance(0.0f), priority(0.0f), pos(-1, -1), parent(iPoint(0,0))
{}

PathNode::PathNode(iPoint pos): cost_so_far(0.0f), distance(0.0f), priority(0.0f), pos(pos), parent(iPoint(0, 0))
{}

PathNode::PathNode(float cost, float dist, const iPoint& pos,iPoint parent) : cost_so_far(cost), distance(dist), priority(cost + dist), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : cost_so_far(node.cost_so_far), distance(node.distance), priority(node.priority), pos(node.pos), parent(node.parent)
{}

PathNode::~PathNode()
{}

float PathNode::CalculateDistance(iPoint destination)
{
	float sr = (destination.x - pos.x) * (destination.x - pos.x) + (destination.y - pos.y) * (destination.y - pos.y);
	distance = sqrtf(sr);
	return distance;
}

float PathNode::CalculatePriority(const iPoint& destination)
{	
	priority = CalculateDistance(destination) + cost_so_far;
	return priority;
}

const PathNode & PathNode::operator=(const PathNode & node)
{
	this->cost_so_far = node.cost_so_far;
	this->distance = node.distance;
	this->parent = node.parent;
	this->pos = node.pos;

	return *this;
}

inline bool PathNode::operator>(const PathNode & rhs) const
{
	if (this->priority > rhs.priority)
		return true;
	return false;
}

inline bool PathNode::operator<(const PathNode & rhs) const
{
	if (this->distance + this->cost_so_far < rhs.cost_so_far + rhs.distance)
		return true;
	return false;
}

inline bool PathNode::operator==(const PathNode & rhs) const
{
	if (this->distance == rhs.distance && this->cost_so_far == rhs.cost_so_far && this->parent == rhs.parent && this->pos == rhs.pos)
		return true;
	return false;
}

//ForcedNeighbour
ForcedNeighbour::ForcedNeighbour(const iPoint& before, const iPoint& after, float priority): before(before), after(after), opened(false), priority(priority)
{}

ForcedNeighbour::ForcedNeighbour(): before(iPoint(0,0)), after(iPoint(0, 0)), opened(false), priority(0.0f)
{}

const ForcedNeighbour & ForcedNeighbour::operator=(const ForcedNeighbour & rhs)
{
	before = rhs.before;
	after = rhs.after;
	opened = rhs.opened;
	priority = rhs.priority;

	return *this;
}

PointPair::PointPair(): first(iPoint(-1,-1)), second(iPoint(-1, -1))
{}

PointPair::PointPair(const iPoint & first, const iPoint & second): first(first), second(second)
{}

PointPair::PointPair(const PointPair & p): first(p.first), second(p.second)
{}

bool PointPair::operator==(const PointPair & p) const
{
	return ((first == p.first || first == p.second) && (second == p.first || second == p.second));
}

bool PointPair::operator!=(const PointPair & p) const
{
	return ((first != p.first && first != p.second) || (second != p.first && second != p.second));
}

const PointPair & PointPair::operator=(const PointPair & p)
{
	first = p.first;
	second = p.second;
	return *this;
}
