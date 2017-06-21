#pragma once

#include "PanelInfoBasicStructure.h"

#include "UILabel.h"

class OneSelection : public PanelInfo
{
private:
	UIComponents* image = nullptr;
	UILabel* name = nullptr;
	UILabel* life = nullptr;

	UILabel* damage = nullptr;
	UILabel* armor = nullptr;
	UILabel* range = nullptr;

	UILabel* building_percentage = nullptr;

	Entity* e_ptr = nullptr;

	SDL_Rect life_bar_selected;

public:
	OneSelection() : PanelInfo(ONESELECTION) {}

	~OneSelection();

	void Prepare();
	void PrepareUnitInfo();
	void PrepareBuildInfo();
	void PrepareBuildingConstruction();
	void PrepareResourceInfo();

	void Update();
	void UpdateHP();
	void UpdateBuildingPercentageStr();

	void Draw();
	void DrawUnitInfo();
	void DrawBuildInfo();
	void DrawResourceInfo();
};