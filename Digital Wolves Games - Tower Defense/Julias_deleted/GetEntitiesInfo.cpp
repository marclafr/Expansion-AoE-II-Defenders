#define MILITIA_HP 43
#define MANATARMS_HP 58
#define LONGSWORDMAN_HP 118
#define TWOHANDEDSWORDMAN_HP 95
#define CHAMPION_HP 152
#define SPEARMAN_HP 55
#define PIKEMAN_HP 65
#define ARCHER_HP 41
#define ARBALEST_HP 50
#define CAVALRYARCHER_HP 65
#define HEAVYCAVALRYARCHER_HP 81
#define KNIGHT_HP 137
#define CAVALIER_HP 125
#define PALADIN_HP 167
#define SIEGERAM_HP 600
#define MANGONEL_HP 500

#define TURRET_HP 150
#define BOMBARD_HP 175
#define WOODWALL_HP 250
#define STONEWALL_HP 500
#define BRICKWALL_HP 750
#define UNIVERSITY_HP 1500
#define TOWNHALL_HP 1500

#define RESOURCES_HP 10

#define Y_RESOURCES 962
#define X_WALLS 774
#define X_PUBLIC_BUILDINGS 800
#define Y_TOWERS 910
#define Y_BOMBARDS 936

#define HIGH_TOWERS 180
#define HIGH_BOMBARDS 200

#include "UIGetEntitiesInfo.h"

iPoint GetUnitIconPositionFromAtlas(const UNIT_TYPE type)
{
	iPoint ret{0,0};

	switch (type)
	{
		//Infantry
	case U_MILITIA:
		ret = { 1008,936 };
		break;

	case U_MANATARMS:
		ret = { 1034,936 };
		break;

	case U_LONGSWORDMAN:
		ret = { 1060,936 };
		break;

	case U_TWOHANDEDSWORDMAN:
		ret = { 774, 962 };
		break;

	case U_CHAMPION:
		ret = { 982,936 };
		break;

	case U_SPEARMAN:
		ret = { 1086,936 };
		break;

	case U_PIKEMAN:
		ret = { 1112,936 };
		break;

		//Archers
	case U_ARCHER:
		ret = { 1190,936 };
		break;

	case U_ARBALEST:
		ret = { 930,936 };
		break;

	case U_CAVALRYARCHER:
		ret = { 1216,936 };
		break;

	case U_HEAVYCAVALRYARCHER:
		ret = { 956, 936 };
		break;

		//Cavalry
	case U_KNIGHT:
		ret = { 1138,936 };
		break;

	case U_CAVALIER:
		ret = { 1164,936 };
		break;

	case U_PALADIN:
		ret = { 930, 910 };
		break;

		//Siege
	case U_SIEGERAM:
		ret = { 748, 962 };
		break;

	case U_MANGONEL:
		ret = { 1242,936 };
		break;

	case U_GOD:
		ret = { 956, 936};
		break;

	default:
		//LOG("Error UNIT TYPE SDL_Rect NULL (UIManager)");
		ret = { 0, 0};
		break;
	}

	return ret;
}

const char* GetUnitName(const UNIT_TYPE type)
{
	char* ret;

	switch (type)
	{
			//Infantry
	case U_MILITIA:
		ret = "Militia";
		break;

	case U_MANATARMS:
		ret = "Man at Arms";
		break;

	case U_LONGSWORDMAN:
		ret = "Long Swordman";
		break;

	case U_TWOHANDEDSWORDMAN:
		ret = "Two-Handed Swordman";
		break;

	case U_CHAMPION:
		ret = "Champion";
		break;

	case U_SPEARMAN:
		ret = "Spearman";
		break;

	case U_PIKEMAN:
		ret = "Spearman";
		break;
		
		//Archers
	case U_ARCHER:
		ret = "Archer";
		break;

	case U_ARBALEST:
		ret = "Arbalest";
		break;

	case U_CAVALRYARCHER:
		ret = "Cavalry Archer";
		break;

	case U_HEAVYCAVALRYARCHER:
		ret = "Heavy Calvalry Archer";
		break;
		
		//Cavalry
	case U_KNIGHT:
		ret = "Knight";
		break;

	case U_CAVALIER:
		ret = "Cavalier";
		break;

	case U_PALADIN:
		ret = "Paladin";
		break;
		
		//Siege
	case U_SIEGERAM:
		ret = "Siege Ram";
		break;

	case U_MANGONEL:
		ret = "Mangonel";
		break;

		//Dev Units
	case U_GOD:
		ret = "GOD";
		break;

	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = "Error Unit";
		break;
	}

	return ret;
}

const char* GetUnitPrice(const UNIT_TYPE type)
{
	char* ret;

	switch (type)
	{
		//Infantry
		/*
	case U_MILITIA:
		ret = "Food: 60 Gold: 20";
		break;

	case U_MANATARMS:
		ret = "Food: 60 Gold: 20";
		break;

	case U_LONGSWORDMAN:
		ret = "Food: 60 Gold: 20";
		break;

	case U_TWOHANDEDSWORDMAN:
		ret = "Food: 60 Gold: 20";
		break;
		*/
	case U_CHAMPION:
		ret = "Food: 100";
		break;

		/*
	case U_SPEARMAN:
		ret = "Food: 60 Wood: 20";
		break;

	case U_PIKEMAN:
		ret = "Spearman";
		break;
		*/
		//Archers
		/*
	case U_ARCHER:
		ret = "Wood: 60 Gold: 20";
		break;
		*/
	case U_ARBALEST:
		ret = "Food: 125";
		break;
		/*
	case U_CAVALRYARCHER:
		ret = "Wood: 60 Gold: 20";
		break;
		*/
	case U_HEAVYCAVALRYARCHER:
		ret = "Food: 175";
		break;

		//Cavalry
		/*
	case U_KNIGHT:
		ret = "Food: 60 Gold: 20";

	case U_CAVALIER:
		ret = "Food: 60 Gold: 20";
		break;
		*/
	case U_PALADIN:
		ret = "Food: 200";
		break;

		//Siege
		/*
	case U_SIEGERAM:
		ret = "Wood: 60 Gold: 20 Ram";
		break;
		*/
		//Dev Units
		/*
	case U_GOD:
		ret = "Better than others, because I'M FREE!";
		break;
		*/
	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = "Error Unit";
		break;
	}

	return ret;
}

iPoint GetResourceIconPositionFromAtlas(const RESOURCE_TYPE type)
{
	iPoint ret{0,0};

	switch (type)
	{
	case R_WOOD:
		ret = { 852, Y_RESOURCES};
		break;
	case R_STONE:
		ret = { 878, Y_RESOURCES};
		break;
	case R_FOOD:
		ret = { 826, Y_RESOURCES};
		break;
	case R_GOLD:
		ret = { 904, Y_RESOURCES};
		break;
	default:
		//LOG("Error Resource TYPE SDL_Rect NULL (UIManager)");
		ret = { 0, 0};
		break;
	}

	return ret;
}

const char * GetResourceName(const RESOURCE_TYPE type)
{
	const char* ret;

	switch (type)
	{
	case R_WOOD:
		ret = "Sawmill";
		break;
	case R_STONE:
		ret = "Quarry";
		break;
	case R_FOOD:
		ret = "Mill";
		break;
	case R_GOLD:
		ret = "Gold Mine";
		break;
	default:
		//LOG("Error Resource TYPE SDL_Rect NULL (UIManager)");
		ret = "Error Resource";
		break;
	}

	return ret;
}

iPoint GetBuildingIconPositionFromAtlas(const BUILDING_TYPE type)
{
	iPoint ret;

	switch (type)
	{
	case B_WOOD_WALL:
		ret = { X_WALLS, 936};
		break;

	case B_STONE_WALL:
		ret = { X_WALLS, 910};
		break;

	case B_BRICK_WALL:
		ret = { X_WALLS, 884 };
		break;

	case B_TOWNHALL:
		ret = { X_PUBLIC_BUILDINGS, 936};
		break;

	case B_UNIVERSITY:
		ret = { X_PUBLIC_BUILDINGS, 910};
		break;

	default:
		//LOG("Error BUilding TYPE SDL_Rect NULL (UIManager)");
		ret = { 0, 0};
		break;
	}

	return ret;
}

const char* GetBuildingName(const BUILDING_TYPE type)
{
	char* ret;

	switch (type)
	{
	case B_TURRET:
		ret = "Basic Tower";
		break;
	case B_TURRET_UPGRADED_FIRE:
		ret = "Fire Tower";
		break;
	case B_TURRET_UPGRADED_ICE:
		ret = "Ice Tower";
		break;
	case B_TURRET_UPGRADED_AIR:
		ret = "Air Tower";
		break;
	case B_CANNON:
		ret = "Bombard Tower";
		break;
	case B_CANNON_UPGRADED_FIRE:
		ret = "Fire Bombard Tower";
		break;
	case B_CANNON_UPGRADED_ICE:
		ret = "Ice Bombard Tower";
		break;
	case B_CANNON_UPGRADED_AIR:
		ret = "Air Bombard Tower";
		break;
	case B_WOOD_WALL: //THIS
		ret = "Wood Wall";
		break;
	case B_STONE_WALL:
		ret = "Stone Wall";
		break;
	case B_BRICK_WALL:
		ret = "Brick Wall";
		break;
	case B_TOWNHALL:
		ret = "Town Hall";
		break;
	case B_UNIVERSITY:
		ret = "University";
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = "Error Building";
		break;
	}

	return ret;
}

const char * GetBuildingPrice(const BUILDING_TYPE type)
{
	char* ret = nullptr;
	switch (type)
	{
	case B_TURRET:
		ret = "Wood: 75 Stone: 25";
		break;
	case B_WOOD_WALL:
		ret = "Stone: 50";
		break;
	case B_STONE_WALL:
		ret = "Stone: 50";
		break;
	default:
		ret = "Error Building Price";
		break;
	}

	return ret;
}

iPoint GetTowerIconPositionFromAtlas(const TOWER_TYPE type)
{
	iPoint ret{ 0,0};

	switch (type)
	{
	case T_BASIC_TOWER:
		ret = { 826,Y_TOWERS};
		break;
	case T_FIRE_TOWER:
		ret = { 852,Y_TOWERS};
		break;
	case T_ICE_TOWER:
		ret = { 878,Y_TOWERS};
		break;
	case T_AIR_TOWER:
		ret = { 904,Y_TOWERS};
		break;
	case T_BOMBARD_TOWER:
		ret = { 826,Y_BOMBARDS};
		break;
	case T_BOMBARD_FIRE_TOWER:
		ret = { 852,Y_BOMBARDS};
		break;
	case T_BOMBARD_ICE_TOWER:
		ret = { 878,Y_BOMBARDS};
		break;
	case T_BOMBARD_AIR_TOWER:	
		ret = { 904,Y_BOMBARDS};
		break;
	default:
		ret = { 0,0};
		break;
	}

	return ret;
}

const char* GetTowerName(const TOWER_TYPE type)
{
	char* ret = nullptr;

	switch (type)
	{
	case T_BASIC_TOWER:
		ret = "Basic Tower";
		break;
	case T_BOMBARD_TOWER:
		ret = "Bombard Tower";
		break;
	case T_FIRE_TOWER:
		ret = "Fire Tower";
		break;
	case T_ICE_TOWER:
		ret = "Ice Tower";
		break;
	case T_AIR_TOWER:
		ret = "Air Tower";
		break;
	case T_BOMBARD_FIRE_TOWER:
		ret = "Fire Bombard Tower";
		break;
	case T_BOMBARD_ICE_TOWER:
		ret = "Ice Bombard Tower";
		break;
	case T_BOMBARD_AIR_TOWER:
		ret = "Air Bombard Tower";
		break;
	default:
		ret = "Turret Error";
		break;
	}

	return ret;
}

const char * GetTowerPrice(const TOWER_TYPE type)
{
	char* ret = nullptr;

	switch (type)
	{
	case T_BASIC_TOWER:
		ret = "115 Wood, 50 Stone";
		break;
	case T_BOMBARD_TOWER:
		ret = "135 Wood, 80 Stone";
		break;
	default:
		ret = "Turret Price";
		break;
	}

	return ret;
}

const char * GetTowerUpgradeName(const INVESTIGATION_TYPE type)
{
	char* ret = nullptr;

	switch (type)
	{
	case INV_FIRE_TOWER:
		ret = "Upgrade to fire";
		break;
	case INV_ICE_TOWER:
		ret = "Upgrade to ice";
		break;
	case INV_AIR_TOWER:
		ret = "Upgrade to air";
		break;
	default:
		ret = "Error tower upgrade";
		break;
	}

	return ret;
}

const char * GetTowerUpgradePrice(const INVESTIGATION_TYPE type)
{
	char* ret = nullptr;

	switch (type)
	{
	case INV_FIRE_TOWER:
		ret = "Gold: 50, Wood: 55, Stone: 40";
		break;
	case INV_ICE_TOWER:
		ret = "Gold: 35, Wood: 25, Stone: 30";
		break;
	case INV_AIR_TOWER:
		ret = "Gold: 40, Wood: 50, Stone: 35";
		break;
	default:
		ret = "Error tower upgrade";
		break;
	}

	return ret;
}

//The level of wall obtains from type, the next lvl. If wood wall -> stone wall.
const char * GetWallUpgradeName(const BUILDING_TYPE type)
{
	char* ret = nullptr;

	switch (type)
	{
	case B_WOOD_WALL:
		ret = "Upgrade to stone wall";
		break;
	case B_STONE_WALL:
		ret = "Upgrade to brick wall";
		break;
	default:
		ret = "Error wall upgrade";
		break;
	}

	return ret;
}

//Same at GetWallUpgradeName
const char * GetWallUpgradePrice(const BUILDING_TYPE type)
{
	char* ret = nullptr;

	switch (type)
	{
	case B_WOOD_WALL:
		ret = "85 Stone"; // PRICE stone wall
		break;
	case B_STONE_WALL:
		ret = "100 Stone"; // PRICE brick wall
		break;
	default:
		ret = "Error wall upgrade";
		break;
	}

	return ret;
}

const char* GetSideName(const Side type)
{
	char* ret = nullptr;

	switch (type)
	{
	case S_ALLY:
		ret = "Ally";
		break;
	case S_ENEMY:
		ret = "Enemy";
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = "Error Side";
		break;
	}

	return ret;
}

int ReturnValueBarHPUnit(const UNIT_TYPE type, const uint hp, const uint pixels)
{
	if (hp <= 0)
		return 1;

	int ret = 0;

	switch (type)
	{		
	case U_MILITIA:
		ret = hp * pixels / MILITIA_HP;
		break;

	case U_MANATARMS:
		ret = hp * pixels / MANATARMS_HP;
		break;

	case U_LONGSWORDMAN:
		ret = hp * pixels / LONGSWORDMAN_HP;
		break;

	case U_TWOHANDEDSWORDMAN:
		ret = hp * pixels / TWOHANDEDSWORDMAN_HP;
		break;

	case U_CHAMPION:
		ret = hp * pixels / CHAMPION_HP;
		break;

	case U_SPEARMAN:
		ret = hp * pixels / SPEARMAN_HP;
		break;

	case U_PIKEMAN:
		ret = hp * pixels / PIKEMAN_HP;
		break;

	case U_ARCHER:
		ret = hp * pixels / ARCHER_HP;
		break;

	case U_ARBALEST:
		ret = hp * pixels / ARBALEST_HP;
		break;

	case U_CAVALRYARCHER:
		ret = hp * pixels / CAVALRYARCHER_HP;
		break;

	case U_HEAVYCAVALRYARCHER:
		ret = hp * pixels / HEAVYCAVALRYARCHER_HP;
		break;

	case U_KNIGHT:
		ret = hp * pixels / KNIGHT_HP;
		break;

	case U_CAVALIER:
		ret = hp * pixels / CAVALIER_HP;
		break;

	case U_PALADIN:
		ret = hp * pixels / PALADIN_HP;
		break;

	case U_SIEGERAM:
		ret = hp * pixels / SIEGERAM_HP;
		break;

	case U_MANGONEL:
		ret = hp * pixels / MANGONEL_HP;
		break;


	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = 0;
		break;
	}

	return ret;
}

int ReturnValueBarHPBuilding(const BUILDING_TYPE type, const uint hp, const uint pixels)
{
	if (hp <= 0)
		return 1;

	int ret = 0;

	switch (type)
	{
	case B_TURRET:
		ret = hp * pixels / TURRET_HP;
		break;
	case B_WOOD_WALL:
		ret = hp * pixels / WOODWALL_HP;
		break;
	case B_STONE_WALL:
		ret = hp * pixels / STONEWALL_HP;
		break;
	case B_BRICK_WALL:
		ret = hp * pixels / BRICKWALL_HP;
		break;
	case B_TOWNHALL:
		ret = hp * pixels / TOWNHALL_HP;
		break;
	case B_UNIVERSITY:
		ret = hp * pixels / UNIVERSITY_HP;
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = 0;
		break;
	}

	return ret;
}

int ReturnValueBarHPTower(const TOWER_TYPE type, const uint hp, const uint pixels)
{
	int ret = 0;

	switch (type)
	{
	case T_BASIC_TOWER:
		ret = hp * pixels / TURRET_HP;
		break;
	case T_FIRE_TOWER:
		ret = hp * pixels / TURRET_HP;
		break;
	case T_ICE_TOWER:
		ret = hp * pixels / TURRET_HP;
		break;
	case T_AIR_TOWER:
		ret = hp * pixels / TURRET_HP;
		break;
	case T_BOMBARD_TOWER:
		ret = hp * pixels / BOMBARD_HP;
		break;
	case T_BOMBARD_FIRE_TOWER:
		ret = hp * pixels / BOMBARD_HP;
		break;
	case T_BOMBARD_ICE_TOWER:
		ret = hp * pixels / BOMBARD_HP;
		break;
	case T_BOMBARD_AIR_TOWER:
		ret = hp * pixels / BOMBARD_HP;
		break;
	}


	return ret;
}

int ReturnValueBarHPResource(const RESOURCE_TYPE type, const uint hp, const uint pixels)
{
	if (hp <= 0)
		return 1;

	int ret = 0;

	switch (type)
	{
	case R_WOOD:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case R_STONE:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case R_FOOD:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case R_GOLD:
		ret = hp * pixels / RESOURCES_HP;
		break;
	}

	return ret;
}

int ReturnValueHeightCorrectionUnit(const UNIT_CLASS u_class)
{
	int ret = 0;

	switch (u_class)
	{
	case C_INFANTRY:
		ret = 47;
		break;

	case C_ARCHER:
		ret = 47;
		break;

	case C_CAVALRY:
		ret = 50;
		break;
			
	case C_SIEGE:
		ret = 50;
		break;

	case C_SUICIDAL:
		ret = 47;
		break;

	default:
		ret = 0;
		break;
	}

	return ret;
}

int ReturnValueHeightCorrectionBuilding(const BUILDING_TYPE type)
{
	int ret = 0;

	switch (type)
	{
	case B_WOOD_WALL:
		ret = 40;
		break;
	case B_STONE_WALL:
		ret = 90;
		break;
	case B_BRICK_WALL:
		ret = 90;
		break;
	case B_TOWNHALL:
		ret = 210;
		break;
	case B_UNIVERSITY:
		ret = 210;
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = 0;
		break;
	}

	return ret;
}

int ReturnValueHeightCorrectionResource(const RESOURCE_TYPE type)
{
	int ret = 0;

	switch (type)
	{
	case R_WOOD:
		ret = 150;
		break;
	case R_STONE:
		ret = 70;
		break;
	case R_FOOD:
		ret = 160;
		break;
	case R_GOLD:
		ret = 70;
		break;
	}

	return ret;
}

int ReturnValueHeightCorrectionTowers(const TOWER_TYPE type)
{
	int ret = 0;

	switch (type)
	{
	case T_BASIC_TOWER:
		ret = HIGH_TOWERS;
		break;
	case T_FIRE_TOWER:
		ret = HIGH_TOWERS;
		break;
	case T_ICE_TOWER:
		ret = HIGH_TOWERS;
		break;
	case T_AIR_TOWER:
		ret = HIGH_TOWERS;
		break;
	case T_BOMBARD_TOWER:
		ret = HIGH_BOMBARDS;
		break;
	case T_BOMBARD_FIRE_TOWER:
		ret = HIGH_BOMBARDS;
		break;
	case T_BOMBARD_ICE_TOWER:
		ret = HIGH_BOMBARDS;
		break;
	case T_BOMBARD_AIR_TOWER:
		ret = HIGH_BOMBARDS;
		break;
	}


	return ret;
}

const uint GetBuildingPercentage(const Entity * build)
{
	Building* building = (Building*)build;

	double actual_build_time = building->GetBuildTime();

	double time_build = GetBuildTotalTime(building->GetBuildingType());

	if (actual_build_time > time_build)
		return 100;

	return actual_build_time*100/time_build;
}

const double GetBuildTotalTime(const BUILDING_TYPE type)
{
	if(type == B_TURRET)
		return TURRET_BUILD;
	else if(type == B_CANNON)
		return CANNON_BUILD;
	else if (type == B_WOOD_WALL)
		return WOODWALL_BUILD;
	return 0;
}

const bool isBuilded(const Entity * build)
{
	Building* building = (Building*)build;

	double actual_build_time = building->GetBuildTime();

	double time_build = GetBuildTotalTime(building->GetBuildingType());

	if (actual_build_time > time_build)
		return true;

	return false;
}

iPoint GetInvestigationIconPositionFromAtlas(const INVESTIGATION_TYPE type)
{
	iPoint ret{ 0,0};

	switch (type)
	{
		//RESOURCES
	case INV_FOOD:
		ret = { 930,858 };
		break;
	case INV_WOOD:
		ret = { 852,858 };
		break;
	case INV_GOLD:
		ret = { 878,858 };
		break;
	case INV_STONE:
		ret = { 904,858 };
		break;
		//TROPS
		//INV_CAVALRY_UNLOCK
	case INV_CAVALRY_ATTACK:
		ret = { 800,858 };
		break;
	case INV_CAVALRY_DEFENSE:
		ret = { 748,884 };
		break;
		//INV_ARCHERS_UNLOCK
	case INV_ARCHERS_ATTACK:
		ret = { 748,936 };
		break;
	case INV_ARCHERS_DEFENSE:
		ret = { 800,884 };
		break;
		//INV_INFANTRY_UNLOCK,
	case INV_INFANTRY_ATTACK:
		ret = { 748,858 };
		break;
	case INV_INFANTRY_DEFENSE:
		ret = { 748,910 };
		break;
		//TOWERS
	case INV_FIRE_TOWER:
		ret = { 852,884 };
		break;
	case INV_ICE_TOWER:
		ret = { 878,884 };
		break;
	case INV_AIR_TOWER:
		ret = { 904,884 };
		break;
	}

	return ret;
}

iPoint GetGreyInvestigationIconPositionFromAtlas(const INVESTIGATION_TYPE type)
{
	iPoint ret{ 0,0 };

	switch (type)
	{
		//TROPS
		//INV_CAVALRY_UNLOCK
	case INV_CAVALRY_ATTACK:
		ret = { 1129,858 };;
		break;
	case INV_CAVALRY_DEFENSE:
		ret = { 1077,884 };
		break;
		//INV_ARCHERS_UNLOCK
	case INV_ARCHERS_ATTACK:
		ret = { 1181,858 };
		break;
	case INV_ARCHERS_DEFENSE:
		ret = { 1129,884 };
		break;
		//INV_INFANTRY_UNLOCK,
	case INV_INFANTRY_ATTACK:
		ret = { 1077,858 };;
		break;
	case INV_INFANTRY_DEFENSE:
		ret = { 1077,910 };
		break;
		//TOWERS
	case INV_FIRE_TOWER:
		ret = { 1181,884 };
		break;
	case INV_ICE_TOWER:
		ret = { 1207,884 };
		break;
	case INV_AIR_TOWER:
		ret = { 1233,884 };
		break;
	}

	return ret;
}

const char * GetExpandButtonsDescription(SDL_Rect rect_pos)
{
	char* ret = nullptr;

	if (rect_pos.x == 28 && rect_pos.y == 668)
		ret = "Tower Elemental Upgrades";
	else if (rect_pos.x == 28 && rect_pos.y == 698)
		ret = "Units Upgrades";
	else if (rect_pos.x == 28 && rect_pos.y == 728)
		ret = "Resources Upgrades";
	else
		ret = "Error";
	return ret;
}

const uint GetInvestigationPercentage(const Investigation * type)
{
	uint ret = 0;

	return ret;
}

const char* GetInvestigationName(const INVESTIGATION_TYPE type)
{
	char* ret = nullptr;

	switch (type)
	{
		//RESOURCES
		case INV_FOOD:
			ret = "Increase food recollection";
			break;
		case INV_WOOD:
			ret = "Increase wood recollection";
			break;
		case INV_GOLD:
			ret = "Increase gold recollection";
			break;
		case INV_STONE:
			ret = "Increase stone recollection";
			break;
		//TROPS
		//INV_CAVALRY_UNLOCK
		case INV_CAVALRY_ATTACK:
			ret = "Increase cavalry attack";
			break;
		case INV_CAVALRY_DEFENSE:
			ret = "Increase cavalry defense";
			break;
		//INV_ARCHERS_UNLOCK
		case INV_ARCHERS_ATTACK:
			ret = "Increase archers attack";
			break;
		case INV_ARCHERS_DEFENSE:
			ret = "Increase archers defense";
			break;
		//INV_INFANTRY_UNLOCK,
		case INV_INFANTRY_ATTACK:
			ret = "Increase infantry attack";
			break;
		case INV_INFANTRY_DEFENSE:
			ret = "Increase infantry defense";
			break;
		//TOWERS
		case INV_FIRE_TOWER:
			ret = "Allows to upgrade towers to FIRE";
			break;
		case INV_ICE_TOWER:
			ret = "Allows to upgrade towers to ICE";
			break;
		case INV_AIR_TOWER:
			ret = "Allows to upgrade towers to AIR";
			break;
	default:
		ret = "ERROR investigations name";
		break;
	}

	return ret;
}

void GetInvestigationPrice(const INVESTIGATION_TYPE type, std::string & price)
{
	price += std::to_string(App->investigations->GetInvestigationCost(App->investigations->GetInvestigation(type)));
}