#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "j1Gui.h"

class UI_Image;

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
	UI_Label(iPoint pos, SDL_Rect atlas_rect, char* txt, bool not_in_world = true);
	~UI_Label();

private:
	Text* text;

	bool Draw(SDL_Texture* atlas);
};

#endif //__UI_LABEL_H__