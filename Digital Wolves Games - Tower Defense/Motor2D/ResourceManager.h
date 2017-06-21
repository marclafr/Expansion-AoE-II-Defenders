//TODO activate
/*#ifndef _RESOURCE_MANAGER
#define _RESOURCE_MANAGER

#define STARTING_WOOD 325
#define STARTING_FOOD 10
#define STARTING_GOLD 150
#define STARTING_STONE 275

#define BASIC_TOWER_WOOD_COST 115
#define BASIC_TOWER_STONE_COST 50
#define BOMBARD_TOWER_WOOD_COST 135
#define BOMBARD_TOWER_STONE_COST 80

#define TU_UPGRADE_FIRE_GOLD_COST 450
#define TU_FIRE_GOLD_COST 50
#define TU_FIRE_WOOD_COST 55
#define TU_FIRE_STONE_COST 40

#define TU_UPGRADE_ICE_GOLD_COST 320
#define TU_ICE_GOLD_COST 35
#define TU_ICE_WOOD_COST 25
#define TU_ICE_STONE_COST 30

#define TU_UPGRADE_AIR_GOLD_COST 400
#define TU_AIR_GOLD_COST 40
#define TU_AIR_WOOD_COST 50
#define TU_AIR_STONE_COST 35

#define BASIC_WALL_STONE_COST 60
#define UPGRADED_WALL_STONE_COST 85
#define UPGRADED_WALL_BRICK_COST 100

#define CHAMPION_FOOD_COST 100
#define ARBALEST_FOOD_COST 125
#define HEAVYCAVALRYARCHER_FOOD_COST 175
#define PALADIN_FOOD_COST 200

#define STARTING_POS fPoint(-70, 420)

class Resources;
enum BUILDING_TYPE;
enum TOWER_TYPE;
enum RESOURCE_TYPE;
enum UNIT_TYPE;

class ResourceManager
{
private:
	Resources* resource_food = nullptr;
	Resources* resource_wood = nullptr;
	Resources* resource_gold = nullptr;
	Resources* resource_stone = nullptr;

	int wood = STARTING_WOOD;
	int food = STARTING_FOOD;
	int gold = STARTING_GOLD;
	int stone = STARTING_STONE;

public:
	ResourceManager();
	~ResourceManager();

	void SetWood(Resources* wood);
	void SetGold(Resources* gold);
	void SetStone(Resources* stone);
	void SetFood(Resources* food);

	void AddWood(int add);
	void AddStone(int add);
	void AddGold(int add);
	void AddFood(int add);

	bool ReduceWoodCollectTime(float amount) const;
	bool ReduceFoodCollectTime(float amount) const;
	bool ReduceGoldCollectTime(float amount) const;
	bool ReduceStoneCollectTime(float amount) const;

	void IncreaseResourceAmount(RESOURCE_TYPE type, float amount) const;

	void UseResource(RESOURCE_TYPE type, int amount);

	int GetWood() const;
	int GetStone() const;
	int GetGold() const;
	int GetFood() const;

	int GetResource(RESOURCE_TYPE type);

	//Entity creation
	bool CanBuildTower(TOWER_TYPE type);
	void BuildTower(TOWER_TYPE type, iPoint pos);
	bool CanBuildWall(BUILDING_TYPE type);
	bool CanBuildAmountOfWalls(int number_of_walls);
	void BuildWall(BUILDING_TYPE type);
	bool CanTrainSoldier(UNIT_TYPE type);
	void TrainSoldier(UNIT_TYPE type);
	bool CanUpgradeTower(TOWER_TYPE type);
	void UpgradeTower(TOWER_TYPE type);
	bool CanUpgradeWall(BUILDING_TYPE type);
	void UpgradeWall(BUILDING_TYPE type);


	void SaveResourcesAmount(pugi::xml_node&);

	void LoadResourcesAmount(pugi::xml_node&);

};


#endif
*/