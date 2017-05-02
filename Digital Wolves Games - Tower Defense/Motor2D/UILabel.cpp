#include "UILabel.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "Camera.h"

#include "UISelectOption.h"

UILabel::UILabel(UICOMPONENT_TYPE type) : UIComponents(type) 
{
	SetInteractive(false);
}

UILabel::~UILabel()
{
	if (font != nullptr)
		delete font;

	App->tex->UnloadLabel(text_img);
}

void UILabel::Set(int pos_x, int pos_y, const char * text, SDL_Color color, _TTF_Font*  font)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;

	text_img = App->font->Print(text, T_AOE_UI_FONT, color);

	this->text.assign(text);

	App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);
}

void UILabel::Draw()
{
	App->render->Blit(text_img, rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
}

void UILabel::HandleInput(SDL_Event event)
{
	UISelectOption* from_option_selected = nullptr;

	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_DOWN)
			//Selecting option from UISelectOption
			if (GetFrom() != nullptr && GetFrom()->GetType() == UICOMPONENT_TYPE::UIT_UISELECTOPTION)
			{
				from_option_selected = (UISelectOption*)GetFrom();

				if (this != from_option_selected->current)
				{
					from_option_selected->ChangeCurrent(this);
					from_option_selected->ChangeDrawAllOptions();
					from_option_selected->selecting = false;
				}
			}
		break;
	}
}

bool UILabel::Update()
{
	if(IsFocus())
		if(App->input->GetMouseButtonDown(MK_LEFT) == KEY_DOWN)
			if (GetFrom() != nullptr && GetFrom()->GetType() == UICOMPONENT_TYPE::UIT_UISELECTOPTION)
			{
				UISelectOption* from_option_selected = (UISelectOption*)GetFrom();

				if (this != from_option_selected->current)
				{
					from_option_selected->ChangeCurrent(this);
					from_option_selected->ChangeDrawAllOptions();
					from_option_selected->selecting = false;
				}
			}

	Draw();
	return true;
}

void UILabel::ChangeText(const char* text, SDL_Color color)
{
	std::string text_to_change(text);
	if (this->text.compare(text_to_change) != 0)
	{
		App->tex->UnloadLabel(text_img);

		text_img = App->font->Print(text_to_change.c_str(), T_AOE_UI_FONT, color);

		App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);

		this->text.erase();
		this->text.assign(text_to_change);
	}
}