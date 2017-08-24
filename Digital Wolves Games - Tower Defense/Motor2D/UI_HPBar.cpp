#include "j1App.h"
#include "j1Render.h"
#include "Camera.h"
#include "Entity.h"
#include "UI_HPBar.h"

UI_HPBar::UI_HPBar(iPoint pos, SDL_Rect atlas_back_rect, SDL_Rect damaged_atlas_rect, Entity* entity, bool not_in_world) : UI_Element(UI_E_HP_BAR, pos, atlas_back_rect, not_in_world), damaged_atlas_rect(damaged_atlas_rect), entity(entity), max_hp(entity->GetHp())
{	
}

UI_HPBar::~UI_HPBar()
{
}

bool UI_HPBar::Draw(SDL_Texture * atlas, int alpha)
{
	if (showing)
	{
		//Calculate the hp/max_hp relation with the bar
		int green_pixels_num = ceil(entity->GetHp() * App->gui->data.green_hp_bar_rect.w / max_hp);
		atlas_rect.w = green_pixels_num;

		if (not_in_world == true)
		{
			App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &damaged_atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
			App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
		}
		else
		{
			App->render->PushUISprite(atlas, pos.x, pos.y, &damaged_atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
			App->render->PushUISprite(atlas, pos.x, pos.y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
		}
	}
	return true;
}
