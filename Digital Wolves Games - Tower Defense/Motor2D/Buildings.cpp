#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"




Building::Building(BUILDING_TYPE b_type, fPoint pos, Side side) : Entity(E_BUILDING, pos, side), building_type(b_type)
{
	SDL_Rect rect;
	switch (b_type)
	{
	case B_TURRET:
		SetSide(side);
		SetHp(1000);
		SetAttack(12);
		SetArmor(1);
		rate_of_fire = 1;
		range = 120;
		build_time = 7;
		rect = {610,1,107,206};
		SetRect(rect);
		SetPivot(0.504673 * 107, 0.902913 * 206);
		SetTextureID(T_TURRET);
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		building_type = B_NO_BUILDING;
		break;
	}
	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	if (App->pathfinding->IsWalkable(p) == true)
	{
		App->pathfinding->MakeNoWalkable(p);
	}
	if (side == NEUTRAL) {
		App->pathfinding->MakeNoConstruible_neutral(p);
	}
	if (side == ALLY) {
		App->pathfinding->MakeNoConstruible_ally(p);
	}
}

void Building::Update()
{
	if (totallybuilded == true) 
	{
		AI();
	}
	Draw();
	if (GetHp() < 0) {
		iPoint p = App->map->WorldToMap(GetX(), GetY());
		if (GetSide() == NEUTRAL) {
			App->pathfinding->MakeConstruible_neutral(p);
		}
		if (GetSide() == ALLY) {
			App->pathfinding->MakeConstruible_ally(p);
		}
		App->pathfinding->MakeWalkable(p);
		this->Die();
	}
}

void Building::AI()
{
	if (Target == nullptr) 
	{
		std::vector<Entity*> EntityVector = App->entity_manager->GetEntityVector();
		std::vector<Entity*>::iterator item = EntityVector.begin();
		for (; item != EntityVector.end(); item++) 
		{
			if ((*item)->GetEntityType() == E_UNIT)
			{
				//TODO:Use a function from entity instead
				if ((*item)->GetX() >= (GetX() - GetRange()) && (*item)->GetX() < (GetX() + GetRange()) && (*item)->GetY() >= (GetY() - GetRange()) && (*item)->GetY() < (GetY() + GetRange()) && (*item)->GetHp() > 0)
				{
					Target = *item;
					AttackTimer.Start();
				}
			}
		}
	}
	else {
		if (Target->GetHp() <= 0)
		{
			Target = nullptr;
		}
		if (AttackTimer.ReadMs() > 900)
		{
			if (Target != nullptr && Target->GetSide() != S_ALLY && Target->GetHp() > 0)
			{
				if (Target->GetX() >= (GetX() - GetRange()) && Target->GetX() < (GetX() + GetRange()) && Target->GetY() >= (GetY() - GetRange()) && Target->GetY() < (GetY() + GetRange()))
				{
					Attack(Target);
					AttackTimer.Start();
				}
				else
				{
					Target = nullptr;
				}
			}
		}
	}
	//std::list<Entity>::iterator ptarget = App->entity_manager->
}

void Building::Draw()
{
	if (totallybuilded != true) {
		if (buildtimer.ReadMs() <= 3000)
		{
			SDL_Rect rect = { 394,1,96,64 };
			SetRect(rect);
			SetPivot(0.53125 * 96, 0.59375 * 64);
		}
		else if (buildtimer.ReadMs() > 3000 && buildtimer.ReadMs() <= 6000)
		{
			SDL_Rect rect = { 376,539,100,73 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 73);
		}
		else if (buildtimer.ReadMs() > 6000 && buildtimer.ReadMs() <= 9000)
		{
			SDL_Rect rect = { 478,539,100,73 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 73);
		}
		else
		{
			SDL_Rect rect = { 610,1,107,206 };
			SetRect(rect);
			SetPivot(0.504673 * 107, 0.902913 * 206);
			totallybuilded = true;
		}
	}
	App->render->PushEntity(this);
}

const BUILDING_TYPE Building::GetBuildingType() const
{
	return building_type;
}

const int Building::GetRange() const
{
	return range;
}

const double Building::GetBuildTime() const
{
	return buildtimer.ReadMs();
}
