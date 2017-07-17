#ifndef __UI_HP_BAR_H__
#define __UI_HP_BAR_H__

#include "SDL\include\SDL.h"
#include "j1Gui.h"

class UI_HPBar : public UI_Element
{
public:
	UI_HPBar(iPoint pos, SDL_Rect atlas_back_rect, SDL_Rect damaged_atlas_rect, Entity* entity, bool not_in_world = true);
	~UI_HPBar();

	bool Draw(SDL_Texture* atlas);

private:
	SDL_Rect damaged_atlas_rect;
	Entity* entity;
	int max_hp;
};

#endif //__UI_HP_BAR_H__