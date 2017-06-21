#ifndef __UICHECKBUTTION_H__
#define __UICHECKBUTTION_H__

#include "UIComponents.h"

enum CHECKBUTTON_STAT
{
	CB_CHECK,
	CB_UNCHECK
};

class UICheckbutton : public UIComponents
{
private:
	SDL_Rect rect_atlas_clicked;
	CHECKBUTTON_STAT cb_stat = CB_UNCHECK;

public:
	UICheckbutton(UICOMPONENT_TYPE type);

	void Set(const SDL_Rect& position, const SDL_Rect& atlas, const SDL_Rect & atlas_clicked);

	void Draw();
	void HandleInput(SDL_Event);
	bool Update();

	void SetStat(CHECKBUTTON_STAT cb_stat);
	const CHECKBUTTON_STAT GetStat() const;
};

#endif // __UICHECKBUTTION_H__