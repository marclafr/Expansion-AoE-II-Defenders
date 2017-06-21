#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"
#include "Task.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UICheckbutton.h"
#include "UIGetEntitiesInfo.h"
#include "UIHUDPanelButtons.h"
#include "UIHUDDescription.h"

#define BACKGROUND_POSITION_NAME {1, 593, 194, 15}
#define BACKGROUND_POSITION_PRICE {1, 608, 194, 15}

#define ATLAS_BACKGROUND {1, 995, 194, 15}

#define X_LABEL_NAME 3
#define Y_LABEL_NAME 593
#define X_LABEL_PRICE 3
#define Y_LABEL_PRICE 608

UIHUDDescription::UIHUDDescription(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);

	unit_desc = U_NO_UNIT;
	build_desc = B_NO_BUILDING;
	tower_desc = T_NO_TYPE;
	tower_upgrade_desc = INV_NONE;
	investigation_desc = INV_NONE;
}

void UIHUDDescription::SetEnableButton(UICheckbutton* btn)
{
	button_enable_component = btn;
}

bool UIHUDDescription::Update()
{
	if (button_enable_component->GetStat() == CB_CHECK)
		if(created)
			if (selected->GetButton()->IsFocus() == false)
				Clear();

	return true;
}

void UIHUDDescription::SetDescription(info_button * if_btn)
{
	const EntityTask* e_task = (EntityTask*)if_btn->GetTask();

	if (button_enable_component->GetStat() == CB_CHECK && created == false)
	{
		TrainUnitTask* u_task = nullptr;
		UpgradeTowerTask* ut_task = nullptr;
		UpgradeWallTask* uw_task = nullptr;
		DoInvestigation* di_task = nullptr;
		DeleteWallTask* dw_task = nullptr;
		DeleteTowerTask* dt_task = nullptr;

		switch (e_task->GetEntityTaskType())
		{
		case ET_BASICTOWER:
			tower_desc = T_BASIC_TOWER;
			SetLabelTower();
			break;
		case ET_BOMBARDTOWER:
			tower_desc = T_BOMBARD_TOWER;
			SetLabelTower();
			break;
		case ET_WALL:
			build_desc = B_WOOD_WALL;
			SetLabelBuilding();
			break;
		case ET_UNIT:
			u_task = (TrainUnitTask*)e_task;
			unit_desc = u_task->GetUnitType();
			SetLabelUnit();
			break;
		case ET_UPGRADE_TOWER:
			ut_task = (UpgradeTowerTask*)e_task;
			tower_upgrade_desc = ut_task->GetUpgradeType();
			SetLabelTowerUpgrade();
			break;
		case ET_UPGRADE_WALL:
			uw_task = (UpgradeWallTask*)e_task;
			build_desc = uw_task->GetWallType();
			SetLabelWallUpgrade();
			break;
		case ET_INVESTIGATION:
			di_task = (DoInvestigation*)e_task;
			investigation_desc = di_task->GetInvestigationType();
			SetLabelInvestigations();
			break;
		case ET_DELETEWALL:
			SetLabelDestruction();
			break;
		case ET_DELETETOWER:
			SetLabelDestruction();
			break;
		case ET_MOREBUTTONS:
			SDL_Rect rect_pos = if_btn->GetButton()->GetPosRect();
			SetLabelMoreButtons(rect_pos);
			break;
		case ET_BACKBUTTONS:
			SetLabelBackButtons();
			break;
		}
		created = true;
		selected = if_btn;
	}
}

void UIHUDDescription::SetLabelUnit()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);

	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);

	description_name = App->uimanager->AddLabel(X_LABEL_NAME, Y_LABEL_NAME, GetUnitName(unit_desc));

	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, GetUnitPrice(unit_desc));
}

void UIHUDDescription::SetLabelBuilding()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);

	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);

	description_name = App->uimanager->AddLabel(X_LABEL_NAME, Y_LABEL_NAME, GetBuildingName(build_desc));

	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, GetBuildingPrice(build_desc));
}

void UIHUDDescription::SetLabelTower()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);

	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);

	description_name = App->uimanager->AddLabel(X_LABEL_NAME, Y_LABEL_NAME, GetTowerName(tower_desc));

	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, GetTowerPrice(tower_desc));
}

void UIHUDDescription::SetLabelTowerUpgrade()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);

	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);

	description_name = App->uimanager->AddLabel(X_LABEL_NAME, Y_LABEL_NAME, GetTowerUpgradeName(tower_upgrade_desc));

	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, GetTowerUpgradePrice(tower_upgrade_desc));
}

void UIHUDDescription::SetLabelWallUpgrade()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);

	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);

	description_name = App->uimanager->AddLabel(X_LABEL_NAME, Y_LABEL_NAME, GetWallUpgradeName(build_desc));

	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, GetWallUpgradePrice(build_desc));
}

void UIHUDDescription::SetLabelInvestigations()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_NAME, ATLAS_BACKGROUND);

	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);

	description_name = App->uimanager->AddLabel(X_LABEL_NAME, Y_LABEL_NAME, GetInvestigationName(investigation_desc));

	invest_price = "Price: ";
	GetInvestigationPrice(investigation_desc, invest_price);
	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, invest_price.c_str());
}

//Investigations expansion
void UIHUDDescription::SetLabelMoreButtons(SDL_Rect& rect_pos)
{
	background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);
	description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, GetExpandButtonsDescription(rect_pos));
}

void UIHUDDescription::SetLabelBackButtons()
{
background_price = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);
description_price = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, "Back");
}

void UIHUDDescription::SetLabelDestruction()
{
	background_name = App->uimanager->AddComponent(UIT_UIIMAGE, BACKGROUND_POSITION_PRICE, ATLAS_BACKGROUND);
	description_name = App->uimanager->AddLabel(X_LABEL_PRICE, Y_LABEL_PRICE, "Destroy this");
}

void UIHUDDescription::Clear()
{
	unit_desc = U_NO_UNIT;
	build_desc = B_NO_BUILDING;
	tower_desc = T_NO_TYPE;
	tower_upgrade_desc = INV_NONE;
	investigation_desc = INV_NONE;
	if(background_name != nullptr)
		background_name->SetToDelete();
	if (description_name != nullptr)
		description_name->SetToDelete();
	if (background_price != nullptr)
		background_price->SetToDelete();
	if (description_price != nullptr)
		description_price->SetToDelete();
	background_name = nullptr;
	description_name = nullptr;
	background_price = nullptr;
	description_price = nullptr;
	selected = nullptr;
	created = false;
}