#ifndef __UI_APPEARING_ELEMENT_H__
#define __UI_APPEARING_ELEMENT_H__

#include "j1Timer.h"
#include "UI_Label.h"
#include "UI_Image.h"

class UI_AppearingLabel : public UI_Label
{
public:
	UI_AppearingLabel(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, char* txt, bool has_background = true, bool not_in_world = true);
	~UI_AppearingLabel();

	bool Draw(SDL_Texture* atlas, int alpha = 255);

private:
	j1Timer time_on_screen;
	float seconds_on_screen;
	Uint8 alpha_appearing_label = 100;
};

class UI_AppearingImage : public UI_Image
{
public:
	UI_AppearingImage(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, bool not_in_world = true);
	~UI_AppearingImage();

	bool Draw(SDL_Texture* atlas, int alpha = 255);

private:
	j1Timer time_on_screen;
	float seconds_on_screen;
	Uint8 alpha_appearing_label = 100;
};

#endif //__UI_APPEARING_ELEMENT_H__