#ifndef _PROJECTILE
#define _PROJECTILE

#define AREA_DMG_RADIUS 75
#define ELEMENT_TERRAIN_TIME 4.0f
#define FLOOR_DAMAGE_TIME 500 //ms

#include "j1App.h"
#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1PerfTimer.h"

class AnimationManager;

enum PROJECTILE_TYPE 
{
	P_BASIC_ARROW,
	P_FIRE_ARROW,
	P_ICE_ARROW,
	P_AIR_ARROW,
	P_CANNONBALL,
	P_FIRE_CANNONBALL,
	P_ICE_CANNONBALL,
	P_AIR_CANNONBALL
};


class Projectile
{
private:
	SDL_Rect rect;
	iPoint pivot;
	iPoint start_pos;
	iPoint actual_pos, pre_actual_pos;
	iPoint last_pos;
	Entity* target;
	int damage;
	float diferential;
	enum PROJECTILE_TYPE projectile_type;
	float projectile_pos = 0; //it goes from 0 to 1
	int angle = 0;
	int start_height;
	int curve_height;
	AnimationManager* projectile_anim = nullptr;

	void AreaDamage(int damage, iPoint center, int radius);

	j1PerfTimer print_element_terrain_timer;
	j1PerfTimer area_damage_timer;
	bool dest_reached = false;
	iPoint element_terrain_pos;
	void PrintElementTerrain(PROJECTILE_TYPE element, iPoint center);

public:

	Projectile(iPoint initialpos, Entity* target, int damage, float TimeInSecs,int Startheight, int Curveheight, PROJECTILE_TYPE type);
	~Projectile();

	void Update();
	void Draw();

	bool floor_effect = false;
	int GetProjectilePos() const;
	int GetDamage() const;
	void SetRect(SDL_Rect rect);
	AnimationManager* GetProjectileAnim();
};

#endif