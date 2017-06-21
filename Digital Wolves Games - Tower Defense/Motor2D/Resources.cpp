/*
#include "Resources.h"
#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"

Resources::Resources(RESOURCE_TYPE r_type, fPoint pos, int collected, float time) : Entity(E_RESOURCE, pos, S_ALLY), resource_type(r_type)
{
	SDL_Rect rect;
	switch (r_type)
	{
	case R_FOOD:
		SetSide(S_ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 11.0f;
		amount_collected = 50;
		build_time = 7.0f;
		rect = { 0, 0, 222, 217 };
		SetRect(rect);
		SetPivot(115, 165);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		mill_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_MILL_ROTATION));
		break;

	case R_WOOD:
		SetSide(S_ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 11.0f;
		amount_collected = 140;
		build_time = 7.0f;
		rect = { 222, 0, 208, 175 };
		SetRect(rect);
		SetPivot(101, 169);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;

	case R_GOLD:
		SetSide(S_ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 11.0f;
		amount_collected = 60;
		build_time = 7.0f;
		rect = { 0, 217, 212, 130 };
		SetRect(rect);
		SetPivot(104, 85);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;

	case R_STONE:
		SetSide(S_ALLY);
		SetHp(10);
		SetArmor(1);
		collect_time = 11.0f;
		amount_collected = 150;
		build_time = 7.0f;
		rect = { 212, 217, 212, 130 };
		SetRect(rect);
		SetPivot(104, 85);
		SetTextureID(T_RESOURCE);
		totallybuilded = true;
		CollectTimer.Start();
		break;

	default:
		LOG("Error BUILDING TYPE STATS NULL");
		resource_type = R_NO_RESOURCE;
		break;
	}
	amount_collected = collected;
	collect_time = time;
	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	if (App->pathfinding->IsWalkable(p) == true)
	{
		App->pathfinding->MakeNoWalkable(p);
	}
}

Resources::~Resources()
{
}

void Resources::Update(float dt)
{
	if (GetHp() <= 0)
	{
		this->Die();
	}
	if (totallybuilded == true)
	{
		AI();
	}
	Draw();
}

void Resources::AI()
{
	if (CollectTimer.ReadSec() >= collect_time)
	{
		AddResource(amount_collected);

		CollectTimer.Start();
	}
}

void Resources::Draw()
{
	App->render->PushInGameSprite(this);
	if (resource_type == R_FOOD)
	{
		SDL_Rect anim_rect;
		iPoint anim_pivot;
		mill_anim->Update(anim_rect, anim_pivot);
		App->render->PushInGameSprite(App->tex->GetTexture(T_MILL_ANIMATION), GetX() - 14, GetY() - 81 + 100, &anim_rect, SDL_FLIP_NONE, anim_pivot.x, anim_pivot.y + 100); //Magic numbers yay!! :/
	}
}

const RESOURCE_TYPE Resources::GetResourceType() const
{
	return resource_type;
}

const double Resources::GetBuildTime() const
{
	return buildtimer.Read();
}

bool Resources::ReduceCollectTime(float reduction)
{
	collect_time -= reduction;
	if (collect_time < 1.0f)
	{
		collect_time = 1.0f;
		return false;
	}
	return true;
}

void Resources::IncreaseResourceAmount(int amount)
{
	amount_collected += amount;
}

int Resources::GetResource()
{
	return amount_collected;
}

void Resources::Save(pugi::xml_node &data)
{
	pugi::xml_node build = data.child("resources");
	pugi::xml_node actualresource = build.append_child("resource");
	actualresource.append_attribute("resource_type") = GetResourceType();
	actualresource.append_attribute("posx") = GetX();
	actualresource.append_attribute("posy") = GetY();
	actualresource.append_attribute("amount_collected") = amount_collected;
	actualresource.append_attribute("collect_time") = collect_time;
}

void Resources::AddResource(int add)
{
	switch (resource_type)
	{
	case R_WOOD:
		App->scene->resources->AddWood(add);
		break;
	case R_STONE:
		App->scene->resources->AddStone(add);
		break;
	case R_GOLD:
		App->scene->resources->AddGold(add);
		break;
	case R_FOOD:
		App->scene->resources->AddFood(add);
		break;
	}
}*/