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

#define DELAY_BETWEEN_BUILD_PHASES 1.5f //secs

enum BUILDING_TYPE
{
	B_NO_BUILDING = 0,
	B_TOWER,
	B_WALL,
	B_TOWNHALL,
	B_UNIVERSITY
};

enum BUILDING_STATE
{
	BS_NO_STATE,
	BS_IN_CONSTRUCTION_BEGINING,
	BS_IN_CONSTRUCTION_MIDDLE,
	BS_IN_CONSTRUCTION_END,
	BS_BUILT,
	BS_DESTROYED
};

class Building : public Entity
{
protected:
	iPoint left_tile;
	std::vector<iPoint> tiles;
	BUILDING_STATE state = BS_IN_CONSTRUCTION_BEGINING;
	bool state_changed = true;
	BUILDING_TYPE building_type;
	j1Timer buildtimer;
	j1Timer DieTimer;
	IsoRect build_rect;
	AnimationManager* building_fire;
	int alpha = 255;

public:
	Building(BUILDING_TYPE b_type, iPoint pos, bool builded = false);
	~Building();

	void Update(float dt); // defines order
	virtual void ChangeTexture(); //towers change texts depending in inter stats other buildings use this one
	virtual void DoBattle(); //normal buildings do nothing, every other does their thing here (spawn troops, shoot arrows...)
	virtual void StartTimers(); //normal buildings do nothing, every other starts their timers here, they will be managed later in DoBattle
	void AI();
	void Draw();

	virtual void Destroyed();
	void DestroyBuilding();
	
	//Setters
	void SetPositions();

	//Getters
	const iPoint& GetPosition() const;
	const iPoint& GetPixelPosition() const;
	const BUILDING_TYPE GetBuildingType() const;
	IsoRect GetBuildRectangle();
	const int GetMaxHp() const;
	void GetEmptyAttackPositions(std::vector<iPoint>& vec, int range) const;
	const float DistanceInTiles(const iPoint& pos) const; // Always distance to closest tile

	//Extra
	//virtual void Save(pugi::xml_node&);
};

#endif


