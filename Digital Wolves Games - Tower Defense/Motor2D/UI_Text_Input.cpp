#include "j1App.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Fonts.h"

#include "UI_Text_Input.h"

UI_TextInput::UI_TextInput(iPoint pos, std::string text, FONT_NAME font_name, SDL_Color color, bool not_in_world) : UI_Element(pos, BACKGROUND_RECT_DEFAULT_TEXT, not_in_world), text(text), color(color), text_rect(BACKGROUND_RECT_DEFAULT_TEXT)
{
	for (std::list<Font*>::iterator it = App->font->fonts.begin(); it != App->font->fonts.end(); it++)
	{
		if (it._Ptr->_Myval->name == font_name)
		{
			font = it._Ptr->_Myval->font;
			break;
		}
		else
			font = App->font->default;
	}
}

UI_TextInput::~UI_TextInput()
{
}

bool UI_TextInput::SetFont(FONT_NAME name)
{
	for (std::list<Font*>::iterator it = App->font->fonts.begin(); it != App->font->fonts.end(); it++)
	{
		if (it._Ptr->_Myval->name == name)
		{
			font = it._Ptr->_Myval->font;
			return true;
		}
	}
	return false;
}

bool UI_TextInput::Update()
{
	if (text_changed == true)
	{
		tex = App->font->Print(text.c_str(), color, font);
		App->font->CalcSize(text.c_str(), text_rect.w, text_rect.h, font);
		text_rect.w += (2 * TEXT_RIGHT_DISPLACEMENT);
		text_changed = false;
	}
	return true;
}

bool UI_TextInput::Draw(SDL_Texture * atlas)
{
	if (not_in_world == true)
	{
		App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &text_rect);
		App->render->PushUISprite(tex, pos.x - App->render->camera->GetPosition().x + TEXT_RIGHT_DISPLACEMENT, pos.y - App->render->camera->GetPosition().y);
	}
	else
	{
		App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &text_rect);
		App->render->PushUISprite(tex, pos.x + TEXT_RIGHT_DISPLACEMENT, pos.y);
	}
	return true;
}
