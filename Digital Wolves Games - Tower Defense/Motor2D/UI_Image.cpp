#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "Camera.h"
#include "UI_Image.h"

UI_Image::UI_Image(iPoint pos, SDL_Rect atlas_rect) : UI_Element(pos, atlas_rect)
{
}

UI_Image::~UI_Image()
{
}

bool UI_Image::IsMouseInside()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	mouse_pos.x -= App->render->camera->GetPosition().x;
	mouse_pos.y -= App->render->camera->GetPosition().y;
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

bool UI_Image::Draw(SDL_Texture* atlas)
{
	App->render->PushUISprite(atlas, pos.x, pos.y, &atlas_rect);
	return true;
}
