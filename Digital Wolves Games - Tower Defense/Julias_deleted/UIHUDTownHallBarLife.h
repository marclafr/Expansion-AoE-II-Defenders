#pragma once

#include "UIComponents.h"

#include "UILabel.h"

class Building;

class UIHUDTownHallBarLife : public UIComponents
{
private:
	UIComponents* green_bar = nullptr;
	UILabel* life = nullptr;

	Building* townhall = nullptr;

	SDL_Rect rest_green_bar;
	std::string hp_townhall;

public:
	UIHUDTownHallBarLife(UICOMPONENT_TYPE type);

	void SetTownHall(Building* th);
	
	bool Update();

	void Draw();
};