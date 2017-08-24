#include <vector>
#include "j1App.h"
#include "j1Pathfinding.h"
#include "Units.h"
//#include "j1UIManager.h"
//#include "UIHUDPanelInfo.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "j1Score.h"
#include "j1WaveManager.h"
#include "Camera.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1ScoreScene.h"
#include "j1Gui.h"

#include "j1EntityManager.h"

#define MAX_SELECTION 25

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("entity_manager");
}

j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Start()
{

	/*float m = App->map->data.height;
	float n = App->map->data.width;

	float map_h = (m + n) * 0.5f * App->map->data.tile_height + 75;//75 1 4 each tile
	float map_w = (m + n) * 0.5f * App->map->data.tile_width;

	float map_x = (m - n) * 0.5f * 0.5f * App->map->data.tile_width;
	float map_y = (m + n) * 0.5f * 0.5f * App->map->data.tile_height + 75 / 2; //75 1 4 each tile

	//IsoRect map(iPoint(map_x, map_y), map_w, map_h);
	//IsoRect map(fPoint(map_x, map_y), map_w, map_h);*/

	//this should not be 76 * 76
	//IsoRect map(fPoint( / 2.0f,  / 2.0f), App->map->data.width, App->map->data.height);

	entity_quadtree = new QuadTree({0,0}, App->map->data.width, App->map->data.tile_width, App->map->data.height, App->map->data.tile_height);

	return true;
}

bool j1EntityManager::CleanUp() {

	DELETE_PTR(entity_quadtree);
	return true;
}

Entity * j1EntityManager::CreateUnit(UNIT_TYPE u_type,const iPoint& pos, Side side)
{
	Entity* new_entity = (Entity*) new Unit(u_type, pos, side);
	entity_quadtree->PushBack(new_entity);
	App->score_scene->units_count++;
	App->pathfinding->MakeNoWalkable(pos);
	return new_entity;
}

//TODO uncoment and fix
/*
Entity * j1EntityManager::CreateBuilding(BUILDING_TYPE b_type, fPoint pos, bool builded) const
{
	Entity* new_entity = (Entity*) new Building(b_type, pos, builded);
	entity_quadtree->PushBack(new_entity);
	if (b_type == B_TOWNHALL)
		App->uimanager->SetTownHall((Building*)new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateTower(TOWER_TYPE t_type, fPoint pos) const
{
	Entity* new_entity = (Entity*) new Tower(t_type, pos);
	entity_quadtree->PushBack(new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateResource(RESOURCE_TYPE r_type, fPoint pos, int amount_collect, int time) const
{
	Entity* new_entity = (Entity*) new Resources(r_type, pos, amount_collect, time);
	entity_quadtree->PushBack(new_entity);
	return new_entity;
}*/

void j1EntityManager::SelectInQuad(const SDL_Rect& select_rect, std::vector<Entity*>& selection) const
{
	bool unit_found = false;

	selection.clear();

	entity_quadtree->Selection(select_rect, selection);

	for (std::vector<Entity*>::iterator it = selection.begin(); it != selection.end(); ++it)
	{
		(*it)->SetEntityStatus(ST_SELECTED);

		if ((*it)->GetEntityType() == E_UNIT && (*it)->GetHp() > 0 && unit_found == false)
		{
			unit_found = true;
			it = selection.begin();
		}

		if ((*it)->GetHp() > 0)
		{
			if (unit_found)
			{
				if ((*it)->GetEntityType() == E_UNIT && (*it)->GetHp() > 0)
					(*it)->SetEntityStatus(ST_SELECTED);
				else
				{
					(*it)->SetEntityStatus(ST_NON_SELECTED);
					selection.erase(it);
					--it;
					if (selection.size() == 0)
						break;
				}
			}
			else
				(*it)->SetEntityStatus(ST_SELECTED);
		}
		else
		{
			(*it)->SetEntityStatus(ST_NON_SELECTED);
			selection.erase(it);
			--it;
			if (selection.size() == 0)
				break;
		}
	}

	if (selection.size() == 0)
	{
		//click select
		iPoint world_mouse_pos(select_rect.x, select_rect.y);
		Entity* selected = App->entity_manager->ClickSelect(world_mouse_pos);

		if (selected != nullptr)
		{
			selected->SetEntityStatus(ST_SELECTED);
			selection.push_back(selected);
		}
	}
	else if (selection.size() > MAX_SELECTION)
		selection.resize(MAX_SELECTION);

	if (App->scene->selection.size() == 1)
		App->scene->panel_info = (UI_Element*)App->gui->CreatePanel(App->scene->selection[0]);
	else if(App->scene->selection.size() > 1)
		App->scene->panel_info = (UI_Element*)App->gui->CreatePanel(App->scene->selection);
}

void j1EntityManager::UnselectEverything() const
{
	for (int i = 0; i < App->scene->selection.size(); i++)
		App->scene->selection[i]->SetEntityStatus(ST_NON_SELECTED);

	if (App->scene->selection.size() == 1)
		App->gui->DeletePanelInfo((UI_PanelInfoSingleEntity*)App->scene->panel_info);
	else if(App->scene->selection.size() > 1)
		App->gui->DeletePanelInfo((UI_PanelInfoMultipleEntities*)App->scene->panel_info);

	App->scene->selection.clear();
}

/*
//TODO: This function is unused   Why???
void j1EntityManager::Select(Entity * select) const
{
	App->entity_manager->UnselectEverything();
	select->SetEntityStatus(ST_SELECTED);
	App->scene->selection.push_back(select);
	App->gui->CreatePanel(App->scene->selection);
	//App->uimanager->CreatePanelInfo(App->scene->selection);
}
*/

Entity * j1EntityManager::LookForEnemies(int pixel_range, iPoint pos, Side side, Entity* attacker, ENTITY_TYPE entity_type) const
{
	Entity* ret = nullptr;

	std::vector<Entity*> enemies_in_range;
	entity_quadtree->SearchForEnemies(pixel_range, pos, enemies_in_range, side, entity_type);

	if (enemies_in_range.size() == 0)
		return ret;

	float shortest_distance = VISION_RANGE;
	float current_distance = 0.0f;

	for (std::vector<Entity*>::iterator it = enemies_in_range.begin(); it != enemies_in_range.end(); ++it)
	{
		fPoint distance_vec((*it)->GetPixelPosition().x - pos.x, (*it)->GetPixelPosition().y - pos.y);
		current_distance = sqrtf(distance_vec.x * distance_vec.x + distance_vec.y * distance_vec.y);

		if (current_distance < shortest_distance)
		{
			if (attacker->GetEntityType() == E_UNIT)
			{
				if (App->pathfinding->FindClosestEmptyAttackTile((*it)->GetPosition(), ((Unit*)attacker)->GetRange(), attacker).y != -1)
				{
					ret = *it;
					shortest_distance = current_distance;
				}
			}
			else
			{
				ret = *it;
				shortest_distance = current_distance;
			}		
		}
	}
	return ret;
}

void j1EntityManager::CheckClick(int mouse_x, int mouse_y) const
{
	App->scene->selection.clear();
	iPoint click_point = App->render->ScreenToWorld(mouse_x, mouse_y);
	Entity* clicked = entity_quadtree->SearchFirst(1, click_point);
	if(clicked != nullptr)
		App->scene->selection.push_back(clicked);
}

bool j1EntityManager::AreUnitsInRect(const SDL_Rect & rect) const
{
	return (entity_quadtree->SearchFirstUnit(rect) != nullptr);
}

bool j1EntityManager::Update(float dt)
{
	entity_quadtree->UpdateAll(dt);
	return true;
}

bool j1EntityManager::PostUpdate()
{
	entity_quadtree->DeleteEntities();
	/*if (siegeram_destroyed == true)
	{
		App->entity_manager->CreateUnit(U_CHAMPION, { siegeram_pos.x + 10.0f, siegeram_pos.y		 }, S_ENEMY);
		App->entity_manager->CreateUnit(U_CHAMPION, { siegeram_pos.x - 10.0f, siegeram_pos.y		 }, S_ENEMY);
		App->entity_manager->CreateUnit(U_CHAMPION, { siegeram_pos.x,		  siegeram_pos.y + 10.0f }, S_ENEMY);
		App->entity_manager->CreateUnit(U_CHAMPION, { siegeram_pos.x,		  siegeram_pos.y - 10.0f }, S_ENEMY);
		siegeram_destroyed = false;
	}*/
	return true;
}

Entity * j1EntityManager::CheckForCombat(iPoint position, int range, Side side) const
{
	return entity_quadtree->SearchFirstEnemy(range, position, side);
}

Entity* j1EntityManager::CheckForObjective(iPoint position, int vision_range, Side side) const
{
	return entity_quadtree->SearchFirstEnemy(vision_range, position, side);
}

bool j1EntityManager::IsUnitInTile(const Unit* unit, const iPoint tile)const
{
	if (tile == App->map->WorldToMap(unit->GetX(), unit->GetY()))
		return true;
	return false;
}

void j1EntityManager::DrawQuadTree() const
{
	entity_quadtree->Draw();
}

/*
bool j1EntityManager::Load(pugi::xml_node& data)
{
	delete entity_quadtree;

	float m = App->map->data.height;
	float n = App->map->data.width;

	float map_h = (m + n) * 0.5f * App->map->data.tile_height + 75;//75 1 4 each tile
	float map_w = (m + n) * 0.5f * App->map->data.tile_width;

	float map_x = (m - n) * 0.5f * 0.5f * App->map->data.tile_width;
	float map_y = (m + n) * 0.5f * 0.5f * App->map->data.tile_height + 75 / 2; //75 1 4 each tile

																			   //IsoRect map(iPoint(map_x, map_y), map_w, map_h);
	IsoRect map(fPoint(map_x, map_y), map_w, map_h);
	entity_quadtree = new QuadTree(map);

	pugi::xml_node Buildingsload = data.child("buildings").first_child();
	pugi::xml_node Unitsload = data.child("units").first_child();
	pugi::xml_node Turretsload = data.child("turrets").first_child();
	pugi::xml_node Resourcesload = data.child("resources").first_child();

	while (Buildingsload != NULL)
	{
		LoadBuilding(Buildingsload);
		Buildingsload = Buildingsload.next_sibling();
	}

	while (Unitsload != NULL)
	{
		LoadUnit(Unitsload);
		Unitsload = Unitsload.next_sibling();
	}

	while (Turretsload != NULL)
	{
		LoadTurret(Turretsload);
		Turretsload = Turretsload.next_sibling();
	}

	while (Resourcesload != NULL)
	{
		LoadResource(Resourcesload);
		Resourcesload = Resourcesload.next_sibling();
	}

	pugi::xml_node AmountOfResources = data.child("resources_amount").first_child();

	App->scene->resources->LoadResourcesAmount(AmountOfResources);

	pugi::xml_node Score = data.child("score");

	App->score->Reset();
	App->score->SetScore(Score.attribute("points").as_int());
	App->score->SetEnemiesKilleds(Score.attribute("enemies_killeds").as_int());
	App->score->SetTime(Score.attribute("time_passed").as_int());
	App->wave_manager->ResetWave();
	App->wave_manager->SetWaveNum(Score.attribute("wave_num").as_int());

	pugi::xml_node Investigations = data.child("investigations");

	App->investigations->InstaUnlockInvestigation(INV_CAVALRY_ATTACK, (LEVEL)Investigations.attribute("inv_cavalry_attack").as_int());
	App->investigations->InstaUnlockInvestigation(INV_CAVALRY_DEFENSE,(LEVEL)Investigations.attribute("inv_cavalry_defense").as_int());

	App->investigations->InstaUnlockInvestigation(INV_ARCHERS_ATTACK, (LEVEL)Investigations.attribute("inv_archers_attack").as_int());
	App->investigations->InstaUnlockInvestigation(INV_ARCHERS_DEFENSE, (LEVEL)Investigations.attribute("inv_archers_defense").as_int());

	App->investigations->InstaUnlockInvestigation(INV_INFANTRY_ATTACK, (LEVEL)Investigations.attribute("inv_infantry_attack").as_int());
	App->investigations->InstaUnlockInvestigation(INV_INFANTRY_DEFENSE, (LEVEL)Investigations.attribute("inv_infantry_defense").as_int());

	App->investigations->InstaUnlockInvestigation(INV_FIRE_TOWER, (LEVEL)Investigations.attribute("inv_fire_tower").as_int());
	App->investigations->InstaUnlockInvestigation(INV_ICE_TOWER, (LEVEL)Investigations.attribute("inv_ice_tower").as_int());
	App->investigations->InstaUnlockInvestigation(INV_AIR_TOWER, (LEVEL)Investigations.attribute("inv_air_tower").as_int());

	App->investigations->SetInvestigationCost(INV_STONE, Investigations.attribute("inv_res_stone").as_int());
	App->investigations->SetInvestigationCost(INV_GOLD, Investigations.attribute("inv_res_gold").as_int());
	App->investigations->SetInvestigationCost(INV_WOOD, Investigations.attribute("inv_res_wood").as_int());
	App->investigations->SetInvestigationCost(INV_FOOD, Investigations.attribute("inv_res_food").as_int());

	return true;
}

void j1EntityManager::LoadResource(pugi::xml_node& data)
{
	pugi::xml_node actualresource = data;
	fPoint pos(actualresource.attribute("posx").as_int(), actualresource.attribute("posy").as_int());
	Resources* actualres = (Resources*)App->entity_manager->CreateResource(RESOURCE_TYPE(actualresource.attribute("resource_type").as_int()), pos, actualresource.attribute("amount_collected").as_int(), actualresource.attribute("collect_time").as_int());
	
	switch (actualres->GetResourceType())
	{
	case R_WOOD:
		App->scene->resources->SetWood(actualres);
		break;
	case R_FOOD:
		App->scene->resources->SetFood(actualres);
		break;
	case R_GOLD:
		App->scene->resources->SetGold(actualres);
		break;
	case R_STONE:
		App->scene->resources->SetStone(actualres);
		break;
	}
}

void j1EntityManager::LoadBuilding(pugi::xml_node& data)
{
	pugi::xml_node Actualbuilding = data;
	fPoint pos(Actualbuilding.attribute("posx").as_int(), Actualbuilding.attribute("posy").as_int());
	Building* actualbuild = (Building*)App->entity_manager->CreateBuilding(BUILDING_TYPE(Actualbuilding.attribute("building_type").as_int()), pos, true);
	actualbuild->SetHp(Actualbuilding.attribute("hp").as_int());
	actualbuild->BuildingComplete();
	if (actualbuild->GetBuildingType() == B_TOWNHALL)
	{
		App->scene->townhall = actualbuild;
	}
}

void j1EntityManager::LoadUnit(pugi::xml_node& data)
{
	pugi::xml_node Actualunit = data;
	fPoint pos(Actualunit.attribute("posx").as_int(), Actualunit.attribute("posy").as_int());
	Unit* actualunit = (Unit*)App->entity_manager->CreateUnit(UNIT_TYPE(Actualunit.attribute("unit_type").as_int()), pos, Side(Actualunit.attribute("side").as_int()));
	actualunit->SetHp(Actualunit.attribute("hp").as_int());
}

void j1EntityManager::LoadTurret(pugi::xml_node& data)
{
	pugi::xml_node Actualturret = data;
	fPoint pos(Actualturret.attribute("posx").as_int(), Actualturret.attribute("posy").as_int());
	Tower* actualturret = (Tower*)App->entity_manager->CreateTower(TOWER_TYPE(Actualturret.attribute("tower_type").as_int()), pos);
	actualturret->SetHp(Actualturret.attribute("hp").as_int());
	actualturret->BuildingComplete();
}

bool j1EntityManager::Save(pugi::xml_node &data) const
{
	pugi::xml_node Saved = data.append_child("Saved_Game");
	Saved.append_attribute("saved_game") = 1;
	pugi::xml_node Buildings = data.append_child("buildings");
	pugi::xml_node Units = data.append_child("units");
	pugi::xml_node Turrets = data.append_child("turrets");
	pugi::xml_node Resourcess = data.append_child("resources");

	entity_quadtree->SaveAll(data);


	pugi::xml_node AmountOfResources = data.append_child("resources_amount");

	App->scene->resources->SaveResourcesAmount(AmountOfResources);

	pugi::xml_node Score = data.append_child("score");

	Score.append_attribute("points") = App->score->GetScore();
	Score.append_attribute("enemies_killeds") = App->score->GetEnemiesKilled();
	Score.append_attribute("time_passed") = App->score->GetTime();
	Score.append_attribute("wave_num") = App->wave_manager->GetWaveNum();

	pugi::xml_node Investigations = data.append_child("investigations");

	Investigations.append_attribute("inv_cavalry_attack") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_CAVALRY_ATTACK));
	Investigations.append_attribute("inv_cavalry_defense") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_CAVALRY_DEFENSE));

	Investigations.append_attribute("inv_archers_attack") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ARCHERS_ATTACK));
	Investigations.append_attribute("inv_archers_defense") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ARCHERS_DEFENSE));

	Investigations.append_attribute("inv_infantry_attack") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_INFANTRY_ATTACK));
	Investigations.append_attribute("inv_infantry_defense") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_INFANTRY_DEFENSE));

	Investigations.append_attribute("inv_fire_tower") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER));
	Investigations.append_attribute("inv_ice_tower") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ICE_TOWER));
	Investigations.append_attribute("inv_air_tower") = App->investigations->GetLevel(App->investigations->GetInvestigation(INV_AIR_TOWER));

	Investigations.append_attribute("inv_res_stone") = App->investigations->GetInvestigationCost(App->investigations->GetInvestigation(INV_STONE));
	Investigations.append_attribute("inv_res_gold") = App->investigations->GetInvestigationCost(App->investigations->GetInvestigation(INV_GOLD));
	Investigations.append_attribute("inv_res_wood") = App->investigations->GetInvestigationCost(App->investigations->GetInvestigation(INV_WOOD));
	Investigations.append_attribute("inv_res_food") = App->investigations->GetInvestigationCost(App->investigations->GetInvestigation(INV_FOOD));


	return true;
}*/

void j1EntityManager::DropUnits(float pos_x, float pos_y)
{
	siegeram_destroyed = true;
	siegeram_pos.x = pos_x;
	siegeram_pos.y = pos_y;
}

/*void j1EntityManager::BlitMinimap() const
{
	entity_quadtree->BlitMinimap();
}*/

void j1EntityManager::GetEntitiesInIsoRect(const IsoRect rect, std::vector<Entity*>& vec) const
{
	entity_quadtree->SearchInIsoRect(rect, vec);
}

Entity * j1EntityManager::ClickSelect(const iPoint & mouse_pos) const
{
	return entity_quadtree->ClickSelect(mouse_pos);
}
