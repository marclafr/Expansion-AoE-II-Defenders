#include "j1App.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "UI_HPBar.h"
#include "Entity.h"
#include "Buildings.h"
#include "Towers.h"
#include "Units.h"
#include "Camera.h"
#include "Task.h"
#include "p2Math.h"
#include "UI_PanelInfo.h"

UI_PanelInfoSingleEntity::UI_PanelInfoSingleEntity(iPoint pos, SDL_Rect panel_background_rect, Entity* entity) : UI_Element(UI_E_PANEL_INFO_SINGLE, pos, panel_background_rect)
{
	Building* building = nullptr;
	Tower* tower = nullptr;
	Unit* unit = nullptr;
	iPoint attribute_pos;

	switch (entity->GetEntityType())
	{
	case E_BUILDING:
		building = (Building*)entity;
		//Icon
		//TODO: 
		//entity_icon = new UI_Image(App->gui->data.selection_start_pos, App->gui->GetBuildingIcon(unit));
		if (building->GetBuildingType() == B_TOWER)
		{
			tower = (Tower*) building;
			range_icon = new UI_Image(App->gui->data.range_icon_pos, App->gui->data.range_icon);
			//attribute_value_range = std::to_string(tower->GetRange());
			attribute_pos = App->gui->data.range_icon_pos;
			attribute_pos.x += App->gui->data.attributes_displacement;
			range_label = new UI_Label(attribute_pos, DEFAULT_BACKGROUND_RECT_TEXT, (char*)attribute_value_range.c_str());
		}
		break;

	case E_UNIT:
		unit = (Unit*)entity;
		//Icon
		entity_icon = new UI_Image(App->gui->data.selection_start_pos, App->gui->GetUnitIcon(unit));
		//Range
		range_icon = new UI_Image(App->gui->data.range_icon_pos, App->gui->data.range_icon);
		attribute_value_range = std::to_string(unit->GetRange());
		attribute_pos = App->gui->data.range_icon_pos;
		attribute_pos.x += App->gui->data.attributes_displacement;
		range_label = new UI_Label(attribute_pos, DEFAULT_BACKGROUND_RECT_TEXT, (char*)attribute_value_range.c_str());
		break;

	case E_RESOURCE:
		break;

	case E_NO_ENTITY:
	default:
		break;
	}

	//Attack
	attack_icon = new UI_Image(App->gui->data.attack_icon_pos, App->gui->data.attack_icon);
	attribute_value_attack = std::to_string(entity->GetAttack());
	attribute_pos = App->gui->data.attack_icon_pos;
	attribute_pos.x += App->gui->data.attributes_displacement;
	attack_label = new UI_Label(attribute_pos, DEFAULT_BACKGROUND_RECT_TEXT, (char*)attribute_value_attack.c_str());
	//Armor
	armor_icon = new UI_Image(App->gui->data.armor_icon_pos, App->gui->data.armor_icon);
	attribute_value_armor = std::to_string(entity->GetArmor());
	attribute_pos = App->gui->data.armor_icon_pos;
	attribute_pos.x += App->gui->data.attributes_displacement;
	armor_label = new UI_Label(attribute_pos, DEFAULT_BACKGROUND_RECT_TEXT, (char*)attribute_value_armor.c_str());
}

UI_PanelInfoSingleEntity::~UI_PanelInfoSingleEntity()
{
	DELETE_PTR(entity_icon);

	DELETE_PTR(attack_icon);
	DELETE_PTR(armor_icon);
	DELETE_PTR(range_icon);

	DELETE_PTR(attack_label);
	DELETE_PTR(armor_label);
	DELETE_PTR(range_label);
}

bool UI_PanelInfoSingleEntity::Draw(SDL_Texture * atlas, int alpha)
{
	if (showing)
	{
		//Draw the panel background
		if (not_in_world == true)
			App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
		else
			App->render->PushUISprite(atlas, pos.x, pos.y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);

		if (entity_icon != nullptr)
			entity_icon->Draw(atlas, alpha);
		if (attack_icon != nullptr)
			attack_icon->Draw(atlas, alpha);
		if (armor_icon != nullptr)
			armor_icon->Draw(atlas, alpha);
		if (range_icon != nullptr)
			range_icon->Draw(atlas, alpha);
		if (attack_label != nullptr)
			attack_label->Draw(atlas, alpha);
		if (armor_label != nullptr)
			armor_label->Draw(atlas, alpha);
		if (range_label != nullptr)
			range_label->Draw(atlas, alpha);
	}
	return true;
}

UI_PanelInfoMultipleEntities::UI_PanelInfoMultipleEntities(iPoint pos, SDL_Rect panel_background_rect, std::vector<Entity*> selection) : UI_Element(UI_E_PANEL_INFO_MULTIPLE, pos, panel_background_rect)
{
	Building* building;
	Unit* unit;
	iPoint correct_pos;
	int row_num = 0;
	int row_num_element = 0;
	for (int i = 0; i < selection.size(); i++)
	{
		correct_pos = App->gui->data.selection_start_pos;
		if (i >= row_num * App->gui->data.max_icons_in_row)
		{
			row_num++;
			row_num_element = 0;
		}
		correct_pos.x += App->gui->data.space_between_selected_icons * row_num_element;
		row_num_element++;
		correct_pos.y += App->gui->data.space_between_selected_icons * (row_num - 1);

		switch (selection[i]->GetEntityType())
		{
		case E_NO_ENTITY:
			break;
		case E_BUILDING:
			building = (Building*)selection[i];
			switch (building->GetBuildingType())
			{
				//TODO: Create its respective ICON
			case B_NO_BUILDING:
				break;
			case B_TOWER:
				break;
			case B_WALL:
				break;
			case B_TOWNHALL:
				break;
			case B_UNIVERSITY:
				break;

			default:
				break;
			}
			break;
		case E_UNIT:
			unit = (Unit*)selection[i];
			panel_buttons.push_back(new UI_Button(correct_pos, App->gui->GetUnitIcon(unit), App->gui->GetUnitIcon(unit), App->gui->GetUnitIcon(unit), App->gui->GetUnitName(unit)));
			break;
		case E_RESOURCE:
			break;

		default:
			break;
		}

		correct_pos.y += App->gui->data.hp_bar_selection_Y_displacement;
		panel_hpbars.push_back(new UI_HPBar(correct_pos, App->gui->data.green_hp_bar_rect, App->gui->data.red_hp_bar_rect, selection[i]));
	}
}

UI_PanelInfoMultipleEntities::~UI_PanelInfoMultipleEntities()
{
	for (int i = 0; i < panel_buttons.size(); i++)
		DELETE_PTR(panel_buttons[i]);

	for (int i = 0; i < panel_hpbars.size(); i++)
		DELETE_PTR(panel_hpbars[i]);
}

bool UI_PanelInfoMultipleEntities::Update()
{
	for (int i = 0; i < panel_buttons.size(); i++)
		panel_buttons[i]->Update();

	return true;
}

bool UI_PanelInfoMultipleEntities::Draw(SDL_Texture * atlas, int alpha)
{
	//Draw the panel background
	if (not_in_world == true)
		App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
	else
		App->render->PushUISprite(atlas, pos.x, pos.y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);

	for (int i = 0; i < panel_buttons.size(); i++)
		panel_buttons[i]->Draw(atlas, alpha);

	for (int i = 0; i < panel_hpbars.size(); i++)
		panel_hpbars[i]->Draw(atlas, alpha);

	return true;
}

UI_PanelButtons::UI_PanelButtons(iPoint pos, SDL_Rect panel_background_rect, Task * task, SDL_Rect rect_idle, int num_buttons_width, SDL_Rect rect_mouse_on_top, SDL_Rect rect_clicking, char* description, SDL_Rect description_background_rect)
	: UI_Element(UI_E_PANEL_BUTTONS, pos, panel_background_rect), num_buttons_width(num_buttons_width)
{
	if (IsZero(rect_mouse_on_top))
		rect_mouse_on_top = rect_idle;

	if (IsZero(rect_clicking))
		rect_clicking = rect_idle;

	panel_buttons.push_back(new UI_Button({ pos.x, pos.y }, rect_idle, rect_mouse_on_top, rect_clicking, description, description_background_rect));
	tasks.push_back(task);
}

UI_PanelButtons::~UI_PanelButtons()
{
	for (int i = 0; i < panel_buttons.size(); i++)
		DELETE_PTR(panel_buttons[i]);

	for (int i = 0; i < tasks.size(); i++)
		DELETE_PTR(tasks[i]);
}

bool UI_PanelButtons::Update()
{
	for (int i = 0; i < panel_buttons.size(); i++)
		if (panel_buttons[i]->Update())
			tasks[i]->Execute();

	return true;
}

bool UI_PanelButtons::Draw(SDL_Texture * atlas, int alpha)
{
	//Draw the panel background
	if (not_in_world == true)
		App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);
	else
		App->render->PushUISprite(atlas, pos.x, pos.y, &atlas_rect, SDL_FLIP_NONE, 0, 0, 1, 0, alpha);

	for (int i = 0; i < panel_buttons.size(); i++)
		panel_buttons[i]->Draw(atlas, alpha);

	return true;
}

void UI_PanelButtons::AddButton(Task * task, SDL_Rect rect_idle, SDL_Rect rect_mouse_on_top, SDL_Rect rect_clicking, char* description, SDL_Rect description_background_rect)
{
	if (IsZero(rect_mouse_on_top))
		rect_mouse_on_top = rect_idle;

	if (IsZero(rect_clicking))
		rect_clicking = rect_idle;

	//TODO SOLVE POSITIONING
	int column = 0;
	for (column = 0; column < panel_buttons.size(); )
		column++;

	int line = column / num_buttons_width;
	column -= (line * num_buttons_width);

	panel_buttons.push_back(new UI_Button({ pos.x + column*(BUTTON_WIDTH + MARGIN), pos.y + line*(BUTTON_HEIGHT + MARGIN) }, rect_idle, rect_mouse_on_top, rect_clicking, description, description_background_rect));
	tasks.push_back(task);
}