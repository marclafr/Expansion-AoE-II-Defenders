#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "Camera.h"
#include "UI_Label.h"

#define TEXTS_COLOR {0, 0, 0, 255} //R G B ALPHA

UI_Label::UI_Label(iPoint pos, SDL_Rect atlas_rect, char * txt, bool has_background, bool not_in_world, UI_ELEMENT_TYPE ui_element_type) : UI_Element(ui_element_type, pos, atlas_rect, not_in_world), has_background(has_background)
{
	text = new Text(txt);
	text->text_rect.x = atlas_rect.x;
	text->text_rect.y = atlas_rect.y;

	//Set Text
	text->text_texture = App->font->Print(text->text, TEXTS_COLOR);
	App->font->CalcSize(text->text, text->text_rect.w, text->text_rect.h);
	text->text_rect.w += (2 * TEXT_RIGHT_DISPLACEMENT);
}

UI_Label::~UI_Label()
{
}

bool UI_Label::Draw(SDL_Texture* atlas)
{
	if (not_in_world == true)
	{
		if (has_background == true)
			App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &text->text_rect);
		App->render->PushUISprite(text->text_texture, pos.x + TEXT_RIGHT_DISPLACEMENT - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y);
	}
	else
	{
		if (has_background == true)
			App->render->PushUISprite(atlas, pos.x, pos.y, &text->text_rect);
		App->render->PushUISprite(text->text_texture, pos.x + TEXT_RIGHT_DISPLACEMENT, pos.y);
	}
	return true;
}

bool UI_Label::HasBackground()
{
	return has_background;
}

int UI_Label::GetTextHeight()
{
	return text->text_rect.h;
}

Text::Text(char* txt) : text(txt)
{
}

Text::~Text()
{
}
