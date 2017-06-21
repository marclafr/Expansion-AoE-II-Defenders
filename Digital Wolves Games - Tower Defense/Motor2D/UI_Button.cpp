#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "UI_Button.h"

UI_Button::UI_Button(iPoint pos, SDL_Rect rect_idle, SDL_Rect rect_mouse_on_top, SDL_Rect rect_clicking) : UI_Element(pos, rect_idle), text(nullptr)
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

}

UI_Button::~UI_Button()
{
	if (img_idle != nullptr)
	{
		delete img_idle;
		img_idle = nullptr;
	}

	if (img_mouse_on_top != nullptr)
	{
		delete img_mouse_on_top;
		img_mouse_on_top = nullptr;
	}

	if (img_clicking != nullptr)
	{
		delete img_clicking;
		img_clicking = nullptr;
	}

	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}

bool UI_Button::Draw(SDL_Texture * atlas)
{
	switch (button_state)
	{
	case UI_B_IDLE:
		App->render->PushUISprite(atlas, img_idle->pos.x, img_idle->pos.y, &img_idle->atlas_rect);
		break;

	case UI_B_MOUSE_ON_TOP:
		App->render->PushUISprite(atlas, img_mouse_on_top->pos.x, img_mouse_on_top->pos.y, &img_mouse_on_top->atlas_rect);
		break;

	case UI_B_CLICKING:
		App->render->PushUISprite(atlas, img_clicking->pos.x, img_clicking->pos.y, &img_clicking->atlas_rect);
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
