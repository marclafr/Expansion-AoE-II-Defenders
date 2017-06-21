#include "UIHUDPanelButtons.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1UIManager.h"
#include "j1Scene.h"
#include "SDL\include\SDL_rect.h"
#include "Task.h"
#include "UIButton.h"
#include "UIHUDDescription.h"
#include "j1Tutorial.h"
#include "j1Audio.h"
#include "UIGetEntitiesInfo.h"

#define ICON_ATLASSIZE 25
#define ICON_SIZE 25
#define ICON_SEPARATION 30
#define PANEL_XPOSITION 28 //26 + 2
#define PANEL_YPOSITION 668 //666+2
#define ATLASX_BUTTONGREY_CANNON_DISTANCE 355
#define ATLASY_BUTTONGREY_CANNON_DISTANCE 78
#define ATLAS_BUTTONGREY_TOWER_DISTANCE 251

UIHUDPanelButtons::UIHUDPanelButtons(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);
	panel_seleted_type = BP_NO_SELECTION;
}

UIHUDPanelButtons::~UIHUDPanelButtons()
{
	DeletePanel();

	std::vector<info_button*>::iterator ib_item;
	for (ib_item = panel_no_selection.begin(); ib_item != panel_no_selection.end(); ++ib_item)
		DELETE_PTR(*ib_item);
	
	for (ib_item = panel_townhall.begin(); ib_item != panel_townhall.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_university.begin(); ib_item != panel_university.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_turret.begin(); ib_item != panel_turret.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_cannon.begin(); ib_item != panel_cannon.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_turret_upgraded.begin(); ib_item != panel_turret_upgraded.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_wood_wall.begin(); ib_item != panel_wood_wall.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_stone_wall.begin(); ib_item != panel_stone_wall.end(); ++ib_item)
		DELETE_PTR(*ib_item);

	for (ib_item = panel_brick_wall.begin(); ib_item != panel_brick_wall.end(); ++ib_item)
		DELETE_PTR(*ib_item);
}

void UIHUDPanelButtons::SetPanel(Building* building)
{
	DeletePanel();

	if (building == nullptr)
	{
		panel_seleted = &panel_no_selection;
		panel_seleted_type = BP_NO_SELECTION;
		b_selected = nullptr;
		CreatePanel();
	}
	else {
		switch (building->GetBuildingType())
		{
		case B_TURRET:
			panel_seleted = &panel_turret;
			panel_seleted_type = BP_TURRET;
			b_selected = building;
			CreatePanel();
			if (App->tutorial->tutorial_num == TUTORIAL_5) App->tutorial->TowerSelected = true;
			break;
		case B_CANNON:
			panel_seleted = &panel_cannon;
			panel_seleted_type = BP_CANNON;
			b_selected = building;
			CreatePanel();
			if (App->tutorial->tutorial_num == TUTORIAL_5) App->tutorial->TowerSelected = true;
			break;
			case B_TURRET_UPGRADED_FIRE:
			panel_seleted = &panel_turret_upgraded;
			panel_seleted_type = BP_TURRET_UPGRADED;
			b_selected = building;
			CreatePanel();
			break;
		case B_CANNON_UPGRADED_ICE:
			panel_seleted = &panel_turret_upgraded;
			panel_seleted_type = BP_TURRET_UPGRADED;
			b_selected = building;
			CreatePanel();
			break;
		case B_TURRET_UPGRADED_AIR:
			panel_seleted = &panel_turret_upgraded;
			panel_seleted_type = BP_TURRET_UPGRADED;
			b_selected = building;
			CreatePanel();
			break;
		case B_CANNON_UPGRADED_FIRE:
			panel_seleted = &panel_turret_upgraded;
			panel_seleted_type = BP_TURRET_UPGRADED;
			b_selected = building;
			CreatePanel();
			break;
		case B_TURRET_UPGRADED_ICE:
			panel_seleted = &panel_turret_upgraded;
			panel_seleted_type = BP_TURRET_UPGRADED;
			b_selected = building;
			CreatePanel();
			break;
		case B_CANNON_UPGRADED_AIR:
			panel_seleted = &panel_turret_upgraded;
			panel_seleted_type = BP_TURRET_UPGRADED;
			b_selected = building;
			CreatePanel();
			break;
		case B_WOOD_WALL:
			panel_seleted = &panel_wood_wall;
			panel_seleted_type = BP_WOOD_WALL;
			b_selected = building;
			CreatePanel();
			break;
		case B_STONE_WALL:
			panel_seleted = &panel_stone_wall;
			panel_seleted_type = BP_STONE_WALL;
			b_selected = building;
			CreatePanel();
			break;
		case B_BRICK_WALL:
			panel_seleted = &panel_brick_wall;
			panel_seleted_type = BP_BRICK_WALL;
			b_selected = building;
			CreatePanel();
			break;
		case B_TOWNHALL:
			panel_seleted = &panel_townhall;
			panel_seleted_type = BP_TOWNHALL;
			b_selected = building;
			CreatePanel();
			if (App->tutorial->tutorial_num == TUTORIAL_2) App->tutorial->TownHallSelected = true;
			break;
		case B_UNIVERSITY:
			panel_seleted = &panel_university;
			panel_seleted_type = BP_UNIVERSITY;
			b_selected = building;
			CreatePanel();
			if (App->tutorial->tutorial_num == TUTORIAL_4) App->tutorial->UniversitySelected = true;
			break;
		default:
			panel_seleted = nullptr;
			panel_seleted_type = BP_NONE;
			b_selected = nullptr;
			break;
		}
	}
}

bool UIHUDPanelButtons::Update()
{
	if (panel_seleted != nullptr)
	{
		std::vector<info_button*>* inside_button = isInsideButton(panel_seleted);

		if (inside_button == nullptr) {
			for (std::vector<info_button*>::iterator ib_item = panel_seleted->begin(); ib_item != panel_seleted->end(); ++ib_item)
			{
				(*ib_item)->Update();
			}
		}
		else
		{
			for (std::vector<info_button*>::iterator ib_item = inside_button->begin(); ib_item != inside_button->end(); ++ib_item)
			{
				(*ib_item)->Update();
			}
		}

		if (want_to_reset)
			Reset();
	}
	return true;
}

//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
info_button* UIHUDPanelButtons::AddButton(BUILDING_PANELINFO type, iPoint position, iPoint atlas, Task* task, bool delete_button)
{
	info_button* new_btn = new info_button(position,atlas, task, delete_button);

	switch (type)
	{
	case BP_NO_SELECTION:
		panel_no_selection.push_back(new_btn);
		return new_btn;
		break;
	case BP_TOWNHALL:
		panel_townhall.push_back(new_btn);
		return new_btn;
		break;
	case BP_UNIVERSITY:
		panel_university.push_back(new_btn);
		return new_btn;
		break;
	case BP_TURRET:
		panel_turret.push_back(new_btn);
		return new_btn;
		break;
	case BP_CANNON:
		panel_cannon.push_back(new_btn);
		return new_btn;
		break;
	case BP_TURRET_UPGRADED:
		panel_turret_upgraded.push_back(new_btn);
		return new_btn;
		break;
	case BP_WOOD_WALL:
		panel_wood_wall.push_back(new_btn);
		return new_btn;
		break;
	case BP_STONE_WALL:
		panel_stone_wall.push_back(new_btn);
		return new_btn;
		break;
	case BP_BRICK_WALL:
		panel_brick_wall.push_back(new_btn);
		return new_btn;
		break;
	}

	DELETE_PTR(new_btn);
	return nullptr;
}

void UIHUDPanelButtons::CreatePanel()
{
	if (panel_seleted != nullptr)
	{
		std::vector<info_button*>* inside_button = isInsideButton(panel_seleted);

		if (inside_button == nullptr) {
			for (std::vector<info_button*>::iterator ib_item = panel_seleted->begin(); ib_item != panel_seleted->end(); ++ib_item)
			{
				(*ib_item)->CreateButton(panel_seleted_type, b_selected);
			}
		}
		else
		{
			for (std::vector<info_button*>::iterator ib_item = inside_button->begin(); ib_item != inside_button->end(); ++ib_item)
			{
				(*ib_item)->CreateButton(panel_seleted_type, b_selected);
			}
		}
	}
}

void UIHUDPanelButtons::DeletePanel()
{
	if (panel_seleted != nullptr)
	{
		App->uimanager->ClearDescription();

		std::vector<info_button*>* inside_button = isInsideButton(panel_seleted);

		if (inside_button == nullptr) {
			for (std::vector<info_button*>::iterator ib_item = panel_seleted->begin(); ib_item != panel_seleted->end(); ++ib_item)
			{
				(*ib_item)->ButtonToDelete();
			}
		}
		else
		{
			for (std::vector<info_button*>::iterator ib_item = inside_button->begin(); ib_item != inside_button->end(); ++ib_item)
			{
				(*ib_item)->ButtonToDelete();
			}
			search_infobutton(panel_seleted, inside_button)->SetShowButtons(false);
		}

		panel_seleted_type = BP_NONE;
		b_selected = nullptr;
		panel_seleted = nullptr;
	}
}

void UIHUDPanelButtons::Reset()
{
	std::vector<info_button*>* temp_panel_seleted = panel_seleted;
	BUILDING_PANELINFO temp_panel_seleted_type = panel_seleted_type;
	Building* temp_b_selected = b_selected;
	DeletePanel();
	panel_seleted = temp_panel_seleted;
	panel_seleted_type = temp_panel_seleted_type;
	b_selected = temp_b_selected;
	if(want_enter)
		ib_reset->SetShowButtons(true);
	else
		ib_reset->SetShowButtons(false);
	CreatePanel();
	want_to_reset = false;
	ib_reset = nullptr;
}

void UIHUDPanelButtons::WantReset(info_button* ib_btn, bool enter)
{
	want_to_reset = true;
	want_enter = enter;
	ib_reset = ib_btn;
}

std::vector<info_button*>* UIHUDPanelButtons::isInsideButton(std::vector<info_button*>* ib_vector)
{
	for (std::vector<info_button*>::iterator ib_item = ib_vector->begin(); ib_item != ib_vector->end(); ++ib_item)
	{

		if ((*ib_item)->ShowButtons()) { return (*ib_item)->GetInfoButtons(); }

		std::vector<info_button*>* ib_showing = isInsideButton((*ib_item)->GetInfoButtons());
		if (ib_showing != nullptr) { return ib_showing; }
	}
	return nullptr;
}

info_button* UIHUDPanelButtons::search_infobutton(std::vector<info_button*>* ib_vector, std::vector<info_button*>* search)
{
	for (std::vector<info_button*>::iterator ib_item = ib_vector->begin(); ib_item != ib_vector->end(); ++ib_item)
	{
		if ((*ib_item)->GetInfoButtons() == search) { return (*ib_item);}

		info_button* ib_showing = search_infobutton((*ib_item)->GetInfoButtons(), search);
		if (ib_showing != nullptr) { return ib_showing; }
	}
	return nullptr;
}

void info_button::Update()
{
	if(btn->IsFocus()) 
		App->uimanager->SetDescriptionHUDDescription(this);

}

info_button* info_button::AddButton(iPoint position, iPoint atlas, Task* task, bool delete_button)
{
	info_button*  new_if = new info_button(position, atlas, task, delete_button);
	if(!contain_butons)
	{
		ShowButtonsInside* show_buttoins_task = (ShowButtonsInside*)this->task;
		show_buttoins_task->SetInfoButton(this);
		
		ExitButonsInside* back_task = new ExitButonsInside();
		back_ib = new info_button(iPoint(4, 2), { 1112,962 }, back_task, false);
		back_task->SetInfoButton(this);
		buttons_inside.push_back(back_ib);
		contain_butons = true;
	}
	buttons_inside.push_back(new_if);
	return new_if;
}

void info_button::CreateButton(BUILDING_PANELINFO b_panel_type, Building* b_selected)
{
	btn = App->uimanager->AddButton(
	{ PANEL_XPOSITION + (ICON_SEPARATION * position.x),PANEL_YPOSITION + (ICON_SEPARATION * position.y),ICON_SIZE, ICON_SIZE },
	{ atlas.x, atlas.y,ICON_ATLASSIZE, ICON_ATLASSIZE }, true);
	btn->SetTask(task);
	btn->SetNotDeleteTask();
	btn->SetFxSound(App->audio->fx_click_btn);
	PrepareButton(b_panel_type, b_selected);
}

void info_button::PrepareButton(BUILDING_PANELINFO b_panel_type, Building* b_selected)
{
	SDL_Rect atlas_grey;
	EntityTask* e_task = (EntityTask*)task;
	if (e_task != nullptr && e_task->GetEntityTaskType() == ET_INVESTIGATION)
	{
		DoInvestigation* do_inv = (DoInvestigation*)e_task;
		if (App->investigations->GetInvestigation(do_inv->GetInvestigationType())->inv_state != INV_S_IDLE)
		{
			if (do_inv->GetInvestigationType() == INV_FIRE_TOWER
				|| do_inv->GetInvestigationType() == INV_ICE_TOWER
				|| do_inv->GetInvestigationType() == INV_AIR_TOWER
				|| do_inv->GetInvestigationType() == INV_CAVALRY_ATTACK
				|| do_inv->GetInvestigationType() == INV_CAVALRY_DEFENSE
				|| do_inv->GetInvestigationType() == INV_ARCHERS_ATTACK
				|| do_inv->GetInvestigationType() == INV_ARCHERS_DEFENSE
				|| do_inv->GetInvestigationType() == INV_INFANTRY_ATTACK
				|| do_inv->GetInvestigationType() == INV_INFANTRY_DEFENSE)
			{
				iPoint icon_grey = GetGreyInvestigationIconPositionFromAtlas(do_inv->GetInvestigationType());
				atlas_grey = { icon_grey.x, icon_grey.y, ICON_ATLASSIZE, ICON_ATLASSIZE };
				btn->SetAtlasBtn(atlas_grey);
				btn->SetFxSound(App->audio->fx_cancelclick_btn);
				do_inv->SetCanInvestigate(false);
			}
		}
		else
		{
			do_inv->SetCanInvestigate(true);
		}
	}
	DeleteTowerTask* delete_task = nullptr;
	UpgradeTowerTask* upgrade_task = nullptr;
	DeleteWallTask* delete_w_task = nullptr;
	UpgradeWallTask* upgrade_w_task = nullptr;
	switch (b_panel_type)
	{
	case BP_TURRET:
		if (IsForDelete())
		{
			delete_task = (DeleteTowerTask*)task;
			delete_task->SetTower((Tower*)b_selected);
		}
		else
		{
			upgrade_task = (UpgradeTowerTask*)task;
			if (App->investigations->GetInvestigation(upgrade_task->GetUpgradeType())->inv_state != INV_S_COMPLETED)
			{
				atlas_grey = btn->GetAtlasRect();
				atlas_grey.x += ATLAS_BUTTONGREY_TOWER_DISTANCE;
				btn->SetAtlasBtn(atlas_grey);
				btn->SetFxSound(App->audio->fx_cancelclick_btn);
				upgrade_task->SetCanUpgrade(false);
			}
			else if (App->investigations->GetInvestigation(upgrade_task->GetUpgradeType())->inv_state == INV_S_COMPLETED)
			{
				upgrade_task->SetTower((Tower*)b_selected);
				upgrade_task->SetCanUpgrade(true);
			}
		}
		break;
	case BP_CANNON:
		if (delete_entity)
		{
			delete_task = (DeleteTowerTask*)task;
			delete_task->SetTower((Tower*)b_selected);
		}
		else
		{
			upgrade_task = (UpgradeTowerTask*)task;
			if (App->investigations->GetInvestigation(upgrade_task->GetUpgradeType())->inv_state != INV_S_COMPLETED)
			{
				atlas_grey = btn->GetAtlasRect();
				atlas_grey.x += ATLASX_BUTTONGREY_CANNON_DISTANCE;
				atlas_grey.y -= ATLASY_BUTTONGREY_CANNON_DISTANCE;
				btn->SetAtlasBtn(atlas_grey);
				btn->SetFxSound(App->audio->fx_cancelclick_btn);
				upgrade_task->SetCanUpgrade(false);
			}
			else if (App->investigations->GetInvestigation(upgrade_task->GetUpgradeType())->inv_state == INV_S_COMPLETED)
			{
				upgrade_task->SetTower((Tower*)b_selected);
				upgrade_task->SetCanUpgrade(true);
			}
		}
		break;
	case BP_TURRET_UPGRADED:
		delete_task = (DeleteTowerTask*)task;
		delete_task->SetTower((Tower*)b_selected);
		break;
	case BP_WOOD_WALL:
		if (delete_entity)
		{
			delete_w_task = (DeleteWallTask*)task;
			delete_w_task->SetWall((Building*)b_selected);
		}
		else
		{
			upgrade_w_task = (UpgradeWallTask*)task;
			upgrade_w_task->SetWall((Building*)b_selected);
		}
		break;
	case BP_STONE_WALL:
		if (delete_entity)
		{
			delete_w_task = (DeleteWallTask*)task;
			delete_w_task->SetWall((Building*)b_selected);
		}
		else
		{
			upgrade_w_task = (UpgradeWallTask*)task;
			upgrade_w_task->SetWall((Building*)b_selected);
		}
		break;
	case BP_BRICK_WALL:
		delete_w_task = (DeleteWallTask*)task;
		delete_w_task->SetWall((Building*)b_selected);
		break;
	}
}

void info_button::ButtonToDelete()
{
	btn->SetToDelete();
}

UIButton* info_button::GetButton() const
{
	return btn;
}

const Task* info_button::GetTask() const
{
	return task;
}

bool info_button::IsForDelete()
{
	return delete_entity;
}

bool info_button::ContainButtons()
{
	return contain_butons;
}

bool info_button::ShowButtons()
{
	return show_buttons;
}

void info_button::SetShowButtons(bool sw)
{
	show_buttons = sw;
}

std::vector<info_button*>* info_button::GetInfoButtons()
{
	return &buttons_inside;
}