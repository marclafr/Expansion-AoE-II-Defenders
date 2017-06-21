#ifndef __TOWERS_
#define __TOWERS_

#include "Buildings.h"
#include "j1Timer.h"
#include "Projectile.h"
#define HEIGHT_BASIC_TOWER 146
class AnimationManager;

enum TOWER_TYPE
{
	T_NO_TYPE = 0,
	T_BASIC_TOWER,
	T_FIRE_TOWER,
	T_ICE_TOWER,
	T_AIR_TOWER,
	T_BOMBARD_TOWER,
	T_BOMBARD_FIRE_TOWER,
	T_BOMBARD_ICE_TOWER,
	T_BOMBARD_AIR_TOWER
};

enum TURRET_UPGRADE
{
	TU_NULL = 0,
	TU_FIRE,
	TU_ICE,
	TU_AIR
};

class Tower : public Building
{
private:

	TOWER_TYPE tower_type = T_NO_TYPE;
	PROJECTILE_TYPE projectile_type;
	float rate_of_fire;
	int projectile_spd;
	float range;
	j1Timer AttackTimer;
	Entity* Target = nullptr;
	SDL_Rect Position_rect;
	AnimationManager* anim_fire = nullptr;
public:
	iPoint arrowpos;
	Tower(TOWER_TYPE t_type, iPoint pos);
	~Tower();
	//void UpgradeTurret(INVESTIGATION_TYPE type);
	void Update(float dt); // defines order
	void AI();
	void Draw();
	bool attacking = false;
	const TOWER_TYPE GetTowerType() const;
	const int GetRange() const;
	void Save(pugi::xml_node&);
	float GetSpeed();
	void DestroyTower();
	void SetSpeed(float new_speed);
	void SetRange(float new_range);
	void ConvertTowerToRubble();
};

#endif //__TOWERS_