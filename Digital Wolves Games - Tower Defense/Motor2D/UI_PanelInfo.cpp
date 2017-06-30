#include "j1App.h"
#include "UI_Button.h"
#include "UI_Label.h"
#include "Entity.h"
#include "Buildings.h"
#include "Towers.h"
#include "Units.h"
#include "Camera.h"
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
		//TODO: entity_icon = App->gui->CreateImage(App->gui->data.selection_start_pos, App->gui->GetBuildingIcon(unit));
		if (building->IsTower())
		{
			tower = (Tower*) building;
			range_icon = new UI_Image(App->gui->data.range_icon_pos, App->gui->data.range_icon);
			attribute_value_range = std::to_string(tower->GetRange());
			attribute_pos = App->gui->data.range_icon_pos;
			attribute_pos.x += App->gui->data.attributes_displacement;
			range_label = new UI_Label(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_range.c_str());
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
		range_label = new UI_Label(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_range.c_str());
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
	attack_label = new UI_Label(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_attack.c_str());
	//Armor
	armor_icon = new UI_Image(App->gui->data.armor_icon_pos, App->gui->data.armor_icon);
	attribute_value_armor = std::to_string(entity->GetArmor());
	attribute_pos = App->gui->data.armor_icon_pos;
	attribute_pos.x += App->gui->data.attributes_displacement;
	armor_label = new UI_Label(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_armor.c_str());
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

bool UI_PanelInfoSingleEntity::Draw(SDL_Texture * atlas)
{
	//Draw the panel background
	if (not_in_world == true)
		App->render->PushUISprite(atlas, pos.x - App->render->camera->GetPosition().x, pos.y - App->render->camera->GetPosition().y, &atlas_rect);
	else
		App->render->PushUISprite(atlas, pos.x, pos.y, &atlas_rect);	
	
	entity_icon->Draw(atlas);
	attack_icon->Draw(atlas);
	armor_icon->Draw(atlas);
	if (range_icon != nullptr)
		range_icon->Draw(atlas);
	attack_label->Draw(atlas);
	armor_label->Draw(atlas);
	if(range_label != nullptr)
		range_label->Draw(atlas);

	return true;
}

UI_PanelInfoMultipleEntities::UI_PanelInfoMultipleEntities(iPoint pos, SDL_Rect panel_background_rect, std::vector<Entity*> selection) : UI_Element(UI_E_PANEL_INFO_MULTIPLE, pos, panel_background_rect)
{
	//TODO
}

UI_PanelInfoMultipleEntities::~UI_PanelInfoMultipleEntities()
{
	//TODO
}
