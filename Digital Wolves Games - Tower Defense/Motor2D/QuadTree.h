#ifndef _QUADTREE
#define _QUADTREE

#include <vector>
#include "SDL\include\SDL_rect.h"
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "Resources.h"

#define NODE_ENTITIES 3

class QuadTreeNode
{
private:
	QuadTreeNode* parent = nullptr;
	QuadTreeNode* childs[4];
	IsoRect area;
	Entity* entities[NODE_ENTITIES];

public:
	QuadTreeNode(IsoRect area, QuadTreeNode* parent);
	~QuadTreeNode();
	
	bool AddEntity(Entity* entity);
	bool Inside(const Entity* entity) const;

	Entity* SearchFirst(int pixel_range, const  iPoint from) const;
	Entity* SearchFirst(IsoRect rect,const Entity* exeption = nullptr) const;
	Entity* SearchFirst(const SDL_Rect rect) const;
	Entity* SearchFirstUnit(const SDL_Rect& rect) const;
	Entity* SearchFirstEnemy(int pixel_range, iPoint from, Side side, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	Entity* SearchFirstEnemy(IsoRect rect, const Side side, ENTITY_TYPE entity_type) const;
	void Search(int pixel_range, const iPoint from, std::vector<Entity*>& vec) const;
	void Search(const IsoRect rect, std::vector<Entity*>& vec) const;
	void SearchForEnemies(int pixel_range, iPoint from, std::vector<Entity*>& vec, const Side side, ENTITY_TYPE entity_type = E_NO_ENTITY);
	Entity* SearchFirstCollisionInTile(iPoint tile, Entity* exeption = nullptr);
	void SearchCollisionsInTile(iPoint tile, std::vector<Unit*>& vec, Entity* exeption = nullptr);
	void SearchCollisions(IsoRect tile, std::vector<Unit*>& vec, Entity* exeption = nullptr);
	Entity* SearchFirstCollision(IsoRect rect, Entity* exeption = nullptr) const;

	void Selection(const SDL_Rect rect, std::vector<Entity*>& vec) const;

	Entity* ClickSelect(const iPoint& mouse_pos) const;

	void Update(float dt);
	void DeleteEntities();
	void CheckUnitCollisions(const Unit* ptr) const;
	void DrawArea();
	void SaveAll(pugi::xml_node& node);
	//void BlitMinimap();

private:
	bool PushToCorrectChild(Entity* entity);
	bool SubDivide(Entity* entity);
	void Reorganise();
	void RedoQuads();
	bool Empty() const;
	int NumEntities() const;
	void MoveUpEntities();
};

class QuadTree
{
private:
	QuadTreeNode* origin;

public:
	QuadTree(IsoRect area);
	~QuadTree();

	bool PushBack(Entity* entity) const;
	
	Entity* SearchFirst(int pixel_range, iPoint from) const;
	Entity* SearchFirstEnemy(int pixel_range, iPoint from, Side side, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	Entity* SearchFirstEnemy(IsoRect rect, const Side side, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	bool CheckIfFull(IsoRect tile,const Entity* exeption = nullptr) const;
	void Search(int pixel_range, iPoint from, std::vector<Entity*>& vec) const;
	void SearchForEnemies(int pixel_range, iPoint from, std::vector<Entity*>& vec, const Side side, ENTITY_TYPE entity_type = E_NO_ENTITY);
	void SearchInIsoRect(const IsoRect rect, std::vector<Entity*>& vec);
	Entity* SearchFirstCollisionInTile(iPoint tile, Entity* exeption = nullptr) const;
	void SearchCollisionsInTile(iPoint tile, std::vector<Unit*>& vec, Entity* exeption = nullptr);
	Entity* SearchFirst(const SDL_Rect& rect) const;
	Entity* SearchFirstUnit(const SDL_Rect& rect) const;

	void Selection(SDL_Rect rect, std::vector<Entity*>& vec) const;

	void UpdateAll(float dt) const;

	void DeleteEntities() const;

	void Draw() const;

	void SaveAll(pugi::xml_node& node);

	//void BlitMinimap() const;

	Entity* ClickSelect(const iPoint& mouse_pos) const;
};

#endif
