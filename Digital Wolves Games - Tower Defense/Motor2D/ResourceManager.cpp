#include "p2Log.h"
#include "j1App.h"
#include "j1Gui.h"
#include "UI_Label.h"
#include "UI_Image.h"
#include "Wall.h"
#include "Towers.h"
#include "j1EntityManager.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(): gold (STARTING_GOLD)
{
	static char current_gold[32];
	sprintf_s(current_gold, 32, "%i", STARTING_GOLD);
	gold_label = App->gui->CreateLabel(GOLD_LABEL_POS, DEFAULT_BACKGROUND_RECT_TEXT, current_gold);
	iPoint gold_img_pos = GOLD_LABEL_POS;
	gold_img_pos.x += gold_label->GetTextWidth();
	gold_image = App->gui->CreateImage(gold_img_pos, GOLD_IMAGE_RECT);
}

ResourceManager::~ResourceManager()
{}

void ResourceManager::SetGold(int value)
{
	gold = value;
	static char current_gold[32];
	sprintf_s(current_gold, 32, "%i", gold);
	gold_label->SetText(current_gold);
	gold_image->SetX(gold_label->GetX() + gold_label->GetTextWidth());
}

void ResourceManager::AddGold(int add)
{
	gold += add;
	static char current_gold[32];
	sprintf_s(current_gold, 32, "%i", gold);
	gold_label->SetText(current_gold);
	gold_image->SetX(gold_label->GetX() + gold_label->GetTextWidth());
}

void ResourceManager::UseGold(int amount)
{
	gold -= amount;
	static char current_gold[32];
	sprintf_s(current_gold, 32, "%i", gold);
	gold_label->SetText(current_gold);
	gold_image->SetX(gold_label->GetX() + gold_label->GetTextWidth());
}

int ResourceManager::GetGold() const
{
	return gold;
}


bool ResourceManager::CanBuildTower(TOWER_TYPE type) const
{
	switch (type)
	{
	case T_NO_TYPE:
		break;

	case T_BASIC_TOWER:
		return gold >= BASIC_TOWER_COST;
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
		return gold >= BASIC_TOWER_COST;
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

bool ResourceManager::BuildTower(TOWER_TYPE type)
{
	switch (type)
	{
	case T_NO_TYPE:
		break;

	case T_BASIC_TOWER:
		gold -= BASIC_TOWER_COST;
		//App->score_scene->build_simple_tower = false;
		break;

	case T_FIRE_TOWER:
		break;

	case T_ICE_TOWER:
		break;

	case T_AIR_TOWER:
		break;

	case T_BOMBARD_TOWER:
		gold -= BASIC_TOWER_COST;
		break;

	case T_BOMBARD_FIRE_TOWER:
		break;

	case T_BOMBARD_ICE_TOWER:
		break;

	case T_BOMBARD_AIR_TOWER:
		break;
	}
	//App->entity_manager->CreateTower(type, fPoint(pos.x, pos.y));
	return false;
}

bool ResourceManager::CanBuildWall(BUILDING_TYPE type) const
{
	return gold <= BASIC_WALL_COST;
}

bool ResourceManager::CanBuildAmountOfWalls(int number_of_walls) const
{
	return gold >= BASIC_WALL_COST * number_of_walls;
}

void ResourceManager::BuildWall(BUILDING_TYPE type)
{
	gold -= BASIC_WALL_COST;
	// maybe CreateWall() ??
}

bool ResourceManager::CanTrainSoldier(UNIT_TYPE type) const
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
		return gold >= CHAMPION_COST;
	case U_SPEARMAN:
		return false;
	case U_PIKEMAN:
		return false;
	case U_ARCHER:
		return false;
	case U_ARBALEST:
		return gold >= ARBALEST_COST;
	case U_CAVALRYARCHER:
		return false;
	case U_HEAVYCAVALRYARCHER:
		return gold >= HEAVYCAVALRYARCHER_COST;
	case U_KNIGHT:
		return false;
	case U_CAVALIER:
		return false;
	case U_PALADIN:
		return gold >= PALADIN_COST;
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
		gold -= CHAMPION_COST;
		break;
	case U_SPEARMAN:
		break;
	case U_PIKEMAN:
		break;
	case U_ARCHER:
		break;
	case U_ARBALEST:
		gold -= ARBALEST_COST;
		break;
	case U_CAVALRYARCHER:
		break;
	case U_HEAVYCAVALRYARCHER:
		gold -= HEAVYCAVALRYARCHER_COST;
		break;
	case U_KNIGHT:
		break;
	case U_CAVALIER:
		break;
	case U_PALADIN:
		gold -= PALADIN_COST;
		break;
	case U_SIEGERAM:
		break;
	}
	App->entity_manager->CreateUnit(type, STARTING_POS, S_ALLY);
}

bool ResourceManager::CanUpgradeTower(TOWER_TYPE type) const
{
	switch (type)
	{
	case T_NO_TYPE:
		return false;
	case T_FIRE_TOWER:
		return 	gold >= TU_UPGRADE_FIRE_COST;
	case T_ICE_TOWER:
		return 	gold >= TU_UPGRADE_ICE_COST;
	case T_AIR_TOWER:
		return 	gold >= TU_UPGRADE_AIR_COST;
	default:
		return false;
	}
	return false;
}

const int ResourceManager::GetTowerUpgradeCost(TOWER_TYPE type) const
{
	switch (type)
	{
	case T_NO_TYPE:
		break;
	case T_FIRE_TOWER:
		return TU_UPGRADE_FIRE_COST;
	case T_ICE_TOWER:
		return TU_UPGRADE_ICE_COST;
	case T_AIR_TOWER:
		return TU_UPGRADE_AIR_COST;
	default:
		break;
	}
}

bool ResourceManager::CanUpgradeWall(WALL_LEVEL type) const
{
	switch (type)
	{
	case WL_STONE:
		return gold >= STONE_WALL_UPGRADE_COST;
	case WL_BRICK:
		return gold >= BRICK_WALL_UPGRADE_COST;
	default:
		LOG("Wrong wall level, can't know if upgradable");
	}
	return false;
}

const int ResourceManager::GetWallUpgradeCost(WALL_LEVEL type) const
{
	switch (type)
	{
	case WL_STONE:
		return STONE_WALL_UPGRADE_COST;
	case WL_BRICK:
		return BRICK_WALL_UPGRADE_COST;
	default:
		LOG("Wrong wall level, can't know cost");
	}
}

void ResourceManager::SaveResourcesAmount(pugi::xml_node &data)
{
	pugi::xml_node actualresource = data.append_child("ResourcesAmount");
	actualresource.append_attribute("gold") = GetGold();
}

void ResourceManager::LoadResourcesAmount(pugi::xml_node &data)
{
	gold = data.attribute("gold").as_int();
}