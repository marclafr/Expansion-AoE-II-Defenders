/*#include "Towers.h"
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
#include "j1Scene.h"

Tower::Tower(TOWER_TYPE t_type, iPoint pos) : Building(B_TURRET, pos, S_ALLY), tower_type(t_type)
{
	SDL_Rect tower_rect;
	iPoint pivot;
	SDL_Texture* text;
	switch (t_type)
	{
	case T_BASIC_TOWER:
		SetHp(150);
		SetMaxHP(150);
		SetAttack(12);
		SetArmor(2);
		rate_of_fire = 0.60f;	//time between each attack in seconds
		range = 310;
		tower_type = T_BASIC_TOWER;
		projectile_type = P_BASIC_ARROW;
		SetBuildingType(B_TURRET);
		projectile_spd = 75;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BASIC_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_BOMBARD_TOWER:
		SetHp(175);
		SetMaxHP(175);
		SetAttack(26);
		SetArmor(4);
		rate_of_fire = 1.95f;
		range = 310;
		tower_type = T_BOMBARD_TOWER;
		projectile_type = P_CANNONBALL;
		SetBuildingType(B_CANNON);
		projectile_spd = 80;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_FIRE_TOWER:
		SetHp(190);
		SetMaxHP(190);
		SetAttack(25);
		SetArmor(1);
		rate_of_fire = 0.85f;	//time between each attack in seconds
		range = 250;
		tower_type = T_FIRE_TOWER;
		projectile_type = P_FIRE_ARROW;
		SetBuildingType(B_TURRET_UPGRADED_FIRE);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_FIRE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_ICE_TOWER:
		SetHp(175);
		SetMaxHP(175);
		SetAttack(20);
		SetArmor(1);
		rate_of_fire = 0.88;	//time between each attack in seconds
		range = 300;
		tower_type = T_ICE_TOWER;
		projectile_type = P_ICE_ARROW;
		SetBuildingType(B_TURRET_UPGRADED_ICE);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_ICE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_AIR_TOWER:
		SetHp(165);
		SetMaxHP(165);
		SetAttack(11);
		SetArmor(1);
		rate_of_fire = 0.55f;	//time between each attack in seconds
		range = 350;
		tower_type = T_AIR_TOWER;
		projectile_type = P_AIR_ARROW;
		SetBuildingType(B_TURRET_UPGRADED_AIR);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_AIR_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_BOMBARD_FIRE_TOWER:
		SetHp(225);
		SetMaxHP(225);
		SetAttack(33);
		SetArmor(4);
		rate_of_fire = 1.95f;	//time between each attack in seconds
		range = 290;
		tower_type = T_BOMBARD_FIRE_TOWER;
		projectile_type = P_FIRE_CANNONBALL;
		SetBuildingType(B_CANNON_UPGRADED_FIRE);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_FIRE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_BOMBARD_ICE_TOWER:
		SetHp(195);
		SetMaxHP(195);
		SetAttack(28);
		SetArmor(4);
		rate_of_fire = 1.90f;	//time between each attack in seconds
		range = 290;
		tower_type = T_BOMBARD_ICE_TOWER;
		projectile_type = P_ICE_CANNONBALL;
		SetBuildingType(B_CANNON_UPGRADED_ICE);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_ICE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_BOMBARD_AIR_TOWER:
		SetHp(175);
		SetMaxHP(175);
		SetAttack(21);
		SetArmor(4);
		rate_of_fire = 1.45f;	//time between each attack in seconds
		range = 325;
		tower_type = T_BOMBARD_AIR_TOWER;
		projectile_type = P_AIR_CANNONBALL;
		SetBuildingType(B_CANNON_UPGRADED_AIR);
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
	AttackTimer.Start();

	iPoint tile = App->map->WorldToMap(pos.x, pos.y);
	App->pathfinding->MakeNoWalkable(tile);
	App->pathfinding->MakeNoConstruible_neutral(tile);
	App->pathfinding->MakeNoConstruible_ally(tile);

	Position_rect = { (int)pos.x - (tower_rect.w/2),(int)pos.y - tower_rect.h, tower_rect.w, tower_rect.h };

}

Tower::~Tower()
{
}

void Tower::Update(float dt)
{
	DT(dt);

	if (GetAIDT() >= dt * 15)
	{
		ResetDT();
		AI();
	}

	Draw();
}

void Tower::AI()
{
	if (Target != nullptr)
	{
		if (Target->GetX() < (GetX() - range) || Target->GetX() > (GetX() + range) || Target->GetY() < (GetY() - range) || Target->GetY() > (GetY() + range))
			Target = nullptr;
		if (Target != nullptr && Target->GetHp() <= 0)
		{
			Target = nullptr;
			attacking = false;
		}

	}
	else
		attacking = false;

	if (Target == nullptr && AttackTimer.ReadSec() >= rate_of_fire && attacking == false && IsBuilt() == true && IsAlive() == true)
	{
		Target = App->entity_manager->LookForEnemies(GetRange(), GetPosition(), GetSide(), this);
		if (Target != nullptr)
			attacking = true;
	}

	if (attacking == true && Target != nullptr && AttackTimer.ReadSec() >= rate_of_fire)
	{
		App->projectile_manager->CreateProjectile(GetPosition(), Target, GetAttack(), projectile_spd, HEIGHT_BASIC_TOWER, 100, projectile_type);
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY()))
		{
			if (tower_type == T_BASIC_TOWER || tower_type == T_ICE_TOWER || tower_type == T_AIR_TOWER || tower_type == T_FIRE_TOWER)
				App->audio->PlayFx(App->audio->fx_arrow);
			else
				App->audio->PlayFx(App->audio->fx_cannon);
		}
		AttackTimer.Start();
	}

	if (IsAlive() == true && this->GetHp() <= 0)
	{
		Target = nullptr;
		ConvertToRubble();
		ConvertTowerToRubble();
		App->pathfinding->MakeWalkable(App->map->WorldToMap(GetPosition().x, GetPosition().y));
	}
	if (IsAlive() == false && GetDieTime() >= 2)
		DestroyTower();
}

void Tower::Draw()
{
	if (IsBuilt())
	{
		if (App->entity_manager->AreUnitsInRect(Position_rect))
		{
			SetTextureID(T_TURRET_ALPHA_DOWN);
		}
		else
		{
			SetTextureID(T_TURRET);
		}
	if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY())) App->render->PushInGameSprite(this);
		if (GetHp() <= GetMaxHp() / 2) //TOWERS FIRE
		{
			SDL_Rect fire_rect;
			iPoint fire_pivot;
			building_fire->Update(fire_rect, fire_pivot);
			App->render->PushInGameSprite(App->tex->GetTexture(T_BUILDINGS_FIRE), GetX(), GetY() + 100, &fire_rect, SDL_FLIP_NONE, fire_pivot.x, fire_pivot.y + 100);
		}
	}
	else
	{
		if (GetBuildTime() <= 3)
		{
			SDL_Rect rect = { 0,0,96,65 };
			SetRect(rect);
			SetPivot(0.53125 * 96, 0.59375 * 65);

		}
		else if (GetBuildTime() > 3 && GetBuildTime() <= 6)
		{
			SDL_Rect rect = { 98,0,100,75 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 75);

		}
		else if (GetBuildTime() > 6 && GetBuildTime() <= 9)
		{
			SDL_Rect rect = { 202,0,100,75 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 75);
		}
		else if (GetBuildTime() > 9)
		{
			switch (tower_type)
			{
			case T_BASIC_TOWER:
				SetRect({ 302,0,107,208 });
				SetPivot(0.504673 * 107, 0.902913 * 208);
				Position_rect = { (int)GetX()- (GetRect().w / 2),(int)GetY() - GetRect().h, GetRect().w, GetRect().h };
				break;

			case T_BOMBARD_TOWER:
				SetRect({ 629,0,130,281 });
				SetPivot(0.5 * 130, 0.914591 * 281);
				Position_rect = { (int)GetX() - (GetRect().w / 2),(int)GetY() - GetRect().h, GetRect().w, GetRect().h };
				break;

			default:
				break;
			}
			BuildingComplete();
			AttackTimer.Start();
		}
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY()))   App->render->PushInGameSprite(this);
	}
}

const TOWER_TYPE Tower::GetTowerType() const
{
	return tower_type;
}

const int Tower::GetRange() const
{
	return range;
}

void Tower::Save(pugi::xml_node &data)
{
	pugi::xml_node build = data.child("turrets");
	pugi::xml_node ActualTurret = build.append_child("turret");
	ActualTurret.append_attribute("tower_type") = GetTowerType();
	ActualTurret.append_attribute("posx") = GetX();
	ActualTurret.append_attribute("posy") = GetY();
	ActualTurret.append_attribute("hp") = GetHp();
}

float Tower::GetSpeed()
{
	return rate_of_fire;

}

void Tower::DestroyTower()
{
	iPoint tile = App->map->WorldToMap(GetPosition().x, GetPosition().y);
	App->pathfinding->MakeConstruible_neutral(tile);
	App->pathfinding->MakeConstruible_ally(tile);
	this->Die();
}

void Tower::SetSpeed(float new_speed)
{
	if (new_speed<rate_of_fire)
		rate_of_fire -= new_speed;
}

void Tower::SetRange(float new_range)
{
	range += new_range;
}

void Tower::ConvertTowerToRubble()
{
	SDL_Rect rect;
	rect = { 1029, 520, 93, 53 };
	SetRect(rect);
	SetPivot(0.362637 * 91, 0.431373 * 51);
}

/*
void Tower::UpgradeTurret(INVESTIGATION_TYPE type)
{
	if (this->IsBuilt())
	{
		SDL_Rect tower_rect;
		iPoint pivot;
		SDL_Texture* text;
		if (GetTowerType() == T_BASIC_TOWER)
		{
			switch (type)
			{
			case INV_FIRE_TOWER:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED && App->scene->resources->CanUpgradeTower(T_FIRE_TOWER) == true)
				{
					App->scene->resources->UpgradeTower(T_FIRE_TOWER);
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_FIRE_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_FIRE_ARROW;
					tower_type = T_FIRE_TOWER;
					SetBuildingType(B_TURRET_UPGRADED_FIRE);
					SetAttack(GetAttack() + 12);
					SetHp(190);
					SetRange(-0.50);
					Position_rect = { (int)GetX() - (tower_rect.w / 2), (int)GetY() - tower_rect.h, tower_rect.w, tower_rect.h };
				}
				break;
			case INV_ICE_TOWER:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ICE_TOWER)) == INV_LVL_UNLOCKED && App->scene->resources->CanUpgradeTower(T_ICE_TOWER) == true)
				{
					App->scene->resources->UpgradeTower(T_ICE_TOWER);
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_ICE_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_ICE_ARROW;
					tower_type = T_ICE_TOWER;
					SetBuildingType(B_TURRET_UPGRADED_ICE);
					SetAttack(GetAttack() + 5);
					SetHp(175);
					SetSpeed(0.88f);
					Position_rect = { (int)GetX() - (tower_rect.w / 2), (int)GetY() - tower_rect.h, tower_rect.w, tower_rect.h };
				}
				break;
			case INV_AIR_TOWER:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_AIR_TOWER)) == INV_LVL_UNLOCKED && App->scene->resources->CanUpgradeTower(T_AIR_TOWER) == true)
				{
					App->scene->resources->UpgradeTower(T_AIR_TOWER);
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_AIR_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_AIR_ARROW;
					tower_type = T_AIR_TOWER;
					SetBuildingType(B_TURRET_UPGRADED_AIR);
					SetAttack(GetAttack() - 6);
					SetSpeed(0.3f);
					SetRange(90);
					Position_rect = { (int)GetX() - (tower_rect.w / 2),  (int)GetY() - tower_rect.h, tower_rect.w, tower_rect.h };
				}
				break;
			default:
				break;
			}
		}
		else if (GetTowerType() == T_BOMBARD_TOWER)
		{
			switch (type)
			{
			case INV_FIRE_TOWER:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED && App->scene->resources->CanUpgradeTower(T_FIRE_TOWER) == true)
				{
					App->scene->resources->UpgradeTower(T_FIRE_TOWER);
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_FIRE_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_FIRE_CANNONBALL;
					tower_type = T_BOMBARD_FIRE_TOWER;
					SetBuildingType(B_CANNON_UPGRADED_FIRE);
					SetAttack(GetAttack() + 7);
					SetHp(225);
					SetRange(-10);
					Position_rect = { (int)GetX() - (tower_rect.w / 2),  (int)GetY() - tower_rect.h, tower_rect.w, tower_rect.h };
				}
				break;
			case INV_ICE_TOWER:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ICE_TOWER)) == INV_LVL_UNLOCKED && App->scene->resources->CanUpgradeTower(T_ICE_TOWER) == true)
				{
					App->scene->resources->UpgradeTower(T_ICE_TOWER);
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_ICE_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_ICE_CANNONBALL;
					tower_type = T_BOMBARD_ICE_TOWER;
					SetBuildingType(B_CANNON_UPGRADED_ICE);
					SetAttack(GetAttack() + 2);
					SetHp(195);
					Position_rect = { (int)GetX() - (tower_rect.w / 2),  (int)GetY() - tower_rect.h, tower_rect.w, tower_rect.h };
				}
				break;
			case INV_AIR_TOWER:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_AIR_TOWER)) == INV_LVL_UNLOCKED && App->scene->resources->CanUpgradeTower(T_AIR_TOWER) == true)
				{
					App->scene->resources->UpgradeTower(T_AIR_TOWER);
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_AIR_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_AIR_CANNONBALL;
					tower_type = T_BOMBARD_AIR_TOWER;
					SetBuildingType(B_CANNON_UPGRADED_AIR);
					SetAttack(GetAttack() - 5);
					SetHp(175);
					SetRange(40);
					SetSpeed(0.60f);
					Position_rect = { (int)GetX() - (tower_rect.w / 2), (int)GetY() - tower_rect.h, tower_rect.w, tower_rect.h };
				}
				break;
			default:
				break;
			}
		}
	}
}
*/
