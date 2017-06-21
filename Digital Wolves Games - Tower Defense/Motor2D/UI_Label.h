#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#include "j1Gui.h"

class UI_Image;

class UI_Text
{
public:
	UI_Text(char* txt);
	~UI_Text();

	SDL_Texture* text_texture;
	SDL_Rect text_rect;
	char* text;
};

class UI_Label : public UI_Element
{
public:
	UI_Label(iPoint pos, SDL_Rect atlas_rect, char* txt);
	~UI_Label();

private:
	UI_Text* text;
	bool text_changed = true;

	bool Draw(SDL_Texture* atlas);
	bool Update();
};

#endif //__UI_LABEL_H__