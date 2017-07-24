#include "j1App.h"
#include "j1Gui.h"
#include "UI_AppearingLabel.h"

UI_AppearingLabel::UI_AppearingLabel(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, char * txt, bool has_background, bool not_in_world) : UI_Label(UI_E_APPEARING_LABEL, pos, atlas_rect, txt, has_background, not_in_world), seconds_on_screen(seconds_on_screen)
{
	time_on_screen.Start();
}

UI_AppearingLabel::~UI_AppearingLabel()
{
}

bool UI_AppearingLabel::Draw(SDL_Texture * atlas)
{
	//TODO: Change alpha	float appearing_time = seconds_on_screen / 3.0f;

	if (time_on_screen.ReadSec() <= seconds_on_screen)
		UI_Label::Draw(atlas);
	else
		return false;

	return true;
}