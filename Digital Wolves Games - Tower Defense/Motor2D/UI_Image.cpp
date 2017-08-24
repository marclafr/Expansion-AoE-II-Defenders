#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "Camera.h"
#include "UI_Image.h"

UI_Image::UI_Image(iPoint pos, SDL_Rect atlas_rect, bool not_in_world, UI_ELEMENT_TYPE ui_element_type) : UI_Element(ui_element_type, pos, atlas_rect, not_in_world)
{
}

UI_Image::~UI_Image()
{
}

bool UI_Image::IsMouseInside()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	if (not_in_world == false)
	{
		mouse_pos.x -= App->render->camera->GetPosition().x;
		mouse_pos.y -= App->render->camera->GetPosition().y;
	}
	SDL_Rect image_rect = GetImageRect();
	if (image_rect.x <= mouse_pos.x && mouse_pos.x <= image_rect.x + image_rect.w&&
		image_rect.y <= mouse_pos.y && mouse_pos.y <= image_rect.y + image_rect.h)
		return true;

	return false;
}

SDL_Rect UI_Image::GetImageRect()
{
	return{ pos.x,pos.y,atlas_rect.w,atlas_rect.h };
}

bool UI_Image::Draw(SDL_Texture* atlas, int alpha)
{
	if (showing)
	{
		if (not_in_world == true)
			App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
		else
			App->render->PushUISprite(atlas, pos.x, pos.y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
	}
	return true;
}
