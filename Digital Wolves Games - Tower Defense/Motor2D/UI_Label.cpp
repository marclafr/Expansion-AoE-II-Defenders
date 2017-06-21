#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "UI_Label.h"

#define TEXTS_COLOR {0, 0, 0, 255} //R G B ALPHA
#define TEXT_RIGHT_DISPLACEMENT 5 //pixels

UI_Label::UI_Label(iPoint pos, SDL_Rect atlas_rect, char * txt) : UI_Element(pos, atlas_rect)
{
	text = new UI_Text(txt);
	text->text_rect.x = atlas_rect.x;
	text->text_rect.y = atlas_rect.y;
}

UI_Label::~UI_Label()
{
}

bool UI_Label::Draw(SDL_Texture* atlas)
{
	App->render->PushUISprite(atlas, pos.x, pos.y, &text->text_rect);
	App->render->PushUISprite(text->text_texture, pos.x + TEXT_RIGHT_DISPLACEMENT, pos.y);
	return true;
}

bool UI_Label::Update()
{
	if (text_changed == true)
	{
		text->text_texture = App->font->Print(text->text, TEXTS_COLOR);
		App->font->CalcSize(text->text, text->text_rect.w, text->text_rect.h);
		text->text_rect.w += (2 * TEXT_RIGHT_DISPLACEMENT);
	}
	
	return true;
}

UI_Text::UI_Text(char* txt) : text(txt)
{
}

UI_Text::~UI_Text()
{
}
