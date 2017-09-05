#ifndef _RESOURCE_MANAGER
#define _RESOURCE_MANAGER

#define GOLD_LABEL_POS { 700, 500 }
#define GOLD_IMAGE_RECT { 406, 1165, 83, 56 }
#define STARTING_GOLD 150

#define BASIC_TOWER_COST 115
#define BOMBARD_TOWER_COST 135

#define TU_UPGRADE_FIRE_COST 450

#define TU_UPGRADE_ICE_COST 320

#define TU_UPGRADE_AIR_COST 400

#define BASIC_WALL_COST 60
#define STONE_WALL_UPGRADE_COST 85
#define BRICK_WALL_UPGRADE_COST 100

#define CHAMPION_COST 100
#define ARBALEST_COST 125
#define HEAVYCAVALRYARCHER_COST 175
#define PALADIN_COST 200

#define STARTING_POS iPoint(-70, 420) //has to change to tiles

enum BUILDING_TYPE;
enum TOWER_TYPE;
enum UNIT_TYPE;
enum WALL_LEVEL;
class UI_Label;
class UI_Image;

class ResourceManager
{
private:
	int gold = STARTING_GOLD;
	UI_Label* gold_label = nullptr;
	UI_Image* gold_image = nullptr;

public:
	ResourceManager();
	~ResourceManager();

	void SetGold(int value);
	void AddGold(int add);
	void UseGold(int amount);
	int GetGold() const;

	//Entity creation
	bool CanBuildTower(TOWER_TYPE type) const;
	bool BuildTower(TOWER_TYPE type);
	bool CanBuildWall(BUILDING_TYPE type) const;
	bool CanBuildAmountOfWalls(int number_of_walls) const;
	void BuildWall(BUILDING_TYPE type);
	bool CanTrainSoldier(UNIT_TYPE type) const;
	void TrainSoldier(UNIT_TYPE type);
	bool CanUpgradeTower(TOWER_TYPE type) const;
	const int GetTowerUpgradeCost(TOWER_TYPE type) const;
	bool CanUpgradeWall(WALL_LEVEL type) const;
	const int GetWallUpgradeCost(WALL_LEVEL type) const;


	void SaveResourcesAmount(pugi::xml_node&);
	void LoadResourcesAmount(pugi::xml_node&);
};

#endif