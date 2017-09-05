#ifndef _UNITS
#define _UNITS

#include "PugiXml\src\pugixml.hpp"
#include <vector>
#include "Entity.h"
#include "j1Timer.h"
#include "IsoPrimitives.h"

#define XY_TILES_RELATION 2
#define TOWN_HALL iPoint(0, 272) //Final townhall destination;
#define SLOW_PROPORTION 1.7f
#define SLOW_TIME 5.0f

//range in tiles // will be turned to float (sqrt(range^2 + range^2)) to get diagonals
#define CLOSE_COMBAT_RANGE 1
#define MID_COMBAT_RANGE 2
#define LONG_COMBAT_RANGE 3
#define EXTRA_LONG_COMBAT_RANGE 4	//MANGONEL

//close combat approach
#define APPROACH 65 //pixels from ennemy while attacking
#define VISION_RANGE 300 //pixels

//AI
#define CHECK_DESTINATION_FRAMES 3 //chechs if destination full every x frames

class AnimationManager;
struct PathList;
struct PathNode;

enum UNIT_TYPE
{
	U_NO_UNIT = 0,
	U_VILLAGER,
	U_GOD,

		//INFANTRY
	U_MILITIA,
	U_MANATARMS,
	U_LONGSWORDMAN,
	U_TWOHANDEDSWORDMAN,
	U_CHAMPION,

	U_SPEARMAN,
	U_PIKEMAN,
		//ARCHERS
	U_ARCHER,
	U_ARBALEST,
	U_CAVALRYARCHER,
	U_HEAVYCAVALRYARCHER,
		//CAVALRY
	U_KNIGHT,
	U_CAVALIER,
	U_PALADIN,
		//SIEGE
	U_SIEGERAM,
	U_MANGONEL
};

enum UNIT_CLASS
{
	C_NO_CLASS = 0,
	C_INFANTRY,
	C_ARCHER,
	C_CAVALRY,
	C_SIEGE,
	C_SUICIDAL
};

enum ACTION
{
	A_NO_ACTION = 0,
	A_FIGHT,
	A_DIE,
	A_DISAPPEAR,
	A_IDLE,
	A_MOVE,
	A_APPROACH,
	A_CENTER
};

enum DIRECTION
{
	D_NO_DIRECTION = 0,
	D_NORTH,
	D_NORTH_EAST,
	D_EAST,
	D_SOUTH_EAST,
	D_SOUTH,
	D_SOUTH_WEST,
	D_WEST,
	D_NORTH_WEST
};

class Unit : public Entity
{
private:
	enum UNIT_TYPE unit_type;
	enum ACTION action;
	enum DIRECTION direction;
	UNIT_CLASS unit_class; //todo understeand and delete
	fPoint position_in_tile;	
	int attack;
	int range;
	float speed; //pixels per frame
	fPoint directional_speed;
	float rate_of_fire; // maybe attackSpeed?

	//path
	iPoint position; //tile
	iPoint destination; //tile
	uint path_position; //tile
	Elipse unit_circle;
	std::vector<iPoint> path_vec;

	AnimationManager* animation;
	AnimationManager* idle_siege; //siedge should be a sub class
	bool changed;
	Entity* target;
	bool siege_attacked = false; //siedge should be a sub class

	//buffs and debuffs
	bool slowed = false;
	j1Timer slow_timer;
	bool bonus_attack = false;
	bool bonus_defense = false;

private:
	//AI internal functions
		//Functional
	bool Move();
	void UnitDies();
	void GoToEnemy();
	void ChangeDirecctionToEnemy();
	void GoIdle();
	void DoDamage();
	//bool AproachEnemy();
	//void SetAttackPosition();
	void Fight();
	void MoveAway();
	void CenterUnit();
	void ChangeAnimation();
	void MoveToNextTile();

		//Getters
	bool OutOfHP() const;
	void EnemyInSight();
	bool DestinationFull() const;	
	bool EnemyDead();	
	//void CheckUnitsBuffs();
	int GetFrameAttack();
	void GetNextPathPosition();
	void GetEmptyAttackPositions(std::vector<iPoint>& vec, int range) const; //gives empty attack positions to attack this unit
	const iPoint& FindClosestEmptyAttackTile() const;

public:
	//Constructors & Destructors
	Unit(UNIT_TYPE u_type, iPoint pos, Side side);
	~Unit();

	//Main Functions
	void Update( float dt); // defines order
	void AI();
	void Draw();

	//Usefull
	void LookAt(const iPoint& pos);
	bool GoTo(const iPoint& destination);
	bool ChangeDirection(const iPoint& destination);
	void PlayDeathSound() const;
	void PlayAttackSound() const;
	void SlowUnit();

	//Getters
	const iPoint& GetPosition() const;
	const float GetX() const;
	const float GetY() const;
	const iPoint& GetPixelPosition() const;
	const DIRECTION GetDir() const;
	const UNIT_TYPE GetUnitType() const;
	const UNIT_CLASS GetUnitClass() const;
	const ACTION GetAction() const;
	const int GetUnitRadius() const;
	bool GetPath(const iPoint& destination);
	const int GetAttack() const;
	const float GetRange() const;
	const Elipse GetUnitCircle() const;
	const bool IsMoving() const;
	const iPoint& GetDestination() const;
	const float FramesToTileCenter() const;
	const fPoint& PixelsToTileCenter() const;
	const float GetSpeed() const;
	const float DistanceInTiles(const iPoint& pos) const;
	const DIRECTION GetDirection(iPoint objective) const;

	//Setters
	void SetAction(const ACTION action);

	//Extras
	void Save(pugi::xml_node& );
};

#endif

