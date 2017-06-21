#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "j1Map.h"
#include "IsoPrimitives.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "QuadTree.h"


QuadTreeNode::QuadTreeNode(IsoRect area, QuadTreeNode * parent): area(area), parent(parent)
{
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
	return area.Inside(entity->GetPosition());
}

Entity * QuadTreeNode::SearchFirst(int pixel_range, const iPoint from) const
{
	//wrong
	Elipse circle(from, pixel_range);

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (circle.Intersects(&iPoint(entities[i]->GetX(), entities[i]->GetY())))
					return entities[i];
	}
	else
		for(int i = 0; i < 4; i++)
			if (circle.Intersects(&childs[i]->area))
				 return childs[i]->SearchFirst(pixel_range, from);
	return nullptr;
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
			if (rect.Overlaps(childs[i]->area))
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
			if (childs[i]->area.Overlaps(rect))
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
			if (childs[i]->area.Overlaps(rect))
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
			if (childs[i]->area.Overlaps( from, pixel_range))
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
			if (rect.Overlaps(childs[i]->area))
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
			if (childs[i]->area.Overlaps(from, pixel_range))
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
			if (childs[i]->area.Overlaps(rect))
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
			if (rect.Overlaps(childs[i]->area))
				childs[i]->Search(rect, vec);
}

void QuadTreeNode::SearchForEnemies(int pixel_range, iPoint from, std::vector<Entity*>& vec, const Side side, ENTITY_TYPE entity_type)
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				float distance_x = entities[i]->GetX() - from.x;
				float distance_y = entities[i]->GetY() - from.y;
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
			if (childs[i]->area.Overlaps(from, pixel_range))
				childs[i]->SearchForEnemies(pixel_range, from, vec, side, entity_type);
}

Entity * QuadTreeNode::SearchFirstCollisionInTile(iPoint tile, Entity* exeption)
{
	iPoint i_pos = App->map->MapToWorld(tile.x, tile.y);
	fPoint tile_center(i_pos.x, i_pos.y);
	IsoRect rect(tile_center, App->map->data.tile_width, App->map->data.tile_height);
	return SearchFirstCollision(rect, exeption);
}

void QuadTreeNode::SearchCollisionsInTile(iPoint tile, std::vector<Unit*>& vec, Entity * exeption)
{ 
	iPoint i_pos = App->map->MapToWorld(tile.x, tile.y);
	fPoint tile_center(i_pos.x, i_pos.y);
	IsoRect rect(tile_center, App->map->data.tile_width, App->map->data.tile_height);
	return SearchCollisions(rect, vec, exeption);
}

void QuadTreeNode::SearchCollisions(IsoRect tile, std::vector<Unit*>& vec, Entity * exeption)
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				if (tile.Inside(entities[i]->GetPosition()) && entities[i] != exeption && entities[i]->GetHp() > 0 && ((Unit*)entities[i])->GetCollision() == nullptr && ((Unit*)entities[i])->GetAction() != A_WALK)
					vec.push_back((Unit*)entities[i]);
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (tile.Overlaps(childs[i]->area))
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
				if (rect.Inside(entities[i]->GetPosition()) && entities[i] != exeption && entities[i]->GetHp() > 0 && ((Unit*) entities[i])->GetCollision() == nullptr && ((Unit*)entities[i])->GetAction() != A_WALK)
					ret = entities[i];
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			if (rect.Overlaps(childs[i]->area))
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

	float half_w = ceil(area.GetWidth() / 2.0f);
	float half_h = ceil(area.GetHeight() / 2.0f);

	fPoint first_pos(area.GetPosition().x, area.GetPosition().y - half_h / 2.0f);
	fPoint second_pos(area.GetPosition().x - half_w / 2.0f, area.GetPosition().y);
	fPoint third_pos(area.GetPosition().x, area.GetPosition().y + half_h / 2.0f);
	fPoint fourth_pos(area.GetPosition().x + half_w / 2.0f, area.GetPosition().y);

	IsoRect first(first_pos, half_w, half_h);
	IsoRect second(second_pos, half_w, half_h);
	IsoRect third(third_pos, half_w, half_h);
	IsoRect forth(fourth_pos, half_w, half_h);

	childs[0] = new QuadTreeNode(first, this);
	childs[1] = new QuadTreeNode(second, this);
	childs[2] = new QuadTreeNode(third, this);
	childs[3] = new QuadTreeNode(forth, this);

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
						else if (unit->GetAction() == A_ATTACK)
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
			if (ptr->GetUnitCircle().Intersects(&childs[i]->area))
				childs[i]->CheckUnitCollisions(ptr);
}

void QuadTreeNode::DrawArea()
{
	if (childs[0] != nullptr)
		for (int i = 0; i < 4; i++)
			childs[i]->DrawArea();

	float x_angle = area.GetXAngle();
	float width = App->map->data.width * App->map->data.tile_width;
	float height = App->map->data.height * App->map->data.tile_height + (App->map->data.width + App->map->data.height) / 2.0f;

	fPoint tile_center = area.GetPosition();
	fPoint center = App->map->MapToWorld(tile_center);

	App->render->DrawLine(center.x - width * 0.5f, center.y, center.x, center.y - height*sin(x_angle), 255, 0, 0, 255, true);
	App->render->DrawLine(center.x - width * 0.5f, center.y, center.x, center.y + height*sin(x_angle), 255, 0, 0, 255, true);
	App->render->DrawLine(center.x + width * 0.5f, center.y, center.x, center.y - height*sin(x_angle), 255, 0, 0, 255, true);
	App->render->DrawLine(center.x + width * 0.5f, center.y, center.x, center.y + height*sin(x_angle), 255, 0, 0, 255, true);
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

QuadTree::QuadTree(IsoRect area)
{
	origin = new QuadTreeNode(area, nullptr);
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
	origin->SearchCollisionsInTile(tile, vec, exeption);
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
	origin->DrawArea();
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