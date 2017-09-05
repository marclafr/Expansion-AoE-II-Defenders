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
	int range;
	j1Timer attack_timer;
	Unit* target = nullptr;
	AnimationManager* anim_fire = nullptr;

public:
	iPoint arrowpos;
	Tower(TOWER_TYPE t_type,const iPoint& pos);
	~Tower();
	bool UpgradeTurret(TOWER_TYPE new_type); //only built turrets
	bool CanUpgrade(TOWER_TYPE new_type);
	void DoBattle();
	void Attack();
	void ChangeTexture();
	void StartTimers();
	//TODO const iPoint& GetProjectilePosition() const; //depending on target pos will give one of four options based on direction 
	const TOWER_TYPE GetTowerType() const;
	const int GetRange() const;
	void Destroyed();
	//void Save(pugi::xml_node&);
	float GetAttackSpeed();
	void SetSpeed(float new_speed);
	void SetRange(float new_range);

	//For Upgrades
	void ChangeProjectiles(TOWER_TYPE new_type);
	void NewSpeed(TOWER_TYPE new_type);
	void NewLife(TOWER_TYPE new_type);
	void NewAttack(TOWER_TYPE new_type);
	void NewRange(TOWER_TYPE new_type);
};

#endif //__TOWERS_