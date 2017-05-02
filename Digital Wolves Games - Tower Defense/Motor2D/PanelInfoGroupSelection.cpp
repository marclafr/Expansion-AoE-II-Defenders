#define MARK_BTN { 220, 666, 29, 29 }
#define BAR_LIFE_CENTER 16

#define BAR_LIFE_PIXELS 29
#define BAR_LIFE { 999, 861, 29, 4 }

#include "PanelInfoGroupSelection.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "Camera.h"

#include "UIGetEntitiesInfo.h"

GroupSelection::~GroupSelection()
{
	for (std::list<entity_selected*>::iterator es_item = es_selection.begin(); es_item != es_selection.end(); ++es_item)
		delete *es_item;
	es_selection.clear();
}

void GroupSelection::Prepare()
{
	std::list<Entity*>::iterator check = selection->begin();

	if ((*check)->GetEntityType() == ENTITY_TYPE::E_UNIT)
		PrepareUnitSelection();
	else
		PrepareNoUnitSelection();

	life_bar = BAR_LIFE;
}

void GroupSelection::PrepareUnitSelection()
{
	std::list<Entity*>::iterator u_item = selection->begin();

	std::list<Entity*>::iterator last_u_item = selection->end();
	last_u_item--;

	while (u_item != selection->end())
	{
		entity_selected* add_entity_selected = new entity_selected();
		Unit* selected = (Unit*)*u_item;

		UIButton* new_btn = App->uimanager->AddButton(MARK_BTN, GetUnitIconPositionFromAtlas(selected->GetUnitType()));
		add_entity_selected->btn_selected = new_btn;
		add_entity_selected->pointer_entity = (Entity*)selected;
		es_selection.push_back(add_entity_selected);

		u_item++;
	}
}

void GroupSelection::PrepareNoUnitSelection()
{
	std::list<Entity*>::iterator e_item = selection->begin();

	std::list<Entity*>::iterator last_e_item = selection->end();
	last_e_item--;

	while (e_item != selection->end())
	{
		entity_selected* add_entity_selected = new entity_selected();
		Building* b_selected = nullptr;
		Resources* r_selected = nullptr;

		UIButton* new_btn = nullptr;
		switch ((*e_item)->GetEntityType())
		{
		case E_BUILDING:
			b_selected = (Building*)*e_item;
			new_btn = App->uimanager->AddButton(MARK_BTN, GetBuildingIconPositionFromAtlas(b_selected->GetBuildingType()));
			break;

		case E_RESOURCE:
			r_selected = (Resources*)*e_item;
			new_btn = App->uimanager->AddButton(MARK_BTN, GetResourceIconPositionFromAtlas(r_selected->GetResourceType()));
			break;
		}
		add_entity_selected->btn_selected = new_btn;
		add_entity_selected->pointer_entity = *e_item;
		es_selection.push_back(add_entity_selected);

		e_item++;
	}
}

void GroupSelection::Update()
{
	for (std::list<entity_selected*>::iterator es_item = es_selection.begin(); es_item != es_selection.end(); ++es_item)
	{
		if ((*es_item)->btn_selected->IsFocus())
			if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_UP)
				SetOneEntitySelection((*es_item)->pointer_entity);

		if ((*es_item)->pointer_entity->GetHp() <= 0)
		{
			(*es_item)->btn_selected->SetToDelete(true);
			delete *es_item;
			es_selection.erase(es_item);
		}
	}

	if (es_selection.empty())
		delete_panel_info = true;

	Draw();
}

void GroupSelection::Draw()
{
	std::list<entity_selected*>::iterator es_item = es_selection.begin();

	int count = 0;

	while (es_item != es_selection.end())
	{
		if ((*es_item)->pointer_entity->GetHp() > 0)
		{
			UIButton* uibutton = (*es_item)->btn_selected;

			uibutton->Set({ 220 + (29 * count++), 666, 29, 29 }, uibutton->GetAtlasRect());

			SDL_Rect mark_btn{ 999, 827, 29, 29 };
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->GetPosRect().x + 2 - App->render->camera->GetPosition().x, uibutton->GetPosRect().y + 2 - App->render->camera->GetPosition().y, &uibutton->GetAtlasRect(), SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->GetPosRect().x - App->render->camera->GetPosition().x, uibutton->GetPosRect().y - App->render->camera->GetPosition().y, &mark_btn, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);

			int rest_life_bar = 0;
			int height_correction = 0;
			ENTITY_TYPE e_type = (*es_item)->pointer_entity->GetEntityType();
			Entity* e_selected = (*es_item)->pointer_entity;
			Building* b_selected = nullptr;
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
				rest_life_bar = ReturnValueBarHPBuilding(b_selected->GetBuildingType(), b_selected->GetHp());
				height_correction = ReturnValueHeightCorrectionBuilding(b_selected->GetBuildingType());
				life_bar.w = ReturnValueBarHPBuilding(b_selected->GetBuildingType(), b_selected->GetHp(), BAR_LIFE_PIXELS);
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
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), e_selected->GetX() - BAR_LIFE_CENTER, e_selected->GetY() - height_correction, &mark_life_bar_red, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), e_selected->GetX() - BAR_LIFE_CENTER, e_selected->GetY() - height_correction, &mark_life_bar_green, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, false);

			//Down bar_life
			App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), uibutton->GetPosRect().x - App->render->camera->GetPosition().x, uibutton->GetPosRect().y + 25 - App->render->camera->GetPosition().y, &life_bar, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
		}

		es_item++;
	}
}

void GroupSelection::SetOneEntitySelection(Entity* e_selected)
{
	e_oneselected = e_selected;
}