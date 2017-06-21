//TODO activate
/*
#include "p2Log.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Buildings.h"
#include "Towers.h"
#include "Units.h"
#include "Resources.h"
#include "ResourceManager.h"
#include "j1ScoreScene.h"

ResourceManager::ResourceManager(): wood(STARTING_WOOD), food (STARTING_FOOD), gold (STARTING_GOLD), stone (STARTING_STONE)
{
	resource_food = (Resources*)App->entity_manager->CreateResource(R_FOOD, fPoint(1412, 888), 25, 8);
	resource_wood = (Resources*)App->entity_manager->CreateResource(R_WOOD, fPoint(1260, 1009), 80, 7);
	resource_gold = (Resources*)App->entity_manager->CreateResource(R_GOLD, fPoint(1608, 982), 55, 17);
	resource_stone = (Resources*)App->entity_manager->CreateResource(R_STONE, fPoint(1439, 1050), 65, 6);
}

ResourceManager::~ResourceManager()
{}

void ResourceManager::SetWood(Resources * wood)
{
	resource_wood = wood;
}

void ResourceManager::SetGold(Resources * gold)
{
	resource_gold = gold;
}

void ResourceManager::SetStone(Resources * stone)
{
	resource_stone = stone;
}

void ResourceManager::SetFood(Resources * food)
{
	resource_food = food;
}

void ResourceManager::AddWood(int add)
{
	wood += add;
}

void ResourceManager::AddStone(int add)
{
	stone += add;
}

void ResourceManager::AddGold(int add)
{
	gold += add;
}

void ResourceManager::AddFood(int add)
{
	food += add;
}

bool ResourceManager::ReduceWoodCollectTime(float amount) const
{
	return resource_wood->ReduceCollectTime(amount);
}

bool ResourceManager::ReduceFoodCollectTime(float amount) const
{
	return resource_food->ReduceCollectTime(amount);
}

bool ResourceManager::ReduceGoldCollectTime(float amount) const
{
	return resource_gold->ReduceCollectTime(amount);
}

bool ResourceManager::ReduceStoneCollectTime(float amount) const
{
	return resource_stone->ReduceCollectTime(amount);
}

void ResourceManager::IncreaseResourceAmount(RESOURCE_TYPE type, float amount) const
{
	switch (type)
	{
	case R_WOOD:
		resource_wood->IncreaseResourceAmount(amount);
		break;
	case R_STONE:
		resource_stone->IncreaseResourceAmount(amount);
		break;
	case R_GOLD:
		resource_gold->IncreaseResourceAmount(amount);
		break;
	case R_FOOD:
		resource_food->IncreaseResourceAmount(amount);
		break;
	}
}

void ResourceManager::UseResource(RESOURCE_TYPE type, int amount)
{
	switch (type)
	{
	case R_WOOD:
		wood -= amount;
		break;
	case R_STONE:
		stone -= amount;
		break;
	case R_GOLD:
		gold -= amount;
		break;
	case R_FOOD:
		food -= amount;
		break;
	}
}

int ResourceManager::GetWood() const
{
	return wood;
}

int ResourceManager::GetStone() const
{
	return stone;
}

int ResourceManager::GetGold() const
{
	return gold;
}

int ResourceManager::GetFood() const
{
	return food;
}

int ResourceManager::GetResource(RESOURCE_TYPE type)
{
	switch (type)
	{
	case R_FOOD:
		return food;
	case R_WOOD:
		return wood;
	case R_GOLD:
		return gold;
	case R_STONE:
		return stone;
	default:
		LOG("Invalid Resource");
		return 0;
	}
}

bool ResourceManager::CanBuildTower(TOWER_TYPE type)
{
	switch (type)
	{
	case T_NO_TYPE:
		return false;
		break;

	case T_BASIC_TOWER:
		return wood >= BASIC_TOWER_WOOD_COST && stone > BASIC_TOWER_STONE_COST;
		break;

	case T_FIRE_TOWER:
		return false;
		break;

	case T_ICE_TOWER:
		return false;
		break;

	case T_AIR_TOWER:
		return false;
		break;

	case T_BOMBARD_TOWER:
		return wood >= BASIC_TOWER_WOOD_COST && stone > BASIC_TOWER_STONE_COST;
		break;

	case T_BOMBARD_FIRE_TOWER:
		return false;
		break;

	case T_BOMBARD_ICE_TOWER:
		return false;
		break;

	case T_BOMBARD_AIR_TOWER:
		return false;
		break;

	}
	return false;
}

void ResourceManager::BuildTower(TOWER_TYPE type, iPoint pos)
{
	switch (type)
	{
	case T_NO_TYPE:
		break;

	case T_BASIC_TOWER:
		wood -= BASIC_TOWER_WOOD_COST;
		stone -= BASIC_TOWER_STONE_COST;
		App->score_scene->build_simple_tower = false;
		break;

	case T_FIRE_TOWER:
		break;

	case T_ICE_TOWER:
		break;

	case T_AIR_TOWER:
		break;

	case T_BOMBARD_TOWER:
		wood -= BASIC_TOWER_WOOD_COST;
		stone -= BASIC_TOWER_STONE_COST;
		break;

	case T_BOMBARD_FIRE_TOWER:
		break;

	case T_BOMBARD_ICE_TOWER:
		break;

	case T_BOMBARD_AIR_TOWER:
		break;
	}
	App->entity_manager->CreateTower(type, fPoint(pos.x, pos.y));
}

bool ResourceManager::CanBuildWall(BUILDING_TYPE type)
{
	switch (type)
	{
	case B_WOOD_WALL:
		return stone >= BASIC_WALL_STONE_COST;
		break;
	case B_STONE_WALL:
		return stone >= BASIC_WALL_STONE_COST;
		break;
	}
	return false;
}

bool ResourceManager::CanBuildAmountOfWalls(int number_of_walls)
{
	return stone >= BASIC_WALL_STONE_COST* number_of_walls;
	return false;
}

void ResourceManager::BuildWall(BUILDING_TYPE type)
{
	switch (type)
	{
	case B_WOOD_WALL:
		stone -= BASIC_WALL_STONE_COST;
		break;

	case B_STONE_WALL:
		stone -= BASIC_WALL_STONE_COST;
		break;
	}
}

bool ResourceManager::CanTrainSoldier(UNIT_TYPE type)
{
	switch (type)
	{
	case U_NO_UNIT:
		return false;
	case U_VILLAGER:
		return false;
	case U_GOD:
		return false;
	case U_MILITIA:
		return false;
	case U_MANATARMS:
		return false;
	case U_LONGSWORDMAN:
		return false;
	case U_TWOHANDEDSWORDMAN:
		return false;
	case U_CHAMPION:
		return food >= CHAMPION_FOOD_COST;
		return false;
	case U_SPEARMAN:
		return false;
	case U_PIKEMAN:
		return false;
	case U_ARCHER:
		return false;
	case U_ARBALEST:
		return food >= ARBALEST_FOOD_COST;
		return false;
	case U_CAVALRYARCHER:
		return false;
	case U_HEAVYCAVALRYARCHER:
		return food >= HEAVYCAVALRYARCHER_FOOD_COST;
		return false;
	case U_KNIGHT:
		return false;
	case U_CAVALIER:
		return false;
	case U_PALADIN:
		return food >= PALADIN_FOOD_COST;
		return false;
	case U_SIEGERAM:
		return false;
	}
	return false;
}

void ResourceManager::TrainSoldier(UNIT_TYPE type)
{
	switch (type)
	{
	case U_NO_UNIT:
		break;
	case U_VILLAGER:
		break;
	case U_GOD:
		break;
	case U_MILITIA:
		break;
	case U_MANATARMS:
		break;
	case U_LONGSWORDMAN:
		break;
	case U_TWOHANDEDSWORDMAN:
		break;
	case U_CHAMPION:
		food -= CHAMPION_FOOD_COST;
		break;
	case U_SPEARMAN:
		break;
	case U_PIKEMAN:
		break;
	case U_ARCHER:
		break;
	case U_ARBALEST:
		food -= ARBALEST_FOOD_COST;
		break;
	case U_CAVALRYARCHER:
		break;
	case U_HEAVYCAVALRYARCHER:
		food -= HEAVYCAVALRYARCHER_FOOD_COST;
		break;
	case U_KNIGHT:
		break;
	case U_CAVALIER:
		break;
	case U_PALADIN:
		food -= PALADIN_FOOD_COST;
		break;
	case U_SIEGERAM:
		break;
	}
	App->entity_manager->CreateUnit(type, STARTING_POS, S_ALLY);
}

bool ResourceManager::CanUpgradeTower(TOWER_TYPE type)
{
	switch (type)
	{
	case T_NO_TYPE:
		return false;
	case T_FIRE_TOWER:
		return 	(gold >= TU_FIRE_GOLD_COST && wood >= TU_FIRE_WOOD_COST && stone >= TU_FIRE_STONE_COST);
	case T_ICE_TOWER:
		return 	(gold >= TU_ICE_GOLD_COST && wood >= TU_ICE_WOOD_COST && stone >= TU_ICE_STONE_COST);
	case T_AIR_TOWER:
		return 	(gold >= TU_AIR_GOLD_COST && wood >= TU_AIR_WOOD_COST && stone >= TU_AIR_STONE_COST);
	default:
		return false;
	}
	return false;
}

void ResourceManager::UpgradeTower(TOWER_TYPE type)
{
	switch (type)
	{
	case T_NO_TYPE:
		break;
	case T_FIRE_TOWER:
		gold -= TU_FIRE_GOLD_COST;
		wood -= TU_FIRE_WOOD_COST;
		stone -= TU_FIRE_STONE_COST;
		break;
	case T_ICE_TOWER:
		gold -= TU_ICE_GOLD_COST;
		wood -= TU_ICE_WOOD_COST;
		stone -= TU_ICE_STONE_COST;
		break;
	case T_AIR_TOWER:
		gold -= TU_AIR_GOLD_COST;
		wood -= TU_AIR_WOOD_COST;
		stone -= TU_AIR_STONE_COST;
		break;
	default:
		break;
	}
}

bool ResourceManager::CanUpgradeWall(BUILDING_TYPE type)
{
	switch (type)
	{
	case B_NO_BUILDING:
		return false;
	case B_WOOD_WALL:
		return stone >= UPGRADED_WALL_STONE_COST;
	case B_STONE_WALL:
		return stone >= UPGRADED_WALL_BRICK_COST;
	default:
		return false;
	}
	return false;
}

void ResourceManager::UpgradeWall(BUILDING_TYPE type)
{
	switch (type)
	{
	case B_NO_BUILDING:
		break;
	case B_WOOD_WALL:
		stone -= UPGRADED_WALL_STONE_COST;
		break;
	case B_STONE_WALL:
		stone -= UPGRADED_WALL_BRICK_COST;
		break;
	default:
		break;
	}
}

void ResourceManager::SaveResourcesAmount(pugi::xml_node &data)
{
	pugi::xml_node actualresource = data.append_child("ResourcesAmount");
	actualresource.append_attribute("wood") = GetWood();
	actualresource.append_attribute("stone") = GetStone();
	actualresource.append_attribute("gold") = GetGold();
	actualresource.append_attribute("food") = GetFood();
}

void ResourceManager::LoadResourcesAmount(pugi::xml_node &data)
{
	wood = data.attribute("wood").as_int();
	gold = data.attribute("gold").as_int();
	stone = data.attribute("stone").as_int();
	food = data.attribute("food").as_int();
}
*/