#include "j1App.h"
#include "j1Gui.h"
#include "UI_AppearingElement.h"

#define STARTING_ALPHA 100

UI_AppearingLabel::UI_AppearingLabel(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, char * txt, bool has_background, bool not_in_world) : UI_Label(pos, atlas_rect, txt, has_background, not_in_world, UI_E_APPEARING_ELEMENT), seconds_on_screen(seconds_on_screen), alpha(STARTING_ALPHA)
{
	time_on_screen.Start();
}

UI_AppearingLabel::~UI_AppearingLabel()
{
}

bool UI_AppearingLabel::Draw(SDL_Texture * atlas)
{
	if (showing)
	{
		//TODO function that does this MODIFYALPHA()??
		float appearing_time = seconds_on_screen / 3.0f;
		float disappearing_time = (seconds_on_screen * 2.0f) / 3.0f;

		if (time_on_screen.ReadSec() <= appearing_time)
		{
			if (alpha < 255)
				alpha++;
		}
		else if (time_on_screen.ReadSec() >= disappearing_time)
		{
			if (alpha > 0)
				alpha--;
		}
		else
			alpha = 255;

		SDL_SetTextureAlphaMod(atlas, alpha);

		if (time_on_screen.ReadSec() <= seconds_on_screen)
			UI_Label::Draw(atlas);
		else
			return false;
	}
	return true;
}

UI_AppearingImage::UI_AppearingImage(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, bool not_in_world) : UI_Image(pos, atlas_rect, not_in_world, UI_E_APPEARING_ELEMENT), seconds_on_screen(seconds_on_screen), alpha(STARTING_ALPHA)
{
	time_on_screen.Start();
}

UI_AppearingImage::~UI_AppearingImage()
{
}

bool UI_AppearingImage::Draw(SDL_Texture * atlas)
{
	//TODO function that does this MODIFYALPHA()??
	float appearing_time = seconds_on_screen / 3.0f;
	float disappearing_time = (seconds_on_screen * 2.0f) / 3.0f;

	if (time_on_screen.ReadSec() <= appearing_time)
	{
		if (alpha < 255)
			alpha++;
	}
	else if (time_on_screen.ReadSec() >= disappearing_time)
	{
		if (alpha > 0)
			alpha--;
	}
	else
		alpha = 255;

	SDL_SetTextureAlphaMod(atlas, alpha); 
	
	if (time_on_screen.ReadSec() <= seconds_on_screen)
		UI_Image::Draw(atlas);
	else
		return false;

	return true;
}
