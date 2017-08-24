#ifndef _j1EntityManager_
#define _j1EntityManager_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "Entity.h"
#include "Units.h"
#include "Resources.h"
#include "Buildings.h"
#include "Towers.h"
#include "QuadTree.h"
#include "SDL/include/SDL_rect.h"

class j1EntityManager : public j1Module
{

private:
	QuadTree* entity_quadtree;
	bool siegeram_destroyed = false;
	fPoint siegeram_pos;

public:
	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateUnit(UNIT_TYPE u_type,const iPoint& pos, Side side);
	/*Entity* CreateBuilding(BUILDING_TYPE b_type, fPoint pos, bool builded) const;
	Entity* CreateTower(TOWER_TYPE t_type, fPoint pos) const;
	Entity* CreateResource(RESOURCE_TYPE r_type, fPoint pos, int amount_collect, int time) const;*/

	void SelectInQuad(const SDL_Rect& select_rect, std::vector<Entity*>& selection) const;
	void UnselectEverything() const;
	//void Select(Entity* select) const;	//TODO: This function is unused
	void CheckClick(int mouse_x, int mouse_y)  const;
	bool AreUnitsInRect(const SDL_Rect& rect) const;

	Entity* LookForEnemies(int pixel_range, iPoint pos, Side side, Entity* attacker, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	
	Entity* CheckForCombat(iPoint position, int range, Side side) const;
	Entity* CheckForObjective(iPoint position, int vision_range, Side side) const;

	bool IsUnitInTile(const Unit* unit, const iPoint tile) const;

	void DrawQuadTree() const;

	/*bool Load(pugi::xml_node&);
	void LoadBuilding(pugi::xml_node&);
	void LoadUnit(pugi::xml_node&);
	void LoadTurret(pugi::xml_node&);
	void LoadResource(pugi::xml_node&);

	bool Save(pugi::xml_node&)const;*/

	void DropUnits(float pos_x, float pos_y); //When a siegeram dies

	//void BlitMinimap() const;

	void GetEntitiesInIsoRect(const IsoRect rect, std::vector<Entity*>& vec) const;

	Entity* ClickSelect(const iPoint& mouse_pos) const;
};
#endif //_j1EntityManager_

