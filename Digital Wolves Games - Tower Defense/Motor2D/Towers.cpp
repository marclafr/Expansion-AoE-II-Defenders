#include "Towers.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "j1Animation.h"

Tower::Tower(TOWER_TYPE t_type, fPoint pos) : Building(B_TURRET, pos, S_ALLY), tower_type(t_type)
{
	anim_fire = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_FLOOR));
	switch (t_type)
	{
	case T_BASIC_TOWER:
		SetHp(250);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 100.0f;
		range = 250;
		SetTextureID(T_TURRET);
		tower_type = T_BASIC_TOWER;
		break;

	case T_BOMBARD_TOWER:
		SetHp(250);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 100.0f;
		range = 250;
		SetTextureID(T_TURRET);
		tower_type = T_BOMBARD_TOWER;
		break;

	default:
		LOG("Error BUILDING TYPE STATS NULL");
		tower_type = T_NO_TYPE;
		break;
	}
}

Tower::~Tower()
{
}

void Tower::Update()
{
	if (IsBuilt() == true && IsAlive() == true)
		AI();

	if (Target != nullptr) {
		if (Target->GetHp() <= 0) {
			Target = nullptr;
			ResetArrowPos();
		}
		else
			UpdateArrow(HEIGHT_BASIC_TOWER, Target->GetPosition(), 100, 110);
		if (GetArrowPos() == 1)
		{
			ResetArrowPos();
			Target->Damaged(GetAttack());
			element_terrain_pos = Target->GetPosition();
			print_element_terrain = true;
			PrintElementTerrainTimer.Start();
		}
	}

	if (IsAlive() == true && this->GetHp() <= 0)
		ConvertToRubble();

	if (IsAlive() == false && GetDieTime() >= 2)
		DestroyBuilding();
	
	Draw();
}

void Tower::AI()
{

	if (Target == nullptr)
	{
		attacking = false;
		std::vector<Entity*> EntityVector = App->entity_manager->GetEntityVector();
		std::vector<Entity*>::iterator item = EntityVector.begin();
		for (; item != EntityVector.end(); item++)
		{
			if ((*item)->GetEntityType() == E_UNIT)
			{
				//TODO:Use a function from entity instead
				if ((*item)->GetX() >= (GetX() - GetRange()) && (*item)->GetX() < (GetX() + GetRange()) && (*item)->GetY() >= (GetY() - GetRange()) && (*item)->GetY() < (GetY() + GetRange()) && (*item)->GetHp() > 0 && (*item)->GetSide() == S_ENEMY)
				{
					Target = *item;
					AttackTimer.Start();
					App->audio->PlayFx(App->entity_manager->fx_arrow);
				}
			}
		}
	}
	else 
	{
		if (Target->GetHp() <= 0)
		{
			Target = nullptr;
			attacking = false;
		}
		else
		{
			if (Target != nullptr && Target->GetSide() != S_ALLY && Target->GetHp() > 0)
			{
				if (Target->GetX() >= (GetX() - GetRange()) && Target->GetX() < (GetX() + GetRange()) && Target->GetY() >= (GetY() - GetRange()) && Target->GetY() < (GetY() + GetRange()))
				{
					arrowpos.x = GetX();
					arrowpos.y = GetY() - 100;
				}
				else
				{
					Target = nullptr;
				}
			}
		}
	}
}

void Tower::Draw()
{
	if (print_element_terrain == true && PrintElementTerrainTimer.ReadSec() <= ELEMENT_TERRAIN_TIME)
		PrintElementTerrain(GetElementFromTower(tower_type), element_terrain_pos, 30);

	if (IsBuilt())
		App->render->PushEntity(this);

	else
	{
		if (GetBuildTime() <= 3)
		{
			SDL_Rect rect = { 394,1,96,64 };
			SetRect(rect);
			SetPivot(0.53125 * 96, 0.59375 * 64);

		}
		else if (GetBuildTime() > 3 && GetBuildTime() <= 6)
		{
			SDL_Rect rect = { 376,539,100,73 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 73);

		}
		else if (GetBuildTime() > 6 && GetBuildTime() <= 9)
		{
			SDL_Rect rect = { 478,539,100,73 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 73);


		}
		else if (GetBuildTime() > 9)
		{
			switch (tower_type)
			{
			case T_BASIC_TOWER:
				SetRect({ 610,1,107,206 });
				SetPivot(0.504673 * 107, 0.902913 * 206);
				break;

			case T_BOMBARD_TOWER:
				SetRect({ 154,1,128,281 });
				SetPivot(0.5 * 128,0.914591 * 281);
				break;

			default:
				break;
			}
			BuildingComplete();
		}

		App->render->PushEntity(this);
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

void Tower::UpgradeTurret()
{
	/*SDL_Rect rect;
	switch (this->lvl)
	{
	case INV_LVL_1:
		rect = { 110,284,104,253 };
		SetRect(rect);
		this->SetAttack(200);
		SetPivot(0.519231 * 104, 0.905138 * 253);
		lvl = INV_LVL_2;
		break;
	case INV_LVL_2:
		rect = { 216,284,119,226 };
		SetRect(rect);
		SetPivot(0.504202 * 119, 0.893805 * 226);
		lvl = INV_LVL_3;
		break;
	case INV_LVL_3:
		//maxlvl�?�?�?
		break;
	}*/
}

void Tower::PrintElementTerrain(TOWER_ELEMENT_TYPE element, fPoint center, int radius)
{
	SDL_Rect rect;
	iPoint pivot;
	anim_fire->Update(rect, pivot);
	App->render->Blit(App->tex->GetTexture(T_FIRE), center.x, center.y, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);



}


TOWER_ELEMENT_TYPE Tower::GetElementFromTower(TOWER_TYPE tower)
{
	switch (tower)
	{
	case T_BASIC_TOWER:
	case T_BOMBARD_TOWER:
		return TE_NO_ELEMENT;
		break;
	case T_FIRE_TOWER:
	case T_BOMBARD_FIRE_TOWER:
		return TE_FIRE;
		break;
	case T_ICE_TOWER:
	case T_BOMBARD_ICE_TOWER:
		return TE_ICE;
		break;
	case T_AIR_TOWER:
	case T_BOMBARD_AIR_TOWER:
		return TE_AIR;
		break;
	default:
		break;
	}
	return TE_NULL;
}