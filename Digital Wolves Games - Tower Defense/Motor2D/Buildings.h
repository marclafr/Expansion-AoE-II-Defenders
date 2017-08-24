#ifndef _BUILDINGS
#define _BUILDINGS

#include "SDL/include/SDL_rect.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Module.h"
#include "Entity.h"
#include "Units.h"
#include "j1Timer.h"
#include <vector>
#include "j1Investigations.h"
#include "p2Point.h"

enum BUILDING_TYPE
{
	B_NO_BUILDING = 0,
	B_TURRET,
	B_CANNON,
	B_TURRET_UPGRADED_FIRE,
	B_TURRET_UPGRADED_ICE,
	B_TURRET_UPGRADED_AIR,
	B_CANNON_UPGRADED_FIRE,
	B_CANNON_UPGRADED_ICE,
	B_CANNON_UPGRADED_AIR,
	B_WOOD_WALL,
	B_STONE_WALL,
	B_BRICK_WALL,
	B_TOWNHALL,
	B_UNIVERSITY
};

enum BUILDING_STATE
{
	BS_NO_STATE,
	BS_IN_CONSTRUCTION,
	BS_BUILT,
	BS_DESTROIED
};

class Building : public Entity
{
private:
	iPoint center_position; //tile
	std::vector<iPoint> positions; //tiles
	BUILDING_STATE state = BS_IN_CONSTRUCTION;
	bool state_changed = false;
	BUILDING_TYPE building_type;
	j1Timer buildtimer;
	j1Timer DieTimer;
	bool alive = true;
	IsoRect build_rect;
	int max_hp = 0;
public:
	iPoint arrowpos;
	AnimationManager* building_fire;

	Building(BUILDING_TYPE b_type, iPoint pos, bool builded);
	~Building();

	virtual void Update(float dt); // defines order
	virtual void AI();
	virtual void Draw();

	void SetPositions();
	void ChangeTexture();
	void Destroied();

	const iPoint& GetPosition() const;

	const BUILDING_TYPE GetBuildingType() const;
	IsoRect GetBuildRectangle();
	void SetBuildingType(BUILDING_TYPE type);
	void UpgradeWall(BUILDING_TYPE type);
	const double GetBuildTime() const;
	const double GetDieTime() const;
	bool IsBuilt() const;
	void BuildingComplete();
	bool IsAlive() const;
	void ConvertToRubble();
	void DestroyBuilding();
	const int GetMaxHp() const;
	void SetMaxHP(int maxhp);
	//virtual void Save(pugi::xml_node&);
	bool IsTower();
};

#endif


