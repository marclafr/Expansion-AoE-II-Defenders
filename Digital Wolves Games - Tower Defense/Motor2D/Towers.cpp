#include "Towers.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "ProjectileManager.h"
#include "j1Animation.h"
#include "j1Input.h"
#include "j1Map.h"
#include "Camera.h"
#include "j1Investigations.h"
#include "j1Scene.h"

Tower::Tower(TOWER_TYPE t_type, iPoint pos) : Building(B_TURRET, pos, S_ALLY), tower_type(t_type)
{
	SDL_Rect tower_rect;
	iPoint pivot;
	SDL_Texture* text;
	switch (t_type)
	{
	case T_BASIC_TOWER:
		SetLife(150);
		SetAttack(12);
		SetArmor(2);
		rate_of_fire = 0.60f;	//time between each attack in seconds
		range = 310;
		tower_type = T_BASIC_TOWER;
		projectile_type = P_BASIC_ARROW;
		projectile_spd = 75;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BASIC_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_BOMBARD_TOWER:
		SetLife(175);
		SetAttack(26);
		SetArmor(4);
		rate_of_fire = 1.95f;
		range = 310;
		tower_type = T_BOMBARD_TOWER;
		projectile_type = P_CANNONBALL;
		projectile_spd = 80;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_FIRE_TOWER:
		SetLife(190);
		SetAttack(25);
		SetArmor(1);
		rate_of_fire = 0.85f;	//time between each attack in seconds
		range = 250;
		tower_type = T_FIRE_TOWER;
		projectile_type = P_FIRE_ARROW;
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_FIRE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_ICE_TOWER:
		SetLife(175);
		SetAttack(20);
		SetArmor(1);
		rate_of_fire = 0.88;	//time between each attack in seconds
		range = 300;
		tower_type = T_ICE_TOWER;
		projectile_type = P_ICE_ARROW;
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_ICE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_AIR_TOWER:
		SetLife(165);
		SetAttack(11);
		SetArmor(1);
		rate_of_fire = 0.55f;	//time between each attack in seconds
		range = 350;
		tower_type = T_AIR_TOWER;
		projectile_type = P_AIR_ARROW;
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_AIR_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_BOMBARD_FIRE_TOWER:
		SetLife(225);
		SetAttack(33);
		SetArmor(4);
		rate_of_fire = 1.95f;	//time between each attack in seconds
		range = 290;
		tower_type = T_BOMBARD_FIRE_TOWER;
		projectile_type = P_FIRE_CANNONBALL;
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_FIRE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_BOMBARD_ICE_TOWER:
		SetLife(195);
		SetAttack(28);
		SetArmor(4);
		rate_of_fire = 1.90f;	//time between each attack in seconds
		range = 290;
		tower_type = T_BOMBARD_ICE_TOWER;
		projectile_type = P_ICE_CANNONBALL;
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_ICE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_BOMBARD_AIR_TOWER:
		SetLife(175);
		SetAttack(21);
		SetArmor(4);
		rate_of_fire = 1.45f;	//time between each attack in seconds
		range = 325;
		tower_type = T_BOMBARD_AIR_TOWER;
		projectile_type = P_AIR_CANNONBALL;
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_AIR_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	default:
		LOG("Error BUILDING TYPE STATS NULL");
		tower_type = T_NO_TYPE;
		break;
	}
	SetTextureID(T_TURRET);
	attack_timer.Start();

	iPoint tile = App->map->WorldToMap(pos.x, pos.y);
	App->pathfinding->MakeNoWalkable(tile);
}

Tower::~Tower()
{}

void Tower::DoBattle()
{
	if (target != nullptr)
	{
		if (target->GetPixelPosition().DistanceTo(GetPixelPosition()) > range)
			target = nullptr;

		if (target != nullptr && target->GetHp() <= 0)
			target = nullptr;
	}

	if (target == nullptr)
	{
		target = (Unit*)App->entity_manager->LookForEnemies(GetRange(), GetPosition(), GetSide(), this);
		if (target != nullptr)
			Attack();
	}
}

void Tower::Attack()
{
	if (attack_timer.ReadSec() >= rate_of_fire)
	{
		attack_timer.Start();

		//App->projectile_manager->CreateProjectile(GetProjectilePosition(), target, GetAttack(), projectile_spd, HEIGHT_BASIC_TOWER, 100, projectile_type);
		// for now
		App->projectile_manager->CreateProjectile(GetPixelPosition(), target, GetAttack(), projectile_spd, HEIGHT_BASIC_TOWER, 100, projectile_type);
		
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + left_tile.x, App->render->camera->GetPosition().y + left_tile.y))
		{
			if (tower_type == T_BASIC_TOWER || tower_type == T_ICE_TOWER || tower_type == T_AIR_TOWER || tower_type == T_FIRE_TOWER)
				App->audio->PlayFx(App->audio->fx_arrow);
			else
				App->audio->PlayFx(App->audio->fx_cannon);
		}
	}
}

void Tower::ChangeTexture()
{
	SDL_Rect rect;
	iPoint pivot;
	SDL_Texture* text;

	switch (state)
	{
	case BS_IN_CONSTRUCTION_BEGINING:
		rect = { 0,0,96,65 };
		SetRect(rect);
		SetPivot(0.53125 * 96, 0.59375 * 65);
		break;

	case BS_IN_CONSTRUCTION_MIDDLE:
		rect = { 98,0,100,75 };
		SetRect(rect);
		SetPivot(0.55 * 100, 0.643836 * 75);
		break;

	case BS_IN_CONSTRUCTION_END:
		rect = { 202,0,100,75 };
		SetRect(rect);
		SetPivot(0.55 * 100, 0.643836 * 75);
		break;

	case BS_BUILT:
		switch (tower_type)
		{
		case T_BASIC_TOWER:
			SetRect({ 302,0,107,208 });
			SetPivot(0.504673 * 107, 0.902913 * 208);
			break;

		case T_FIRE_TOWER:
			App->tex->GetTowerTexture(text, rect, pivot, T_FIRE_TOWER);
			SetRect(rect);
			SetPivot(pivot.x, pivot.y);
			break;

		case T_ICE_TOWER:
			App->tex->GetTowerTexture(text, rect, pivot, T_ICE_TOWER);
			SetRect(rect);
			SetPivot(pivot.x, pivot.y);
			break;

		case T_AIR_TOWER:
			App->tex->GetTowerTexture(text, rect, pivot, T_AIR_TOWER);
			SetRect(rect);
			SetPivot(pivot.x, pivot.y);
			break;

		case T_BOMBARD_TOWER:
			SetRect({ 629,0,130,281 });
			SetPivot(0.5 * 130, 0.914591 * 281);
			break;
			
		case T_BOMBARD_FIRE_TOWER:
			App->tex->GetTowerTexture(text, rect, pivot, T_BOMBARD_FIRE_TOWER);
			SetRect(rect);
			SetPivot(pivot.x, pivot.y);
			break;

		case T_BOMBARD_ICE_TOWER:
			App->tex->GetTowerTexture(text, rect, pivot, T_BOMBARD_ICE_TOWER);
			SetRect(rect);
			SetPivot(pivot.x, pivot.y);
			break;

		case T_BOMBARD_AIR_TOWER:
			App->tex->GetTowerTexture(text, rect, pivot, T_BOMBARD_AIR_TOWER);
			SetRect(rect);
			SetPivot(pivot.x, pivot.y);
			break;

		default:
			LOG("Error TOWER TYPE NULL, can't set texture");
			break;
		}
		break;

	case BS_DESTROYED:
		rect = { 1029, 520, 93, 53 };
		SetRect(rect);
		SetPivot(0.362637 * 91, 0.431373 * 51);
		break;

	default:
		LOG("Can not change building texture with null building state");
		break;
	}
}

void Tower::StartTimers()
{
	attack_timer.Start();
}

/* Useless, only here for fire animations reference (destruction animations not attack). Fires for destruction should be done in buildings
void Tower::Draw()
{
	if (GetHp() <= GetMaxHp() / 2) //TOWERS FIRE
	{
		SDL_Rect fire_rect;
		iPoint fire_pivot;
		building_fire->Update(fire_rect, fire_pivot);
		App->render->PushInGameSprite(App->tex->GetTexture(T_BUILDINGS_FIRE), GetX(), GetY() + 100, &fire_rect, SDL_FLIP_NONE, fire_pivot.x, fire_pivot.y + 100);
	}
}*/

const TOWER_TYPE Tower::GetTowerType() const
{
	return tower_type;
}

const int Tower::GetRange() const
{
	return range;
}

void Tower::Destroyed()
{
	state = BS_DESTROYED;
	state_changed = true;
	if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + left_tile.x, App->render->camera->GetPosition().y + left_tile.y))
		App->audio->PlayFx(App->audio->fx_building_destroyed);
	DieTimer.Start();
}

/*void Tower::Save(pugi::xml_node &data)
{
	pugi::xml_node build = data.child("turrets");
	pugi::xml_node ActualTurret = build.append_child("turret");
	ActualTurret.append_attribute("tower_type") = GetTowerType();
	ActualTurret.append_attribute("posx") = left_tile.x;
	ActualTurret.append_attribute("posy") = left_tile.y;
	ActualTurret.append_attribute("hp") = GetHp();
}*/

float Tower::GetAttackSpeed()
{
	return rate_of_fire;
}

void Tower::SetSpeed(float new_speed)
{
	rate_of_fire = new_speed;
}

void Tower::SetRange(float new_range)
{
	range = new_range;
}

void Tower::ChangeProjectiles(TOWER_TYPE new_type)
{
	switch (new_type)
	{
	case T_FIRE_TOWER:
		projectile_type = P_FIRE_ARROW;
		break;

	case T_BOMBARD_FIRE_TOWER:
		projectile_type = P_FIRE_CANNONBALL;
		break;

	case T_ICE_TOWER:
		projectile_type = P_ICE_ARROW;
		break;

	case T_BOMBARD_ICE_TOWER:
		projectile_type = P_ICE_CANNONBALL;
		break;

	case T_AIR_TOWER:
		projectile_type = P_AIR_ARROW;
		break;

	case T_BOMBARD_AIR_TOWER:
		projectile_type = P_AIR_CANNONBALL;
		break;
	}
}

void Tower::NewSpeed(TOWER_TYPE new_type)
{
	float variation = 0.0f;

	switch (new_type)
	{
	case T_ICE_TOWER:
		variation = 0.88f;
		break;

	case T_BOMBARD_ICE_TOWER:
		break;

	case T_AIR_TOWER:
		variation = 0.3f;
		break;

	case T_BOMBARD_AIR_TOWER:
		variation = 0.60f;
		break;
	}
	rate_of_fire -= variation;
}

void Tower::NewLife(TOWER_TYPE new_type)
{
	switch (new_type)
	{
	case T_FIRE_TOWER:
		SetLife(190);
		break;

	case T_BOMBARD_FIRE_TOWER:
		SetLife(225);		
		break;

	case T_ICE_TOWER:
		SetLife(175);
		break;

	case T_BOMBARD_ICE_TOWER:
		SetLife(195);
		break;

	case T_AIR_TOWER:
		SetLife(165);
		break;

	case T_BOMBARD_AIR_TOWER:
		SetLife(175);
		break;
	}
}

void Tower::NewAttack(TOWER_TYPE new_type)
{
	switch (new_type)
	{
	case T_FIRE_TOWER:
		SetAttack(GetAttack() + 12);
		break;

	case T_BOMBARD_FIRE_TOWER:	
		SetAttack(GetAttack() + 7);
		break;

	case T_ICE_TOWER:
		SetAttack(GetAttack() + 5);
		break;

	case T_BOMBARD_ICE_TOWER:
		SetAttack(GetAttack() + 2);
		break;

	case T_AIR_TOWER:
		SetAttack(GetAttack() - 6);
		break;

	case T_BOMBARD_AIR_TOWER:
		SetAttack(GetAttack() - 5);
		break;
	}
}

void Tower::NewRange(TOWER_TYPE new_type)
{
	float variation = 0.0f;

	switch (new_type)
	{
	case T_FIRE_TOWER:
		variation = -0.50;
		break;

	case T_BOMBARD_FIRE_TOWER:
		variation = -10;
		break;

	case T_AIR_TOWER:
		variation = 90;
		break;

	case T_BOMBARD_AIR_TOWER:
		variation = 40;		
		break;
	}
	range += variation;
}

bool Tower::UpgradeTurret(TOWER_TYPE new_type)
{	
	if (CanUpgrade(new_type))
	{	
		int cost = App->scene->gold->GetTowerUpgradeCost(new_type);
		App->scene->gold->UseGold(cost);
		tower_type = new_type;
		state_changed = true;
		ChangeProjectiles(new_type);
		NewSpeed(new_type);
		NewLife(new_type);
		NewAttack(new_type);
		NewRange(new_type);
		return true;
	}
	return false;
}

bool Tower::CanUpgrade(TOWER_TYPE new_type)
{
	return App->investigations->CanUpgradeTower(new_type) && App->scene->gold->CanUpgradeTower(new_type);
}

