#ifndef __UI_APPEARING_LABEL_H__
#define __UI_APPEARING_LABEL_H__

#include "j1Timer.h"
#include "UI_Label.h"

class UI_AppearingLabel : public UI_Label
{
public:
	UI_AppearingLabel(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, char* txt = nullptr, bool has_background = true, bool not_in_world = true);
	~UI_AppearingLabel();

	bool Draw(SDL_Texture* atlas);

private:
	j1Timer time_on_screen;
	float seconds_on_screen;
};

#endif //__UI_APPEARING_LABEL_H__