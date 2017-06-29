#include "j1App.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Console.h"

#include "UI_Text_Input.h"

#define ATLAS_START_POS_WRITING_RECT { 375, 1245, 2, 0 }
#define REPEATING_KEY_TIME 700 //miliseconds

UI_TextInput::UI_TextInput(iPoint pos, std::string text, FONT_NAME font_name, SDL_Color color, bool not_in_world) : UI_Element(pos, BACKGROUND_RECT_DEFAULT_TEXT, not_in_world), text(text), color(color), text_atlas_rect(BACKGROUND_RECT_DEFAULT_TEXT)
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

	//Create Initial Text
	tex = App->font->Print(text.c_str(), color, font);
	App->font->CalcSize(text.c_str(), text_atlas_rect.w, text_atlas_rect.h, font);
	text_atlas_rect.w += (2 * TEXT_RIGHT_DISPLACEMENT);
	text_changed = false;

	writing_pos = text.length();
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

void UI_TextInput::InputTextChanged()
{
	text_changed = true;
}

const char* UI_TextInput::GetText()
{
	return text.c_str();
}

void UI_TextInput::DeleteText()
{
		text.clear();
		//Clears the image aswell
		tex = App->font->Print(text.c_str(), color, font);
		App->font->CalcSize(text.c_str(), text_atlas_rect.w, text_atlas_rect.h, font);
		text_atlas_rect.w += (2 * TEXT_RIGHT_DISPLACEMENT);
}

bool UI_TextInput::Update()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		if (MouseInsideText() == true && this != App->console->GetConsoleInputText())
			App->gui->SetFocusedText(this);

	if (this == App->gui->GetFocusedText())
	{
		//Just in case:
		if (writing_pos > text.length())
			writing_pos = text.length();

		if (writing_pos < 0)
			writing_pos = 0;
		//-------------

		//Delete a Char
		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN && writing_pos > 0)
		{
			DeleteChar();
			repeat_timer.Start();
		}
		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_REPEAT && writing_pos > 0 && repeat_timer.Read() > REPEATING_KEY_TIME)
			DeleteChar();

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_UP)
			repeat_timer.Start();

		//Go LEFT in the text
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			if (writing_pos > 0)
			{
				writing_pos--;
				repeat_timer.Start();
			}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && repeat_timer.Read() > REPEATING_KEY_TIME)
			if (writing_pos > 0)
				writing_pos--;

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
			repeat_timer.Start();

		//Go RIGHT in the text
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
			if (writing_pos < text.size())
			{
				writing_pos++;
				repeat_timer.Start();
			}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && repeat_timer.Read() > REPEATING_KEY_TIME)
			if (writing_pos < text.size())
				writing_pos++;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
			repeat_timer.Start();

		//Go to Start Text
		if (App->input->GetKey(SDL_SCANCODE_HOME) == KEY_DOWN)
			writing_pos = 0;

		//Go to End Text
		if (App->input->GetKey(SDL_SCANCODE_END) == KEY_DOWN)
			writing_pos = text.length();
	}
	
	if (text_changed == true)
	{
		if (App->input->char_input != nullptr)
		{
			text.insert(writing_pos, App->input->char_input);
			writing_pos++;
		}
		
		tex = App->font->Print(text.c_str(), color, font);
		App->font->CalcSize(text.c_str(), text_atlas_rect.w, text_atlas_rect.h, font);
		text_atlas_rect.w += (2 * TEXT_RIGHT_DISPLACEMENT);
		text_changed = false;
	}

	return true;
}

bool UI_TextInput::Draw(SDL_Texture * atlas)
{
	if (this == App->console->GetConsoleInputText())
	{
		if (App->console->IsOn() == true)
			DrawTextInput(atlas);
	}
	else
		DrawTextInput(atlas);
	return true;
}

void UI_TextInput::DrawTextInput(SDL_Texture* atlas)
{
	if (not_in_world == true)
	{
		App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &text_atlas_rect);
		App->render->PushUISprite(tex, pos.x - App->render->camera->GetPosition().x + TEXT_RIGHT_DISPLACEMENT, pos.y - App->render->camera->GetPosition().y);
		if (this == App->gui->GetFocusedText() && writing_pos_appear_timer.Read() > 500)	//every half second it appear && disappear
		{
			SDL_Rect rect = ATLAS_START_POS_WRITING_RECT;
			rect.h = text_atlas_rect.h;
			int w, h;
			std::string temp = text.substr(0, writing_pos);
			App->font->CalcSize(temp.c_str(), w, h, font);
			App->render->PushUISprite(atlas, pos.x + w + TEXT_RIGHT_DISPLACEMENT - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &rect);
			if (writing_pos_appear_timer.Read() > 1000) //every half second it appear && disappear
				writing_pos_appear_timer.Start();
		}
	}

	else
	{
		App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &text_atlas_rect);
		App->render->PushUISprite(tex, pos.x + TEXT_RIGHT_DISPLACEMENT, pos.y);
		if (this == App->gui->GetFocusedText() && writing_pos_appear_timer.Read() > 500)	//every half second it appear && disappear
		{
			SDL_Rect rect = ATLAS_START_POS_WRITING_RECT;
			rect.h = text_atlas_rect.h;
			int w, h;
			std::string temp = text.substr(0, writing_pos);
			App->font->CalcSize(temp.c_str(), w, h, font);
			App->render->PushUISprite(atlas, pos.x + w + TEXT_RIGHT_DISPLACEMENT, pos.y, &rect);
			if (writing_pos_appear_timer.Read() > 1000)	//every half second it appear && disappear
				writing_pos_appear_timer.Start();
		}
	}
}

bool UI_TextInput::MouseInsideText()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	if (not_in_world == false)
	{
		mouse_pos.x -= App->render->camera->GetPosition().x;
		mouse_pos.y -= App->render->camera->GetPosition().y;
	}
	SDL_Rect text_rect;
	text_rect = text_atlas_rect;
	text_rect.x = pos.x;
	text_rect.y = pos.y;

	if (text_rect.x <= mouse_pos.x && mouse_pos.x <= text_rect.x + text_rect.w &&
		text_rect.y <= mouse_pos.y && mouse_pos.y <= text_rect.y + text_rect.h)
		return true;
	
	return false;
}

void UI_TextInput::DeleteChar()
{
	text.erase(writing_pos - 1, 1);
	writing_pos--;
	text_changed = true;
	App->input->char_input = nullptr;
}
