#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "Entity.h"
#include "Buildings.h"
#include "Towers.h"
#include "Units.h"
#include "j1FileSystem.h"

//UI_ELEMENTS
#include "UI_AppearingLabel.h"
#include "UI_Button.h"
#include "UI_HPBar.h"
#include "UI_Image.h"
#include "UI_Label.h"
//----------

#include "j1Gui.h"

/*
#define ONE_SELECTED_IMAGE_POS { 300, 300 }	//Position of the first icon in the selection (pixels)
#define ENTITY_ATTACK_ICON_POS { 350, 300 } //Position of the attack icon in the selection (pixels)
#define ENTITY_ARMOR_ICON_POS  { 350, 330 } //Position of the armor icon in the selection (pixels)
#define ENTITY_RANGE_ICON_POS  { 350, 360 } //Position of the range icon in the selection (pixels)
#define ATTRIBUTES_TEXT_DISPLACEMENT 43 // X Displacement from the icons to the numbers of the attribute: i.e.: *Attack Icon*  10 (pixels)
#define ENTITY_ATTACK_ICON_RECT { 956, 858, 38, 22 } //Attack icon rectangle in atlas
#define ENTITY_ARMOR_ICON_RECT { 956, 902, 37, 19 } //Armor icon rectangle in atlas
#define ENTITY_RANGE_ICON_RECT { 956, 881, 35, 20 } //Range icon rectangle in atlas
#define SPACE_BETWEEN_SELECTED_ICONS 30 //Space between one icon and another in a multiple entity selection
#define MAX_ICONS_IN_ROW 3	//Max amount of icons there can be in a row before going into the bottom of them
*/
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

	//Load data from gui/GuiDat.xml folder
	std::string gui_data_folder = "gui/GuiData.xml";

	char* buff = nullptr;
	int size = App->fs->Load(gui_data_folder.c_str(), &buff);
	pugi::xml_document gui_data;
	pugi::xml_parse_result result = gui_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading GUI DATA from gui/GuiData.xml file: %s", result.description());
		return false;
	}

	pugi::xml_node gui_data_node = gui_data.child("gui").first_child();
	while (gui_data_node != NULL)
	{
		std::string data_string = gui_data_node.attribute("n").as_string();
		GUI_DATA_NAME data_name = DataStr2Enum(data_string.c_str());

		switch (data_name)
		{
			//iPoints---
		case SELECTED_ICON_START_POS:
			data.selection_start_pos = { gui_data_node.attribute("x").as_int(), gui_data_node.attribute("y").as_int() };
			break;
		case ATTACK_ICON_POS:
			data.attack_icon_pos = { gui_data_node.attribute("x").as_int(), gui_data_node.attribute("y").as_int() };
			break;
		case ARMOR_ICON_POS:
			data.armor_icon_pos = { gui_data_node.attribute("x").as_int(), gui_data_node.attribute("y").as_int() };
			break;
		case RANGE_ICON_POS:
			data.range_icon_pos = { gui_data_node.attribute("x").as_int(), gui_data_node.attribute("y").as_int() };
			break;
			//-------------

			//SDL_Rects---
		case ATTACK_ICON_RECT:
			data.attack_icon = { gui_data_node.attribute("x").as_int(),gui_data_node.attribute("y").as_int(),gui_data_node.attribute("w").as_int(),gui_data_node.attribute("h").as_int() };
			break;
		case ARMOR_ICON_RECT:
			data.armor_icon = { gui_data_node.attribute("x").as_int(),gui_data_node.attribute("y").as_int(),gui_data_node.attribute("w").as_int(),gui_data_node.attribute("h").as_int() };
			break;
		case RANGE_ICON_RECT:
			data.range_icon = { gui_data_node.attribute("x").as_int(),gui_data_node.attribute("y").as_int(),gui_data_node.attribute("w").as_int(),gui_data_node.attribute("h").as_int() };
			break;
			//--------------

			//Numbers
		case MAX_ICONS_IN_ROW:
			data.max_icons_in_row = gui_data_node.attribute("i").as_uint();
			break;
		case SPACE_BETWEEN_SELECTED_ICONS:
			data.space_between_selected_icons = gui_data_node.attribute("i").as_uint();
			break;
		case ATTRIBUTES_TEXT_DISPLACEMENT:
			data.attributes_displacement = gui_data_node.attribute("i").as_uint();
			break;
			//--------------

		default:
			break;
		}

		gui_data_node = gui_data_node.next_sibling();
	}

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
	//One entity selected
	if (selection.size() == 1)
	{
		Building* building;
		Tower* tower;
		Unit* unit;
		iPoint attribute_pos;
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
				tower = (Tower*)selection[0];
				//Attack
				CreateImage(data.attack_icon_pos, data.attack_icon);
				attribute_value_attack = std::to_string(tower->GetAttack());
				attribute_pos = data.attack_icon_pos;
				attribute_pos.x += data.attributes_displacement;
				CreateLabel(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_attack.c_str());
				//Armor
				ShowBuildingArmor(building);
				//Range			
				CreateImage(data.range_icon_pos, data.range_icon);
				attribute_value_range = std::to_string(tower->GetRange());
				attribute_pos = data.range_icon_pos;
				attribute_pos.x += data.attributes_displacement;
				CreateLabel(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_range.c_str());
				break;
			case B_WOOD_WALL:
			case B_STONE_WALL:
			case B_BRICK_WALL:
				ShowBuildingArmor(building);
				break;
			case B_TOWNHALL:
				ShowBuildingArmor(building);
				break;
			case B_UNIVERSITY:
				ShowBuildingArmor(building);
				break;

			default:
				break;
			}
			break;
		case E_UNIT:
			unit = (Unit*)selection[0];
			CreateImage(data.selection_start_pos, GetUnitIcon(unit));
			//Attack
			CreateImage(data.attack_icon_pos, data.attack_icon);
			attribute_value_attack = std::to_string(unit->GetAttack());
			attribute_pos = data.attack_icon_pos;
			attribute_pos.x += data.attributes_displacement;
			CreateLabel(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_attack.c_str());
			//Armor
			CreateImage(data.armor_icon_pos, data.armor_icon);
			attribute_value_armor = std::to_string(unit->GetArmor());
			attribute_pos = data.armor_icon_pos;
			attribute_pos.x += data.attributes_displacement;
			CreateLabel(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_armor.c_str());
			//Range
			if (unit->GetUnitClass() == C_ARCHER || unit->GetUnitType() == U_MANGONEL)
			{
				CreateImage(data.range_icon_pos, data.range_icon);
				attribute_value_range = std::to_string(unit->GetRange());
				attribute_pos = data.range_icon_pos;
				attribute_pos.x += data.attributes_displacement;
				CreateLabel(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_range.c_str());
			}
			//----
			break;
		case E_RESOURCE:
			break;

		default:
			break;
		}
	}
	//--------------------------
	//Multiple entities selected
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
				correct_pos = data.selection_start_pos;
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
	//--------------------------
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

GUI_DATA_NAME j1Gui::DataStr2Enum(const std::string name)
{
	if (name == "rect_attack_icon")
		return ATTACK_ICON_RECT;

	else if (name == "rect_armor_icon")
		return ARMOR_ICON_RECT;

	else if (name == "rect_range_icon")
		return RANGE_ICON_RECT;

	else if (name == "selection_start_pos")
		return SELECTED_ICON_START_POS;

	else if (name == "pos_attack_icon")
		return ATTACK_ICON_POS;

	else if (name == "pos_armor_icon")
		return ARMOR_ICON_POS;

	else if (name == "pos_range_icon")
		return RANGE_ICON_POS;

	else if (name == "max_icons_in_a_row")
		return MAX_ICONS_IN_ROW;

	else if (name == "space_between_selected_icons")
		return SPACE_BETWEEN_SELECTED_ICONS;

	else if (name == "attributes_displacement")
		return ATTRIBUTES_TEXT_DISPLACEMENT;
		
	return NO_NAME;
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

void j1Gui::ShowBuildingArmor(Building * building)
{
	CreateImage(data.armor_icon_pos, data.armor_icon);
	attribute_value_armor = std::to_string(building->GetArmor());
	iPoint attribute_pos = data.armor_icon_pos;
	attribute_pos.x += data.attributes_displacement;
	CreateLabel(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_armor.c_str());
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
