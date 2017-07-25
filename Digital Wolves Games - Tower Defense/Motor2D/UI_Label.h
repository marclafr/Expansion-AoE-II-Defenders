#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "j1Gui.h"

class Text
{
public:
	Text(char* txt);
	~Text();

	SDL_Texture* text_texture;
	SDL_Rect text_rect;
	char* text;
};

class UI_Label : public UI_Element
{
public:
	UI_Label(iPoint pos, SDL_Rect atlas_rect, char* txt, bool has_background = true, bool not_in_world = true, UI_ELEMENT_TYPE ui_element_type = UI_E_LABEL);
	~UI_Label();

	bool Draw(SDL_Texture* atlas);

	bool HasBackground();
	int GetTextHeight();

private:
	Text* text;
	bool has_background = true;
};

#endif //__UI_LABEL_H__