#include "PanelInfoGroupSelection.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Scene.h"

#include "UIGetEntitiesInfo.h"

#define MARK_BTN { 220, 666, 29, 29 }
#define BAR_LIFE_CENTER 16

#define BAR_LIFE_PIXELS 29
#define BAR_LIFE { 999, 861, 29, 4 }

GroupSelection::~GroupSelection()
{
	for (std::vector<entity_selected*>::iterator es_item = es_selection.begin(); es_item != es_selection.end(); ++es_item)
	{
		(*es_item)->btn_selected->SetToDelete();
		DELETE_PTR(*es_item);
	}
	es_selection.clear();
}

void GroupSelection::Prepare()
{
	std::vector<Entity*>::iterator check = App->scene->selection.begin();

	if ((*check)->GetEntityType() == ENTITY_TYPE::E_UNIT)
		PrepareUnitSelection();
	else
		PrepareNoUnitSelection();

	life_bar = BAR_LIFE;
	e_oneselected = nullptr;
}

void GroupSelection::PrepareUnitSelection()
{
	for (std::vector<Entity*>::iterator u_item = App->scene->selection.begin(); u_item != App->scene->selection.end(); ++u_item)
	{
		entity_selected* add_entity_selected = new entity_selected();
		Unit* selected = (Unit*)*u_item;

		iPoint u_position(GetUnitIconPositionFromAtlas(selected->GetUnitType()));
		UIButton* new_btn = App->uimanager->AddButton(MARK_BTN, { u_position.x,u_position.y, ICON_SIZE, ICON_SIZE });
		add_entity_selected->btn_selected = new_btn;
		add_entity_selected->pointer_entity = (Entity*)selected;
		es_selection.push_back(add_entity_selected);
	}
}

void GroupSelection::PrepareNoUnitSelection()
{
	for (std::vector<Entity*>::iterator e_item = App->scene->selection.begin(); e_item != App->scene->selection.end(); ++e_item)
	{
		entity_selected* add_entity_selected = new entity_selected();
		Building* b_selected = nullptr;
		Resources* r_selected = nullptr;
		Tower* t_selected = nullptr;

		UIButton* new_btn = nullptr;
		iPoint atlas_icon{0,0};
		switch ((*e_item)->GetEntityType())
		{
		case E_BUILDING:
			b_selected = (Building*)*e_item;

			switch (b_selected->GetBuildingType())
			{
			case B_TURRET:
				t_selected = (Tower*)*e_item;
				atlas_icon = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			case B_CANNON:
				t_selected = (Tower*)*e_item;
				atlas_icon = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			case B_TURRET_UPGRADED_FIRE:
				t_selected = (Tower*)*e_item;
				atlas_icon = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			case B_TURRET_UPGRADED_ICE:
				t_selected = (Tower*)*e_item;
				atlas_icon = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			case B_TURRET_UPGRADED_AIR:
				t_selected = (Tower*)*e_item;
				atlas_icon = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			case B_CANNON_UPGRADED_FIRE:
				t_selected = (Tower*)*e_item;
				atlas_icon = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			case B_CANNON_UPGRADED_ICE:
				t_selected = (Tower*)*e_item;
				atlas_icon = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			case B_CANNON_UPGRADED_AIR:
				t_selected = (Tower*)*e_item;
				atlas_icon = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			default:
				atlas_icon = GetBuildingIconPositionFromAtlas(b_selected->GetBuildingType());
				new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
				break;
			}
			break;
		case E_RESOURCE:
			r_selected = (Resources*)*e_item;
			atlas_icon = GetResourceIconPositionFromAtlas(r_selected->GetResourceType());
			new_btn = App->uimanager->AddButton(MARK_BTN, { atlas_icon.x, atlas_icon.y, ICON_SIZE, ICON_SIZE });
			break;
		}
		add_entity_selected->btn_selected = new_btn;
		add_entity_selected->pointer_entity = *e_item;
		es_selection.push_back(add_entity_selected);
	}
}

void GroupSelection::Update()
{
	for (std::vector<entity_selected*>::iterator es_item = es_selection.begin(); es_item != es_selection.end(); ++es_item)
	{
		if ((*es_item)->btn_selected->IsFocus())
			if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_UP)
				SetOneEntitySelection((*es_item)->pointer_entity);

		if ((*es_item)->pointer_entity->GetHp() <= 0)
		{
			(*es_item)->btn_selected->SetToDelete();
			delete *es_item;
			es_selection.erase(es_item);
			es_item--;
		}
	}

	if (es_selection.empty())
		delete_panel_info = true;

	Draw();
}

void GroupSelection::Draw()
{
	int count = 0;

	for (std::vector<entity_selected*>::iterator es_item = es_selection.begin(); es_item != es_selection.end(); ++es_item)
	{
		if ((*es_item)->pointer_entity->GetHp() > 0)
		{
			UIButton* uibutton = (*es_item)->btn_selected;

			uibutton->Set({ 220 + (29 * count++), 666, 29, 29 }, uibutton->GetAtlasRect());

			SDL_Rect mark_btn{ 999, 827, 29, 29 };
			App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->GetPosRect().x + 2 - App->render->camera->GetPosition().x, uibutton->GetPosRect().y + 2 - App->render->camera->GetPosition().y, &uibutton->GetAtlasRect());
			App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->GetPosRect().x - App->render->camera->GetPosition().x, uibutton->GetPosRect().y - App->render->camera->GetPosition().y, &mark_btn);

			int rest_life_bar = 0;
			int height_correction = 0;
			ENTITY_TYPE e_type = (*es_item)->pointer_entity->GetEntityType();
			Entity* e_selected = (*es_item)->pointer_entity;
			Building* b_selected = nullptr;
			Tower* t_selected = nullptr;
			Resources* r_selected = nullptr;
			Unit* u_selected = nullptr;
			switch (e_type)
			{
			case E_UNIT:
				u_selected = (Unit*)e_selected;
				rest_life_bar = ReturnValueBarHPUnit(u_selected->GetUnitType(), u_selected->GetHp());
				height_correction = ReturnValueHeightCorrectionUnit(u_selected->GetUnitClass());
				life_bar.w = ReturnValueBarHPUnit(u_selected->GetUnitType(), u_selected->GetHp(), BAR_LIFE_PIXELS);
				break;

			case E_BUILDING:
				b_selected = (Building*)e_selected;
				if (b_selected->GetBuildingType() == B_CANNON ||
					b_selected->GetBuildingType() == B_TURRET ||
					b_selected->GetBuildingType() == B_TURRET_UPGRADED_FIRE || 
					b_selected->GetBuildingType() == B_TURRET_UPGRADED_ICE || 
					b_selected->GetBuildingType() == B_TURRET_UPGRADED_AIR ||
					b_selected->GetBuildingType() == B_CANNON_UPGRADED_FIRE ||
					b_selected->GetBuildingType() == B_CANNON_UPGRADED_ICE || 
					b_selected->GetBuildingType() == B_CANNON_UPGRADED_AIR)
				{
					t_selected = (Tower*)b_selected;
					rest_life_bar = ReturnValueBarHPTower(t_selected->GetTowerType(), t_selected->GetHp());
					height_correction = ReturnValueHeightCorrectionTowers(t_selected->GetTowerType());
					life_bar.w = ReturnValueBarHPTower(t_selected->GetTowerType(), t_selected->GetHp(), BAR_LIFE_PIXELS);
				}
				else {
					rest_life_bar = ReturnValueBarHPBuilding(b_selected->GetBuildingType(), b_selected->GetHp());
					height_correction = ReturnValueHeightCorrectionBuilding(b_selected->GetBuildingType());
					life_bar.w = ReturnValueBarHPBuilding(b_selected->GetBuildingType(), b_selected->GetHp(), BAR_LIFE_PIXELS);
				}
				break;

			case E_RESOURCE:
				r_selected = (Resources*)e_selected;
				rest_life_bar = ReturnValueBarHPResource(r_selected->GetResourceType(), r_selected->GetHp());
				height_correction = ReturnValueHeightCorrectionResource(r_selected->GetResourceType());
				life_bar.w = ReturnValueBarHPResource(r_selected->GetResourceType(), r_selected->GetHp(), BAR_LIFE_PIXELS);
				break;
			}

			//Bar life unit
			SDL_Rect mark_life_bar_red{ 1059, 832, 32, 4 };
			SDL_Rect mark_life_bar_green{ 1059, 827, rest_life_bar, 4 };
			App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), e_selected->GetX() - BAR_LIFE_CENTER, e_selected->GetY() - height_correction, &mark_life_bar_red);
			App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), e_selected->GetX() - BAR_LIFE_CENTER, e_selected->GetY() - height_correction, &mark_life_bar_green);

			//Down bar_life
			App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->GetPosRect().x - App->render->camera->GetPosition().x, uibutton->GetPosRect().y + 25 - App->render->camera->GetPosition().y, &life_bar);
		}
	}
}

void GroupSelection::SetOneEntitySelection(Entity* e_selected)
{
	e_oneselected = e_selected;
}