#include <math.h>
#include "Projectile.h"
#include "j1Map.h"
#include "j1Animation.h"
#include "IsoPrimitives.h"
#include "Camera.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "ProjectileManager.h"
#include "j1EntityManager.h"

Projectile::Projectile(iPoint initialpos, Entity * target, int damage, float time_in_secs, int start_height, int curve_height, PROJECTILE_TYPE type) : start_pos(initialpos), damage(damage), target(target), start_height(start_height), curve_height(curve_height), projectile_type(type)
{
	last_pos = target->GetPixelPosition();
	diferential = 1 / time_in_secs;
	pre_actual_pos = initialpos;
	switch (type)
	{
	case P_BASIC_ARROW:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_SIMPLE_ARROW));
		break;
	case P_FIRE_ARROW:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_ARROW));
		break;
	case P_ICE_ARROW:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_ARROW));
		break;
	case P_AIR_ARROW:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_AIR_ARROW));
		break;
	case P_CANNONBALL:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_SIMPLE_BOMB));
		App->audio->PlayFx(App->projectile_manager->fx_cannon_shoot);
		break;
	case P_FIRE_CANNONBALL:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_BOMB));
		App->audio->PlayFx(App->projectile_manager->fx_cannon_shoot);
		floor_effect = true;
		break; 
	case P_ICE_CANNONBALL:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_BOMB));
		App->audio->PlayFx(App->projectile_manager->fx_cannon_shoot);
		floor_effect = true;
		break;
	case P_AIR_CANNONBALL:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_AIR_BOMB));
		App->audio->PlayFx(App->projectile_manager->fx_cannon_shoot);
		break;
	default:
		projectile_anim = nullptr;
		break;
	}
}

Projectile::~Projectile()
{
	if (projectile_anim != nullptr)
		delete projectile_anim;
}

void Projectile::Update()
{
	if (target != nullptr)
		last_pos = target->GetPosition();

	iPoint initial_point (start_pos.x, start_pos.y - start_height);

	fPoint mid_point((initial_point.x + last_pos.x) / 2.0f, ((initial_point.y + last_pos.y) / 2.0f) - curve_height);

	actual_pos.x = ((1 - projectile_pos)*(1 - projectile_pos)*initial_point.x) + ((2 * projectile_pos)*(1 - projectile_pos)*mid_point.x) + ((projectile_pos*projectile_pos)*last_pos.x);
	actual_pos.y = ((1 - projectile_pos)*(1 - projectile_pos)*initial_point.y) + ((2 * projectile_pos)*(1 - projectile_pos)*mid_point.y) + ((projectile_pos*projectile_pos)*last_pos.y);

	fPoint first_point (initial_point.x * (1 - projectile_pos) + mid_point.x * projectile_pos, initial_point.y * (1 - projectile_pos) + mid_point.y * projectile_pos);
	fPoint second_point (mid_point.x * (1 - projectile_pos) + last_pos.x * projectile_pos, mid_point.y * (1 - projectile_pos) + last_pos.y * projectile_pos);

	fPoint vector (second_point.x - first_point.x, second_point.y - first_point.y);
	angle = atan2(vector.y, vector.x) * 57.9257795;
	projectile_pos += diferential;

	if (projectile_pos > 1)
		projectile_pos = 1;

	if (target != nullptr && projectile_pos == 1 && dest_reached == false)
	{
		iPoint center = App->map->MapToWorld(target->GetPosition());
		center.y += App->map->data.tile_height / 2.0f;

		switch (projectile_type)
		{
		case P_BASIC_ARROW:
		case P_FIRE_ARROW:
		case P_ICE_ARROW:
		case P_AIR_ARROW:
			target->Damaged(damage);
			dest_reached = true;
			break;
		case P_CANNONBALL:
		case P_FIRE_CANNONBALL:
		case P_AIR_CANNONBALL:
			target->Damaged(damage);
			AreaDamage(damage, { center.x, center.y }, AREA_DMG_RADIUS);
			element_terrain_pos = target->GetPosition();
			delete projectile_anim;
			projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_EXPLOSION));
			dest_reached = true;
			print_element_terrain_timer.Start();
			break;
		case P_ICE_CANNONBALL:
			target->Damaged(damage);
			AreaDamage(damage, { center.x, center.y }, AREA_DMG_RADIUS);
			element_terrain_pos = target->GetPosition();
			delete projectile_anim;
			projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_EXPLOSION));
			dest_reached = true;
			print_element_terrain_timer.Start();
			break;
		default:
			break;
		}
		if (projectile_type == P_ICE_ARROW)
		{
			Unit* unit_target = (Unit*)target;
			unit_target->SlowUnit();
		}
	}

	Draw();
}

void Projectile::Draw()
{
	float pett = print_element_terrain_timer.ReadMs() / 1000.0f;
	if (pett < ELEMENT_TERRAIN_TIME && dest_reached == true)
		PrintElementTerrain(projectile_type, element_terrain_pos);
	

	//alfa = Math.atan2(by - ay, bx - ax);
	if (projectile_anim != nullptr)
		projectile_anim->Update(rect, pivot);
	if (dest_reached == false)
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + actual_pos.x, App->render->camera->GetPosition().y + actual_pos.y))
			App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), actual_pos.x, actual_pos.y, &rect, SDL_FLIP_HORIZONTAL, pivot.x, pivot.y, 1, angle, false);
}

int Projectile::GetProjectilePos() const
{
	return projectile_pos;
}

int Projectile::GetDamage() const
{
	return damage;
}

void Projectile::SetRect(SDL_Rect rect)
{
	this->rect = rect;
}

AnimationManager * Projectile::GetProjectileAnim()
{
	return projectile_anim;
}

void Projectile::AreaDamage(int damage, iPoint center, int radius)
{
	IsoRect rect(fPoint(center.x, center.y), radius, radius);
	std::vector<Entity*> vec;
	App->entity_manager->GetEntitiesInIsoRect(rect, vec);
	for (int i = 0; i < vec.size(); i++)
	{
		Unit* unit = (Unit*)vec[i];
		unit->Damaged(damage / 2);
		if (projectile_type == P_ICE_CANNONBALL)
		{
			unit->SlowUnit();
		}
	}
	vec.clear();
}

void Projectile::PrintElementTerrain(PROJECTILE_TYPE element, iPoint center)
{
	if (floor_effect == true && projectile_anim->Finished() == true)
	{
		switch (projectile_type)
		{
		case P_FIRE_CANNONBALL:
			delete projectile_anim;
			projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_FLOOR));
			break;
		case P_ICE_CANNONBALL:
			delete projectile_anim;
			projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_FLOOR));
			break;
		default:
			break;
		}
		floor_effect = false;
	}
	SDL_Rect rect;
	iPoint pivot;
	if (area_damage_timer.ReadMs() >= FLOOR_DAMAGE_TIME)
	{
		area_damage_timer.Start();
		if (projectile_type == P_FIRE_CANNONBALL)
			AreaDamage(damage / 2, { (int)element_terrain_pos.x, (int)element_terrain_pos.y }, AREA_DMG_RADIUS);
		else if (projectile_type == P_ICE_CANNONBALL)
			AreaDamage(damage / 2, { (int)element_terrain_pos.x, (int)element_terrain_pos.y }, AREA_DMG_RADIUS);
	}
	if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + actual_pos.x, App->render->camera->GetPosition().y + actual_pos.y))
	{
		projectile_anim->Update(rect, pivot);
		switch (element)
		{
		case P_CANNONBALL:
		case P_FIRE_CANNONBALL:
		case P_ICE_CANNONBALL:
		case P_AIR_CANNONBALL:
			App->render->PushInGameSprite(App->tex->GetTexture(T_EXPLOSIONS_AND_FLOOR), center.x, center.y, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);
			break;
		default:
			break;
		}
	}
}