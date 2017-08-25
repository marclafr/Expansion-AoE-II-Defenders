#include "j1App.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Render.h"
#include "Camera.h"
//#include "j1UIManager.h"
//#include "UIHUDMinimap.h"
#include "Entity.h"


Entity::Entity(ENTITY_TYPE entity_type, Side side): to_delete (false), entity_type(entity_type), side(side)
{}

Entity::~Entity()
{}

//Usefull
void Entity::Kill()
{
	current_hp = 0;
}

void Entity::Die()
{
	to_delete = true;
}

bool Entity::ToDelete() const
{
	return to_delete;
}

void Entity::IncreaseArmor(int extra_defense)
{
	armor += extra_defense;
}

void Entity::Attack(Entity* entity)
{
	entity->SetHp(entity->current_hp - this->attack);
}

void Entity::Damaged(int dmg)
{
	if (armor >= dmg)
		current_hp--;
	else
		current_hp -= (dmg - armor);
}

void Entity::UpgradeUnit(int plushealth) {

	this->SetHp(GetHp() + plushealth);

}

//Getters
const iPoint Entity::GetPixelPosition() const
{
	return App->map->MapToWorld(GetPosition());
}

bool Entity::Inside(SDL_Rect rect) const
{
	iPoint pixel_pos = GetPixelPosition();

	//top-left to down right
	if (rect.x < pixel_pos.x
		&& rect.x + rect.w > pixel_pos.x
		&& rect.y < pixel_pos.y
		&& rect.y + rect.h > pixel_pos.y)
		return true;
	
	//down-right to top-left
	if (rect.x > pixel_pos.x
		&& rect.x + rect.w < pixel_pos.x
		&& rect.y > pixel_pos.y
		&& rect.y + rect.h < pixel_pos.y)
		return true;

	//down-left to top-right
	if (rect.x < pixel_pos.x
		&& rect.x + rect.w > pixel_pos.x
		&& rect.y > pixel_pos.y
		&& rect.y + rect.h < pixel_pos.y)
		return true;

	//top-right to down-left
	if (rect.x > pixel_pos.x
		&& rect.x + rect.w < pixel_pos.x
		&& rect.y < pixel_pos.y
		&& rect.y + rect.h > pixel_pos.y)
		return true;

	return false;
}

ENTITY_TYPE Entity::GetEntityType() const
{
	return entity_type;
}

ENTITY_STATUS Entity::GetEntityStatus() {

	return entity_status;
}

const int Entity::GetHp() const
{
	return current_hp;
}

const int Entity::GetArmor() const
{
	return armor;
}

const int Entity::GetAttack() const
{
	return attack;
}

const iPoint Entity::GetPivot() const
{
	return pivot;
}

const SDL_Rect Entity::GetRect() const
{
	return rect;
}

const TextureID Entity::GetTextureID() const
{
	return texture_id;
}

const Side Entity::GetSide() const
{
	return side;
}

const float Entity::GetAIDT() const
{
	return ai_dt;
}

const SDL_Rect Entity::GetInWorldTextureRect() const
{
	SDL_Rect ret;
	iPoint pixel_pos = App->map->MapToWorld(GetPosition());
	SDL_Rect rect = GetRect();
	iPoint pivot = GetPivot();

	ret.x = pixel_pos.x;
	ret.x -= pivot.x;

	ret.y = pixel_pos.y;
	ret.y += App->map->data.tile_height / 2.0f;
	ret.y -= pivot.y;

	ret.h = rect.h;
	ret.w = rect.w;

	return rect;
}

//Setters
void Entity::SetEntityStatus(ENTITY_STATUS status)
{
	entity_status = status;
}

void Entity::SetArmor(int new_armor)
{
	armor = new_armor;
}

void Entity::SetHp(int new_hp)
{
	current_hp = new_hp;
}

void Entity::SetMaxHp(const int new_max_hp)
{
	max_hp = new_max_hp;
}

void Entity::SetLife(const int hp)
{
	SetMaxHp(hp);
	SetHp(hp);
}

void Entity::SetPivot(int x, int y)
{
	pivot.x = x;
	pivot.y = y;
}

void Entity::SetRect(SDL_Rect rect)
{
	this->rect = rect;
}

void Entity::SetTextureID(TextureID id)
{
	texture_id = id;
}

void Entity::SetSide(Side side)
{
	this->side = side;
}

void Entity::SetToDelete()
{
	to_delete = true;
}

void Entity::SetAttack(int new_attack)
{
	attack = new_attack;
}

//Arrow, SHOULD UPDATE ITSELVE
void Entity::ResetArrowPos()
{
	Arrow_pos = 0;
}

float Entity::GetArrowPos() const
{
	return Arrow_pos;
}

void Entity::UpdateArrow(int start_height, iPoint target_pos, int curve_height, float time_secs)
{
	SDL_Rect rect = { 0,0,45,8 };
	iPoint pos;

	iPoint initial_point = { GetPosition().x,GetPosition().y - start_height };
	iPoint last_point = target_pos;
	iPoint mid_point = { (initial_point.x + last_point.x) / 2,((initial_point.y + last_point.y) / 2) - curve_height };

	pos.x = ((1 - Arrow_pos)*(1 - Arrow_pos)*initial_point.x) + ((2 * Arrow_pos)*(1 - Arrow_pos)*mid_point.x) + ((Arrow_pos*Arrow_pos)*last_point.x);
	pos.y = ((1 - Arrow_pos)*(1 - Arrow_pos)*initial_point.y) + ((2 * Arrow_pos)*(1 - Arrow_pos)*mid_point.y) + ((Arrow_pos*Arrow_pos)*last_point.y);

	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), pos.x, pos.y, &rect, SDL_FLIP_NONE, 0, 0, 1, 0, false);
	float diferential = 1 / time_secs;

	Arrow_pos += diferential;
	if (Arrow_pos > 1) Arrow_pos = 1;
}

//Extras
/*
void Entity::DrawPointMinimap()
{
	iPoint unit_minimap_pos = App->uimanager->GetMinimap()->WorldToMinimap(position);
	unit_minimap_pos.x += App->uimanager->GetMinimap()->GetPosRect().w / 2;
	SDL_Rect atlas_point;
	if (GetSide() == S_ALLY)
		atlas_point = GREEN_POINT_ATLAS;
	else if (GetSide() == S_ENEMY)
		atlas_point = RED_POINT_ATLAS;
	//Draw enemy units points
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), unit_minimap_pos.x - App->render->camera->GetPosition().x, unit_minimap_pos.y - App->render->camera->GetPosition().y, &atlas_point);
}*/