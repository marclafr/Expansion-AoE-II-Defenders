#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "UI_Image.h"
#include "UI_Label.h"

enum UI_BUTTON_STATE
{
	UI_B_IDLE,
	UI_B_MOUSE_ON_TOP,
	UI_B_CLICKING,
};

class UI_Button : public UI_Element
{
public:
	UI_Button(iPoint pos, SDL_Rect rect_idle, SDL_Rect rect_mouse_on_top, SDL_Rect rect_clicking, char* description = nullptr, SDL_Rect description_background_rect = { 400,1750,0,0 }, bool not_in_world = true);
	~UI_Button();
	
	bool Draw(SDL_Texture* atlas);
	bool Update();

private:
	UI_Image* img_idle;
	UI_Image* img_mouse_on_top;
	UI_Image* img_clicking;
	UI_Label* description;

	UI_BUTTON_STATE button_state = UI_B_IDLE;
	bool has_description = false;
};

#endif //__UI_BUTTON_H__