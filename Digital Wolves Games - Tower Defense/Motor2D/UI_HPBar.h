#ifndef __UI_HP_BAR_H__
#define __UI_HP_BAR_H__

#include "SDL\include\SDL.h"
#include "j1Gui.h"

class UI_HPBar : public UI_Element
{
private:
	SDL_Texture* tex;
	int hp;
};

#endif //__UI_HP_BAR_H__