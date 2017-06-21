#pragma once

#include "UIComponents.h"

#include "UILabel.h"

class UIHUDScoreBar : public UIComponents
{
private:
	UIComponents* score_bar = nullptr;

	SDL_Rect rest_score_bar;

public:
	UIHUDScoreBar(UICOMPONENT_TYPE type);

	bool Update();

	void Draw();
};