#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "Entity.h"
#include "Buildings.h"
#include "Units.h"

//UI_ELEMENTS
#include "UI_AppearingLabel.h"
#include "UI_Button.h"
#include "UI_HPBar.h"
#include "UI_Image.h"
#include "UI_Label.h"
//----------

#include "j1Gui.h"

#define ONE_SELECTED_IMAGE_POS { 300, 300 }
#define SPACE_BETWEEN_SELECTED_ICONS 30
#define MAX_ICONS_IN_ROW 3

j1Gui::j1Gui() : j1Module()
{
	name.assign("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.c_str(), T_UI);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	for (int i = 0; i < ui_elements.size(); i++)
		ui_elements[i]->Update();

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (int i = 0; i < ui_elements.size(); i++)
		ui_elements[i]->Draw(atlas);

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	ui_elements.clear();

	return true;
}

UI_Image * j1Gui::CreateImage(iPoint pos, SDL_Rect atlas_rect, bool not_in_world)
{
	UI_Image* ret = nullptr;
	ret = new UI_Image(pos, atlas_rect, not_in_world);
	ui_elements.push_back(ret);
	return ret;
}

UI_Button * j1Gui::CreateButton(iPoint pos, SDL_Rect atlas_rect_idle, SDL_Rect atlas_rect_mouse_on_top, SDL_Rect atlas_rect_clicking, char* description, bool not_in_world, SDL_Rect description_background_rect)
{
	UI_Button* ret = nullptr;
	ret = new UI_Button(pos, atlas_rect_idle, atlas_rect_mouse_on_top, atlas_rect_clicking, description, description_background_rect, not_in_world);
	ui_elements.push_back(ret);
	return ret;
}

UI_Label * j1Gui::CreateLabel(iPoint pos, SDL_Rect atlas_rect, char * txt, bool not_in_world)
{
	UI_Label* ret = nullptr;
	ret = new UI_Label(pos, atlas_rect, txt, not_in_world);
	ui_elements.push_back(ret);
	return ret;
}

void j1Gui::CreatePanel(std::vector<Entity*> selection)
{
	if (selection.size() == 1)
	{
		Building* building;
		Unit* unit;
		switch (selection[0]->GetEntityType())
		{
		case E_NO_ENTITY:
			break;
		case E_BUILDING:
			building = (Building*)selection[0];
			switch (building->GetBuildingType())
			{
			case B_NO_BUILDING:
				break;
			case B_TURRET:
			case B_CANNON:
			case B_TURRET_UPGRADED_FIRE:
			case B_TURRET_UPGRADED_ICE:
			case B_TURRET_UPGRADED_AIR:
			case B_CANNON_UPGRADED_FIRE:
			case B_CANNON_UPGRADED_ICE:
			case B_CANNON_UPGRADED_AIR:
				break;
			case B_WOOD_WALL:
			case B_STONE_WALL:
			case B_BRICK_WALL:
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
			unit = (Unit*)selection[0];
			CreateImage(ONE_SELECTED_IMAGE_POS, GetUnitIcon(unit));
			break;
		case E_RESOURCE:
			break;

		default:
			break;
		}
	}
	else if (selection.size() > 1)
	{
		Building* building;
		Unit* unit;
		iPoint correct_pos;
		int row_num = 0;
		int row_num_element = 0;
		for (int i = 0; i < selection.size(); i++)
		{
			switch (selection[i]->GetEntityType())
			{
			case E_NO_ENTITY:
				break;
			case E_BUILDING:
				building = (Building*)selection[i];
				switch (building->GetBuildingType())
				{
				case B_NO_BUILDING:
					break;
				case B_TURRET:
				case B_CANNON:
				case B_TURRET_UPGRADED_FIRE:
				case B_TURRET_UPGRADED_ICE:
				case B_TURRET_UPGRADED_AIR:
				case B_CANNON_UPGRADED_FIRE:
				case B_CANNON_UPGRADED_ICE:
				case B_CANNON_UPGRADED_AIR:
					break;
				case B_WOOD_WALL:
				case B_STONE_WALL:
				case B_BRICK_WALL:
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
				correct_pos = ONE_SELECTED_IMAGE_POS;
				if (i >= row_num * MAX_ICONS_IN_ROW)
				{
					row_num++;
					row_num_element = 0;
				}
				correct_pos.x += SPACE_BETWEEN_SELECTED_ICONS * row_num_element;
				row_num_element++;
				correct_pos.y += SPACE_BETWEEN_SELECTED_ICONS * (row_num - 1);
				CreateImage(correct_pos, GetUnitIcon(unit));
				break;
			case E_RESOURCE:
				break;

			default:
				break;
			}
		}
	}
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

SDL_Rect j1Gui::GetUnitIcon(Unit * unit)
{
	switch (unit->GetUnitType())
	{
		//TODO: Get All atlas icons pos
	default:
		return { 774, 962, 25, 25 };// contorno { 1092, 827, 29, 29 };
		break;
	}
	return { 0,0,0,0 };
}

// class Gui ---------------------------------------------------

UI_Element::UI_Element(iPoint pos, SDL_Rect atlas_rect, bool not_in_world) : pos(pos), atlas_rect(atlas_rect), not_in_world(not_in_world)
{
}

UI_Element::~UI_Element()
{
}

bool UI_Element::Update()
{
	return true;
}

bool UI_Element::Draw(SDL_Texture* atlas)
{
	return true;
}

const int UI_Element::GetX()
{
	return pos.x;
}

const int UI_Element::GetY()
{
	return pos.y;
}

const SDL_Rect UI_Element::GetAtlasRect()
{
	return atlas_rect;
}
