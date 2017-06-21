#ifndef __UIHUDPANELBUTTONS_H__
#define __UIHUDPANELBUTTONS_H__

#include "UIComponents.h"
#include "UIButton.h"

class Task;
enum ENTITY_TYPE;
enum UNIT_TYPE;
enum BUILDING_TYPE;
enum TOWER_TYPE;
enum Side;

enum BUILDING_PANELINFO
{
	BP_NONE,
	BP_NO_SELECTION,
	BP_TOWNHALL,
	BP_UNIVERSITY,
	BP_TURRET,
	BP_CANNON,
	BP_TURRET_UPGRADED,
	BP_WOOD_WALL,
	BP_STONE_WALL,
	BP_BRICK_WALL,
	BP_UNIT
};

struct info_button
{
private:
	bool contain_butons = false;
	bool show_buttons = false;
	info_button* back_ib = nullptr;
	UIButton* btn;
	iPoint position;
	iPoint atlas;
	Task* task;
	bool delete_entity;
	std::vector<info_button*> buttons_inside;

public:
	info_button(iPoint position, iPoint atlas, Task* task, bool delete_entity) : position(position), atlas(atlas), task(task), delete_entity(delete_entity){}
	~info_button()
	{
		DELETE_PTR(task);
		if (contain_butons)
			for (std::vector<info_button*>::iterator ib_item = buttons_inside.begin(); ib_item != buttons_inside.end(); ++ib_item)
				DELETE_PTR(*ib_item)
	}

	void Update();

	info_button* AddButton(iPoint position, iPoint atlas, Task* task, bool delete_button = false);
	void CreateButton(BUILDING_PANELINFO b_panel_type, Building* b_selected);
	void PrepareButton(BUILDING_PANELINFO b_panel_type, Building* b_selected);
	void ButtonToDelete();
	UIButton* GetButton() const;
	const Task* GetTask() const;
	bool IsForDelete();
	bool ContainButtons();
	bool ShowButtons();
	void SetShowButtons(bool);
	std::vector<info_button*>* GetInfoButtons();
};

class UIHUDPanelButtons : public UIComponents
{
private:
	std::vector<info_button*> panel_no_selection;
	std::vector<info_button*> panel_townhall;
	std::vector<info_button*> panel_university;
	std::vector<info_button*> panel_turret;
	std::vector<info_button*> panel_cannon;
	std::vector<info_button*> panel_turret_upgraded;
	std::vector<info_button*> panel_wood_wall;
	std::vector<info_button*> panel_stone_wall;
	std::vector<info_button*> panel_brick_wall;
	std::vector<info_button*>* panel_seleted = nullptr;
	BUILDING_PANELINFO panel_seleted_type = BP_NONE;
	Building* b_selected = nullptr;

	bool want_to_reset = false;
	bool want_enter = false;
	info_button* ib_reset = nullptr;

public:
	UIHUDPanelButtons(UICOMPONENT_TYPE type);
	~UIHUDPanelButtons();

	void SetPanel(Building* building);
	bool Update();
	//x - 0 to 4 | y - 0 to 2 | Max 15 buttons
	info_button* AddButton(BUILDING_PANELINFO type,iPoint position, iPoint atlas, Task* task, bool delete_button = false);

	void CreatePanel();
	void DeletePanel();
	void Reset();

	void WantReset(info_button*, bool);
	std::vector<info_button*>* isInsideButton(std::vector<info_button*>*);
	info_button* UIHUDPanelButtons::search_infobutton(std::vector<info_button*>*, std::vector<info_button*>*);
};

#endif // __UIHUDPANELBUTTONS_H__