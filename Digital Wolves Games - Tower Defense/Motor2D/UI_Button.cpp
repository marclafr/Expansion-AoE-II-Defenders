#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "UI_Button.h"

UI_Button::UI_Button(iPoint pos, SDL_Rect rect_idle, SDL_Rect rect_mouse_on_top, SDL_Rect rect_clicking, char* descript, SDL_Rect description_background_rect) : UI_Element(pos, rect_idle)
{
	img_idle = new UI_Image(pos, rect_idle);

	int extra_w_mot = rect_mouse_on_top.w - rect_idle.w;
	int extra_h_mot = rect_mouse_on_top.h - rect_idle.h;

	if (extra_w_mot != 0 || extra_h_mot != 0)
	{
		iPoint new_pos = { pos.x - extra_w_mot / 2 , pos.y - extra_h_mot / 2 };
		img_mouse_on_top = new UI_Image(new_pos, rect_mouse_on_top);
	}
	else
		img_mouse_on_top = new UI_Image(pos, rect_mouse_on_top);

	int extra_w_click = rect_clicking.w - rect_idle.w;
	int extra_h_click = rect_clicking.h - rect_idle.h;
	
	if (extra_w_click != 0 || extra_h_click != 0)
	{
		iPoint new_pos = { pos.x - extra_w_click / 2 , pos.y - extra_h_click / 2 };
		img_clicking = new UI_Image(new_pos, rect_clicking);
	}
	else
		img_clicking = new UI_Image(pos, rect_clicking);

	if (descript != nullptr)
	{
		description = App->gui->CreateLabel({ pos.x, pos.y + 30 }, description_background_rect, descript);//TODO SOLVE THE 30 magic number.
	}
	else
	{
		description = nullptr;
		has_description = false;
	}
}

UI_Button::~UI_Button()
{
	DELETE_PTR(img_idle);
	DELETE_PTR(img_mouse_on_top);
	DELETE_PTR(img_clicking);
	DELETE_PTR(description);
}

bool UI_Button::Draw(SDL_Texture * atlas)
{
	switch (button_state)
	{
	case UI_B_IDLE:
		App->render->PushUISprite(atlas, img_idle->GetX(), img_idle->GetY(), &img_idle->GetAtlasRect());
		break;

	case UI_B_MOUSE_ON_TOP:
		App->render->PushUISprite(atlas, img_mouse_on_top->GetX(), img_mouse_on_top->GetY(), &img_mouse_on_top->GetAtlasRect());
		break;

	case UI_B_CLICKING:
		App->render->PushUISprite(atlas, img_clicking->GetX(), img_clicking->GetY(), &img_clicking->GetAtlasRect());
		break;

	default:
		break;
	}

	return true;
}

bool UI_Button::Update()
{
	if (img_idle->IsMouseInside())
	{
		button_state = UI_B_MOUSE_ON_TOP;
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			button_state = UI_B_CLICKING;
	}
	else
		button_state = UI_B_IDLE;

	return true;
}
