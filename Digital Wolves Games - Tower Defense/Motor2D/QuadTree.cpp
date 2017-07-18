#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "j1Map.h"
#include "IsoPrimitives.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "QuadTree.h"


QuadTreeNode::QuadTreeNode(const iPoint& tile_pos, const uint x_tiles, const uint tile_width, const uint y_tiles, const uint tile_height, QuadTreeNode * parent): parent(parent)
{
	area = new TiledIsoRect(tile_pos, x_tiles, tile_width, y_tiles, tile_height);

	for (int i = 0; i < 4; i++)
		childs[i] = nullptr;

	for (int i = 0; i < NODE_ENTITIES; i++)
		entities[i] = nullptr;

}

QuadTreeNode::~QuadTreeNode()
{
	for (int i = 0; i < 4; i++)
		if (childs[i] != nullptr)
		{
			DELETE_PTR(childs[i]);
		}
		else
			break;

	for (int i = 0; i < NODE_ENTITIES; i++)
		if(entities[i] != nullptr)
		{
			DELETE_PTR(entities[i]);
		}
		else
			break;

	delete area;
}

bool QuadTreeNode::AddEntity(Entity * entity)
{
	bool ret = false;

	if (childs[0] == nullptr)
	{
		if (entities[NODE_ENTITIES - 1] != nullptr)
		{
			ret = SubDivide(entity);
			if (ret == false)
				LOG("Quadtree-Subdivide fail");
		}
		else
			for (int i = 0; i < NODE_ENTITIES; i++)
				if (entities[i] == nullptr)
				{
					entities[i] = entity;
					ret = true;
					break;
				}
	}
	else
		ret = PushToCorrectChild(entity);
	return ret;
}

bool QuadTreeNode::Inside(const Entity* entity) const
{
	return area->TileInside(entity->GetPosition());
}

Entity * QuadTreeNode::SearchFirst(int pixel_range, const iPoint from) const
{
	//wrong
	Entity* ret = nullptr;

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				Elipse circle(from, pixel_range);
				if (circle.IsIn(entities[i]->GetPixelPosition()))
					ret = entities[i];
			}
			else
				break;
	}
	else
		for(int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(from, pixel_range))
				 ret = childs[i]->SearchFirst(pixel_range, from);
	return ret;
}

Entity * QuadTreeNode::SearchFirst(IsoRect rect,const Entity* exeption) const
{
	Entity* ret = nullptr;

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				if (rect.Inside(entities[i]->GetPosition()) && entities[i] != exeption && entities[i]->GetHp() > 0)
					ret = entities[i];
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(rect))
			{
				ret = childs[i]->SearchFirst(rect, exeption);
				if (ret != nullptr)
					break;
			}
	return ret;
}

Entity * QuadTreeNode::SearchFirst(const SDL_Rect rect) const
{
	Entity* ret = nullptr;

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			iPoint entity_pos(entities[i]->GetX(), entities[i]->GetY());
			if (entities[i] != nullptr)
			{
				if (rect.x < entity_pos.x && rect.x + rect.w > entity_pos.x
					&& rect.y < entity_pos.y && rect.y + rect.h > entity_pos.y)
					ret = entities[i];
			}
			else
				break;
		}
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(rect))
			{
				ret = childs[i]->SearchFirst(rect);
				if (ret != nullptr)
					break;
			}
	return ret;
}

Entity * QuadTreeNode::SearchFirstUnit(const SDL_Rect & rect) const
{
	Entity* ret = nullptr;

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			if (entities[i] != nullptr)
			{
				iPoint entity_pos(entities[i]->GetX(), entities[i]->GetY());
				if (rect.x < entity_pos.x && rect.x + rect.w > entity_pos.x
					&& rect.y < entity_pos.y && rect.y + rect.h > entity_pos.y)
					if (entities[i]->GetEntityType() == E_UNIT)
						ret = entities[i];
			}
			else
				break;
		}
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(rect))
			{
				ret = childs[i]->SearchFirstUnit(rect);
				if (ret != nullptr)
					break;
			}
	return ret;
}

Entity * QuadTreeNode::SearchFirstEnemy(int pixel_range, const iPoint from, const Side side, ENTITY_TYPE entity_type) const
{
	Entity* ret = nullptr;

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				float distance_x = entities[i]->GetX() - from.x;
				float distance_y = entities[i]->GetY() - from.y;
				float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

				if (distance <= pixel_range && (entity_type == E_NO_ENTITY || entity_type == entities[i]->GetEntityType()))
					if (entities[i]->GetSide() != side && entities[i]->GetHp() >= 0)
						ret = entities[i];
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps( from, pixel_range))
			{
				ret = childs[i]->SearchFirstEnemy(pixel_range, from, side, entity_type);
				if (ret != nullptr)
					break;
			}
	return ret;
}

Entity * QuadTreeNode::SearchFirstEnemy(IsoRect rect, const Side side, ENTITY_TYPE entity_type) const
{
	Entity* ret = nullptr;
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				if (rect.Inside(entities[i]->GetPosition()) && (entity_type == E_NO_ENTITY || entity_type == entities[i]->GetEntityType()))
					if (entities[i]->GetSide() != side && entities[i]->GetHp() >= 0)
						ret = entities[i];
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(rect))
			{
				ret = childs[i]->SearchFirstEnemy(rect, side, entity_type);
				if (ret != nullptr)
					break;
			}
	return ret;
}

void QuadTreeNode::Search(int pixel_range, const iPoint from, std::vector<Entity*>& vec) const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				float distance_x = entities[i]->GetX() - from.x;
				float distance_y = entities[i]->GetY() - from.y;
				float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

				if (distance < pixel_range)
					if (entities[i]->GetHp() >= 0)
						vec.push_back(entities[i]);
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(from, pixel_range))
				childs[i]->Search(pixel_range, from, vec);
}

void QuadTreeNode::Selection(const SDL_Rect rect, std::vector<Entity*>& vec) const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				if (entities[i]->Inside(rect))
					vec.push_back(entities[i]);
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(rect))
				childs[i]->Selection(rect, vec);
}

Entity * QuadTreeNode::ClickSelect(const iPoint & mouse_pos) const
{
	Entity* ret = nullptr;
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				SDL_Rect rect = entities[i]->GetTextureRectWorldPos();
				if (rect.x < mouse_pos.x
					&& rect.x + rect.w > mouse_pos.x
					&& rect.y < mouse_pos.y
					&& rect.y + rect.h > mouse_pos.y)
				{
					ret = entities[i];
					break;
				}
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
		{
			ret = childs[i]->ClickSelect(mouse_pos);
			if (ret != nullptr)
				return ret;
		}
	return ret;
}

void QuadTreeNode::Search(const IsoRect rect, std::vector<Entity*>& vec) const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (rect.Inside(entities[i]->GetPosition()))
					vec.push_back(entities[i]);
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(rect))
				childs[i]->Search(rect, vec);
}

void QuadTreeNode::SearchForEnemies(int pixel_range, iPoint from, std::vector<Entity*>& vec, const Side side, ENTITY_TYPE entity_type)
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				float distance_x = entities[i]->GetPixelPosition().x - from.x;
				float distance_y = entities[i]->GetPixelPosition().y - from.y;
				float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

				if (distance <= pixel_range && (entity_type == E_NO_ENTITY || entity_type == entities[i]->GetEntityType()))
					if (entities[i]->GetSide() != side && entities[i]->GetHp() > 0)
						vec.push_back(entities[i]);
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(from, pixel_range))
				childs[i]->SearchForEnemies(pixel_range, from, vec, side, entity_type);
}

Entity * QuadTreeNode::SearchFirstCollisionInTile(iPoint tile, Entity* exeption)
{
	iPoint i_pos = App->map->MapToWorld(tile.x, tile.y);
	fPoint tile_center(i_pos.x, i_pos.y);
	IsoRect rect(tile_center, App->map->data.tile_width, App->map->data.tile_height);
	return SearchFirstCollision(rect, exeption);
}

void QuadTreeNode::SearchCollisions(iPoint tile, std::vector<Unit*>& vec, Entity * exeption)
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				if (tile == entities[i]->GetPosition() && entities[i] != exeption && entities[i]->GetHp() > 0 && ((Unit*)entities[i])->GetCollision() == nullptr && ((Unit*)entities[i])->GetAction() != A_MOVE)
					vec.push_back((Unit*)entities[i]);
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->TileInside(tile))
				childs[i]->SearchCollisions(tile, vec, exeption);
}

Entity * QuadTreeNode::SearchFirstCollision(IsoRect rect, Entity * exeption) const
{
	Entity* ret = nullptr;

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				if (rect.Inside(entities[i]->GetPosition()) && entities[i] != exeption && entities[i]->GetHp() > 0 && ((Unit*) entities[i])->GetCollision() == nullptr && ((Unit*)entities[i])->GetAction() != A_MOVE)
					ret = entities[i];
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->Overlaps(rect))
			{
				ret = childs[i]->SearchFirst(rect, exeption);
				if (ret != nullptr)
					break;
			}
	return ret;
}

bool QuadTreeNode::PushToCorrectChild(Entity * entity)
{
	for (int i = 0; i < 4; i++)
		if (childs[i]->Inside(entity))
			return childs[i]->AddEntity(entity);
	return false;
}

bool QuadTreeNode::SubDivide(Entity * entity)
{
	bool ret = false;

	float child_x_tiles = area->GetXTiles() / 2.0f;
	float child_y_tiles = area->GetYTiles() / 2.0f;

	if (child_x_tiles < 1.0f || child_y_tiles < 1.0f)
	{
		LOG("Trying to subdivide quadtree further than 1 tile");
		return false;
	}

	fPoint tile_center = area->GetTileCenter();
	tile_center.x = ceil(tile_center.x);
	tile_center.y = ceil(tile_center.y);

	iPoint first(area->GetTilePos());
	iPoint second(area->GetTilePos().x, area->GetTilePos().y + ceil(child_y_tiles));
	iPoint third(area->GetTilePos().x + ceil(child_x_tiles), area->GetTilePos().y);
	iPoint forth(tile_center.x, tile_center.y);

	childs[0] = new QuadTreeNode(first, ceil(child_x_tiles), area->GetTileWidth(), ceil(child_y_tiles), area->GetTileHeight(), this);
	childs[1] = new QuadTreeNode(second, ceil(child_x_tiles), area->GetTileWidth(), (int)child_y_tiles, area->GetTileHeight(), this);
	childs[2] = new QuadTreeNode(third, (int)child_x_tiles, area->GetTileWidth(), ceil(child_y_tiles), area->GetTileHeight(), this);
	childs[3] = new QuadTreeNode(forth, (int)child_x_tiles, area->GetTileWidth(), (int)child_y_tiles, area->GetTileHeight(), this);

	for (int i = 0; i < NODE_ENTITIES; i++)
	{
		ret = PushToCorrectChild(entities[i]);
		
		if (ret == false)
			return ret;
		else
			entities[i] = nullptr;
	}

	ret = PushToCorrectChild(entity);
	return ret;
}

void QuadTreeNode::Reorganise()
{
	for(int i = 0; i < NODE_ENTITIES - 1; i++)
		if (entities[i] == nullptr && entities[i + 1] != nullptr)
		{
			entities[i] = entities[i + 1];
			entities[i + 1] = nullptr;
			i = 0;
		}
}

void QuadTreeNode::RedoQuads()
{
	if (childs[0] != nullptr)
	{
		int num_entities = 0;

		for (int i = 0; i < 4; i++)
		{
			num_entities += childs[i]->NumEntities();
		}

		if (num_entities <= NODE_ENTITIES)
			for (int i = 0; i < 4; i++)
			{
				childs[i]->MoveUpEntities();
				DELETE_PTR(childs[i]);
			}
	}
}

bool QuadTreeNode::Empty() const
{
	bool ret = true;

	if(entities[0] != nullptr)
		ret = false;
	else
	{
		if (childs[0] != nullptr)
			for (int i = 0; i < 4; i++)
			{
				ret = childs[i]->Empty();
				if (ret == false)
					break;
			}
		else
			ret = true;
	}
	return ret;
}

int QuadTreeNode::NumEntities() const
{
	int ret = 0;

	if (childs[0] == nullptr)
	{
		while (ret <= NODE_ENTITIES)
		{
			if (entities[ret] != nullptr)
				ret++;
			else
				break;
		}
	}
	else
		for (int i = 0; i < 4; i++)
			ret += childs[i]->NumEntities();

	return ret;
}

void QuadTreeNode::MoveUpEntities()
{
	for (int i = 0; i < NODE_ENTITIES; i++)
	{
		if (entities[i] != nullptr)
		{
			for (int j = 0; j < NODE_ENTITIES; j++)
				if (parent->entities[j] == nullptr)
				{
					parent->entities[j] = entities[i];
					entities[i] = nullptr;
				}
		}
		else
			break;
	}
}

void QuadTreeNode::Update(float dt)
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				entities[i]->Update(dt);

				if (entities[i]->GetEntityType() == E_UNIT && Inside(entities[i]) == false)
				{
					QuadTreeNode* up_iterator = parent;
					while (up_iterator != nullptr)
					{
						if (up_iterator->Inside(entities[i]))
						{
							if (up_iterator->AddEntity(entities[i]))
							{
								entities[i] = nullptr;
								break;
							}						
						}
						else
							up_iterator = up_iterator->parent;
					}		
					Reorganise();
					RedoQuads();
				}
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			childs[i]->Update(dt);
}

void QuadTreeNode::DeleteEntities()
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			if (entities[i] != nullptr)
			{
				if (entities[i]->ToDelete())
					DELETE_PTR(entities[i]);
			}
			else
				break;
		}
		Reorganise();
	}
	else
	{	
		for(int i = 0; i < 4; i++)
			childs[i]->DeleteEntities();
		RedoQuads();
	}
}

void QuadTreeNode::CheckUnitCollisions(const Unit * ptr) const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			if (entities[i] != nullptr)
			{
				if (entities[i]->GetEntityType() == E_UNIT)
				{
					Unit* unit = (Unit*)entities[i];
					if (ptr->GetUnitCircle().Overlap(&unit->GetUnitCircle()))
					{ }
						//TODO
						/*if (unit->GetAction() == A_IDLE)
						{
							iPoint destination;
							if (App->pathfinding->FindNearestEmpty(destination))
								unit->GoTo(destination);
						}
						else if (unit->GetAction() == A_FIGHT)
						{
							iPoint destination;
							if (unit->FindEmptyAttackPos(destination))
								unit->GoTo(destination);
						}*/
				}
			}
		}
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area->TileInside(ptr->GetPosition()))
				childs[i]->CheckUnitCollisions(ptr);
}

void QuadTreeNode::Draw() const
{
	if (childs[0] != nullptr)
		for (int i = 0; i < 4; i++)
			childs[i]->Draw();

	if (App->debug_features.qt_entities)
		DrawEntities();

	if (App->debug_features.qt_area)
		area->DrawSquare();

	if (App->debug_features.qt_vertex)
		area->DrawVertex();

	if (App->debug_features.qt_diagonals)
		area->DrawDiagonals();

	if (App->debug_features.qt_center)
		area->DrawCenter();

}

void QuadTreeNode::DrawEntities() const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			if (entities[i] != nullptr)
			{
				//change to full tile
				SDL_Rect current_point{ entities[i]->GetPixelPosition().x, entities[i]->GetPixelPosition().y, 5, 5 };
				iPoint going_to = App->map->MapToWorld(entities[i]->GetPosition());
				going_to.y += App->map->data.tile_height / 2.0f;
				SDL_Rect going_to_point{ going_to.x, going_to.y, 5, 5 };

				App->render->DrawQuad(current_point, 0, 255, 0, 255);
				App->render->DrawQuad(going_to_point, 0, 255, 0, 255);
				App->render->DrawLine(current_point.x, current_point.y, going_to.x, going_to.y, 0, 0, 255, 255);
			}
			else
				break;
		}
	}
}

void QuadTreeNode::DrawTiles() const
{
	area->DrawTiles();
}

void QuadTreeNode::SaveAll(pugi::xml_node & node)
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				entities[i]->Save(node);
	}
	else
		for (int i = 0; i < 4; i++)
			childs[i]->SaveAll(node);
}

/*void QuadTreeNode::BlitMinimap()
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				entities[i]->DrawPointMinimap();
	}
	else
		for (int i = 0; i < 4; i++)
			childs[i]->BlitMinimap();
}*/

QuadTree::QuadTree(const iPoint& tile_pos, const uint x_tiles, const uint tile_width, const uint y_tiles, const uint tile_height)
{
	origin = new QuadTreeNode(tile_pos, x_tiles, tile_width, y_tiles, tile_height, nullptr);
}

QuadTree::~QuadTree()
{
	DELETE_PTR(origin);
}

bool QuadTree::PushBack(Entity * entity) const
{
	if(!origin->Inside(entity))
		return false;

	origin->AddEntity(entity);
	return true;
}

Entity * QuadTree::SearchFirst(int pixel_range, iPoint from) const
{
	return origin->SearchFirst(pixel_range, from);
}

Entity * QuadTree::SearchFirstEnemy(int pixel_range, iPoint from, Side side, ENTITY_TYPE entity_type) const
{
	return origin->SearchFirstEnemy(pixel_range, from, side);
}

Entity * QuadTree::SearchFirstEnemy(IsoRect rect, const Side side, ENTITY_TYPE entity_type) const
{
	return origin->SearchFirstEnemy(rect,side,entity_type);
}

bool QuadTree::CheckIfFull(IsoRect tile,const Entity* exeption) const
{
	if (origin->SearchFirst(tile, exeption) != nullptr)
		return true;
	return false;
}

void QuadTree::Search(int pixel_range, iPoint from, std::vector<Entity*>& vec) const
{
	origin->Search(pixel_range, from, vec);
}

void QuadTree::SearchForEnemies(int pixel_range, iPoint from, std::vector<Entity*>& vec, const Side side, ENTITY_TYPE entity_type)
{
	origin->SearchForEnemies(pixel_range, from,	vec, side, entity_type);
}

void QuadTree::SearchInIsoRect(const IsoRect rect, std::vector<Entity*>& vec)
{
	origin->Search(rect, vec);
}

Entity * QuadTree::SearchFirstCollisionInTile(iPoint tile, Entity * exeption) const
{
	return origin->SearchFirstCollisionInTile(tile, exeption);
}

void QuadTree::SearchCollisionsInTile(iPoint tile, std::vector<Unit*>& vec, Entity * exeption)
{
	origin->SearchCollisions(tile, vec, exeption);
}

Entity * QuadTree::SearchFirst(const SDL_Rect & rect) const
{
	return origin->SearchFirst(rect);
}

Entity* QuadTree::SearchFirstUnit(const SDL_Rect & rect) const
{
	return origin->SearchFirstUnit(rect);
}

void QuadTree::Selection(SDL_Rect rect, std::vector<Entity*>& vec) const
{
	vec.clear();
	origin->Selection(rect, vec);
}

void QuadTree::UpdateAll(float dt) const
{	
	origin->Update(dt);
}

void QuadTree::DeleteEntities() const
{
	origin->DeleteEntities();
}

void QuadTree::Draw() const
{
	origin->Draw();

	if (App->debug_features.tiles)
		origin->DrawTiles();
}


void QuadTree::SaveAll(pugi::xml_node & node)
{
	origin->SaveAll(node);
}

/*void QuadTree::BlitMinimap() const
{
	origin->BlitMinimap();
}*/

Entity * QuadTree::ClickSelect(const iPoint & mouse_pos) const
{
	return origin->ClickSelect(mouse_pos);
}

//TiledIsoRect

TiledIsoRect::TiledIsoRect(const iPoint& tile_pos, const uint x_tiles, const uint tile_width, const uint y_tiles, const uint tile_height, const iPoint& displacement, const SDL_Color& color) : tile_pos(tile_pos), x_tiles(x_tiles), y_tiles(y_tiles), tile_height(tile_height), tile_width(tile_width)
{
	iPoint top_pixel_vertex = App->map->MapToWorld(tile_pos);

	iPoint left_tile(tile_pos.x, tile_pos.y + y_tiles - 1.0f);
	iPoint left_pixel_vertex = App->map->MapToWorld(left_tile);
	left_pixel_vertex.x -= tile_width / 2.0f;
	left_pixel_vertex.y += tile_height / 2.0f + 0.5f;

	iPoint right_tile(tile_pos.x + x_tiles - 1.0f, tile_pos.y);
	iPoint right_pixel_vertex = App->map->MapToWorld(right_tile);
	right_pixel_vertex.x += tile_width / 2.0f;
	right_pixel_vertex.y += tile_height / 2.0f + 0.5f;
	
	iPoint bottom_tile(tile_pos.x + x_tiles - 1.0f, tile_pos.y + y_tiles - 1.0f);
	iPoint bottom_pixel_vertex = App->map->MapToWorld(bottom_tile);
	bottom_pixel_vertex.y += tile_height + 1.0f;

	fPoint top_bottom_vec(bottom_pixel_vertex.x - top_pixel_vertex.x, bottom_pixel_vertex.y - top_pixel_vertex.y);
	fPoint right_left_vec(left_pixel_vertex.x - right_pixel_vertex.x, left_pixel_vertex.y - right_pixel_vertex.y);

	fPoint center(top_pixel_vertex.x + top_bottom_vec.x / 2.0f, top_pixel_vertex.y + top_bottom_vec.y / 2.0f);// first vec to get it
	fPoint center_second_vec(right_pixel_vertex.x + right_left_vec.x / 2.0f, right_pixel_vertex.y + right_left_vec.y / 2.0f);

	if (center.x != center_second_vec.x || center.y != center_second_vec.y)
		LOG("TiledIsoRect diagonal centers not equal");

	//uint width = x_tiles * tile_width;
	//uint height = y_tiles * tile_height + (x_tiles + y_tiles) / 2.0f;

	uint width = sqrt(pow(right_left_vec.x, 2) + pow(right_left_vec.y, 2)); 
	uint height = sqrt(pow(top_bottom_vec.x,2) + pow(top_bottom_vec.y,2));

	double angle_horizontal_diagonal = 0.0f;
	double angle_vertical_diagonal = 0.0f;
	
	if (x_tiles != y_tiles)
	{
		fPoint top_center_vec(center.x - top_pixel_vertex.x, center.y - top_pixel_vertex.y);
		fPoint right_center_vec(center.x - right_pixel_vertex.x, center.y - right_pixel_vertex.y);

		angle_horizontal_diagonal = atan(right_center_vec.y / right_center_vec.x);
		angle_vertical_diagonal = atan(top_center_vec.x / top_center_vec.y);
	}

	rect = new IsoRect(center, width, height, angle_horizontal_diagonal, angle_vertical_diagonal,  displacement);
}

TiledIsoRect::~TiledIsoRect()
{
	delete rect;
}

void TiledIsoRect::SetColor(SDL_Color color)
{
	rect->SetColor(color);
}

void TiledIsoRect::DrawSquare() const
{
	//Rect
	rect->Draw();
}

void TiledIsoRect::DrawTiles() const
{
	//Tiles
	for (int i = 0; i <= x_tiles; i++)
	{
		iPoint start = App->map->MapToWorld(i, 0);
		iPoint end = App->map->MapToWorld(i, y_tiles);
		App->render->DrawLine(start.x, start.y, end.x, end.y, 206, 255, 29, 255, true);
	}

	for (int i = 0; i <= y_tiles; i++)
	{
		iPoint start = App->map->MapToWorld(0, i);
		iPoint end = App->map->MapToWorld(x_tiles, i);
		App->render->DrawLine(start.x, start.y, end.x, end.y, 206, 255, 29, 255, true);
	}
}

void TiledIsoRect::DrawVertex() const
{
	//Vertex
	//Top Position
	iPoint top_pixel_vertex = App->map->MapToWorld(tile_pos);
	SDL_Rect square_top{ top_pixel_vertex.x - 3, top_pixel_vertex.y - 3, 6,6 };
	App->render->DrawQuad(square_top, 255, 0, 255, 255, true, true);

	//Left
	iPoint left_tile(tile_pos.x, tile_pos.y + y_tiles - 1.0f);
	iPoint left_pixel_vertex = App->map->MapToWorld(left_tile);
	left_pixel_vertex.x -= tile_width / 2.0f;
	left_pixel_vertex.y += tile_height / 2.0f + 0.5f;
	SDL_Rect square_left{ left_pixel_vertex.x - 3, left_pixel_vertex.y - 3, 6,6 };
	App->render->DrawQuad(square_left, 255, 0, 255, 255, true, true);

	//Right
	iPoint right_tile(tile_pos.x + x_tiles - 1.0f, tile_pos.y);
	iPoint right_pixel_vertex = App->map->MapToWorld(right_tile);
	right_pixel_vertex.x += tile_width / 2.0f;
	right_pixel_vertex.y += tile_height / 2.0f + 0.5f;
	SDL_Rect square_right{ right_pixel_vertex.x - 3, right_pixel_vertex.y - 3, 6,6 };
	App->render->DrawQuad(square_right, 255, 0, 255, 255, true, true);

	//Bottom
	iPoint bottom_tile(tile_pos.x + x_tiles - 1.0f, tile_pos.y + y_tiles - 1.0f);
	iPoint bottom_pixel_vertex = App->map->MapToWorld(bottom_tile);
	bottom_pixel_vertex.y += tile_height + 1.0f;
	SDL_Rect square_bot{ bottom_pixel_vertex.x - 3, bottom_pixel_vertex.y - 3, 6,6 };
	App->render->DrawQuad(square_bot, 255, 0, 255, 255, true, true);
}

void TiledIsoRect::DrawDiagonals() const
{
	rect->DrawDiagonals();
}

void TiledIsoRect::DrawCenter() const
{
	SDL_Rect square_center{	rect->GetPosition().x - 3, rect->GetPosition().y - 3, 6,6 };
	App->render->DrawQuad(square_center, 255, 0, 255, 255, true, true);
}

uint TiledIsoRect::GetPixelWidth() const
{
	return rect->GetWidth();
}

uint TiledIsoRect::GetPixelHeight() const
{
	return rect->GetHeight();
}

uint TiledIsoRect::GetXTiles() const
{
	return x_tiles;
}

uint TiledIsoRect::GetYTiles() const
{
	return y_tiles;
}

uint TiledIsoRect::GetTileWidth() const
{
	return tile_width;
}

uint TiledIsoRect::GetTileHeight() const
{
	return tile_height;
}

const iPoint & TiledIsoRect::GetTilePos() const
{
	return tile_pos;
}

const fPoint & TiledIsoRect::GetPixelCenter()
{
	return rect->GetPosition();
}

const fPoint & TiledIsoRect::GetTileCenter()
{
	return fPoint((float)tile_pos.x + (float)x_tiles / 2.0f, (float)tile_pos.y + (float)y_tiles / 2.0f);
}

bool TiledIsoRect::TileInside(const fPoint tile) const
{
	if (tile.x >= tile_pos.x && tile.x <= tile_pos.x + x_tiles
		&& tile.y >= tile_pos.y && tile.y <= tile_pos.y + y_tiles)
		return true;
	return false;
}

bool TiledIsoRect::TileInside(const iPoint tile) const
{
	if (tile.x >= tile_pos.x && tile.x <= tile_pos.x + x_tiles
		&& tile.y >= tile_pos.y && tile.y <= tile_pos.y + y_tiles)
		return true;
	return false;
}

bool TiledIsoRect::PixelInside(const iPoint pos) const
{
	return rect->Inside(pos);
}

bool TiledIsoRect::PixelInside(const fPoint pos) const
{
	return rect->Inside(pos);
}

bool TiledIsoRect::Overlaps(SDL_Rect rect) const
{
	return this->rect->Overlaps(rect);
}

bool TiledIsoRect::Overlaps(IsoRect rect) const
{
	return this->rect->Overlaps(rect);
}

bool TiledIsoRect::Overlaps(iPoint center, int radius)
{
	return rect->Overlaps(center,radius);
}
