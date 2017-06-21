#ifndef __UIHUDDESCRIPTION_H__
#define __UIHUDDESCRIPTION_H__

#include "UIComponents.h"

enum UNIT_TYPE;
enum BUILDING_TYPE;
enum Side;
struct info_button;

class UIHUDDescription : public UIComponents
{
public:
	UICheckbutton* button_enable_component = nullptr;

	UIComponents* background_name = nullptr;
	UIComponents* background_price = nullptr;
	UILabel* description_name = nullptr;
	UILabel* description_price = nullptr;

	UNIT_TYPE unit_desc;
	BUILDING_TYPE build_desc;
	TOWER_TYPE tower_desc;
	INVESTIGATION_TYPE tower_upgrade_desc;
	INVESTIGATION_TYPE investigation_desc;
	std::string invest_price;

	info_button* selected = nullptr;

	bool created = false;

public:
	UIHUDDescription(UICOMPONENT_TYPE type);

	bool Update();

	void SetEnableButton(UICheckbutton* btn);
	void SetDescription(info_button* if_btn);
	void SetLabelUnit();
	void SetLabelBuilding();
	void SetLabelTower();
	void SetLabelTowerUpgrade();
	void SetLabelWallUpgrade();
	void SetLabelInvestigations();
	void SetLabelMoreButtons(SDL_Rect&);
	void SetLabelBackButtons();
	void SetLabelDestruction();

	void Clear();
};

#endif // __UIHUDDESCRIPTION_H__