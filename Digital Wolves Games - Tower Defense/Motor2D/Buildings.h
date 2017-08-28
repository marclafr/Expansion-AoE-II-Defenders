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
	B_TURRET,
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

public:
	Building(BUILDING_TYPE b_type, iPoint pos, bool builded = false);
	~Building();

	void Update(float dt); // defines order
	virtual void AI();
	virtual void ChangeTexture();
	void Draw();

	void Destroyed();
	void DestroyBuilding();
	
	//Setters
	void SetPositions();

	//Getters
	const iPoint& GetPosition() const;
	const iPoint& GetPixelPosition() const;
	const BUILDING_TYPE GetBuildingType() const;
	IsoRect GetBuildRectangle();
	const int GetMaxHp() const;
	//virtual void Save(pugi::xml_node&);
};

#endif


