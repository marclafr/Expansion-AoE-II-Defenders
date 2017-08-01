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
#include "UI_AppearingElement.h"
#include "UI_Button.h"
#include "UI_HPBar.h"
#include "UI_Image.h"
#include "UI_Label.h"
#include "UI_MultiLabel.h"
#include "UI_Text_Input.h"
#include "UI_PanelInfo.h"
//----------

#include "j1Gui.h"

#define UPLOAD_XMLDATA_ATLAS_RECTS { gui_atlas_icons_data_node.attribute("x").as_int(),gui_atlas_icons_data_node.attribute("y").as_int(),gui_atlas_icons_data_node.attribute("w").as_int(),gui_atlas_icons_data_node.attribute("h").as_int() }

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
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");


	//Load data from gui/GuiAtlasIcons.xml folder
	std::string gui_atlas_icons_data_folder = "gui/GuiAtlasIcons.xml";

	char* buff = nullptr;
	int size = App->fs->Load(gui_atlas_icons_data_folder.c_str(), &buff);
	pugi::xml_document gui_atlas_icons_data;
	pugi::xml_parse_result result = gui_atlas_icons_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading GUI DATA from gui/GuiData.xml file: %s", result.description());
		return false;
	}

	pugi::xml_node gui_atlas_icons_data_node = gui_atlas_icons_data.child("gui").first_child();
	while (gui_atlas_icons_data_node != NULL)
	{
		std::string data_string = gui_atlas_icons_data_node.attribute("n").as_string();

		if (data_string.compare("panel_background_rect") == 0)
			data.panel_background_rect = UPLOAD_XMLDATA_ATLAS_RECTS;

		else if (data_string.compare("rect_attack_icon") == 0)
			data.attack_icon = UPLOAD_XMLDATA_ATLAS_RECTS;

		else if (data_string.compare("rect_armor_icon") == 0)
			data.armor_icon = UPLOAD_XMLDATA_ATLAS_RECTS;

		else if (data_string.compare("rect_range_icon") == 0)
			data.range_icon = UPLOAD_XMLDATA_ATLAS_RECTS;

		else if (data_string.compare("twohandedswordman_icon") == 0)
			data.twohanded_icon = UPLOAD_XMLDATA_ATLAS_RECTS;

		else if (data_string.compare("archer_icon") == 0)
			data.archer_icon = UPLOAD_XMLDATA_ATLAS_RECTS;

		else if (data_string.compare("green_hp_bar_rect") == 0)
			data.green_hp_bar_rect = UPLOAD_XMLDATA_ATLAS_RECTS;

		else if (data_string.compare("red_hp_bar_rect") == 0)
			data.red_hp_bar_rect = UPLOAD_XMLDATA_ATLAS_RECTS;

		gui_atlas_icons_data_node = gui_atlas_icons_data_node.next_sibling();
	}

	//Load data from gui/GuiData.xml folder
	std::string gui_data_folder = "gui/GuiData.xml";

	buff = nullptr;
	size = App->fs->Load(gui_data_folder.c_str(), &buff);
	pugi::xml_document gui_data;
	result = gui_data.load_buffer(buff, size);
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
		case PANEL_INFO_POS:
			data.panel_info_pos = { gui_data_node.attribute("x").as_int(), gui_data_node.attribute("y").as_int() };
			break;
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
		case HP_SELECTION_BAR_Y_DISPLACEMENT:
			data.hp_bar_selection_Y_displacement = gui_data_node.attribute("i").as_uint();
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
	{
		if (!ui_elements[i]->Draw(atlas))
		{
			switch (ui_elements[i]->GetElementType())
			{
			case UI_E_APPEARING_ELEMENT:
				App->gui->DeleteAppearingLabel((UI_AppearingLabel*)ui_elements[i]);
				break;

			default:
				break;
			}
		}
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
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

UI_Label * j1Gui::CreateLabel(iPoint pos, SDL_Rect atlas_rect, char * txt, bool has_background, bool not_in_world)
{
	UI_Label* ret = nullptr;
	ret = new UI_Label(pos, atlas_rect, txt, has_background, not_in_world);
	ui_elements.push_back(ret);
	return ret;
}

UI_MultiLabel * j1Gui::CreateMultiLabel(iPoint pos, SDL_Rect atlas_rect, char * txt, int max_labels, int label_to_remove, bool has_max_labels, bool has_background, bool not_in_world)
{
	UI_MultiLabel* ret = nullptr;
	ret = new UI_MultiLabel(pos, atlas_rect, txt, max_labels, label_to_remove, has_max_labels, has_background, not_in_world);
	ui_elements.push_back(ret);
	return ret;
}

UI_AppearingLabel * j1Gui::CreateAppearingLabel(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, char * txt, bool has_background, bool not_in_world)
{
	UI_AppearingLabel* ret = nullptr;
	ret = new UI_AppearingLabel(pos, atlas_rect, seconds_on_screen, txt, has_background, not_in_world);
	ui_elements.push_back(ret);
	return ret;
}

UI_AppearingImage * j1Gui::CreateAppearingImage(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, bool not_in_world)
{
	UI_AppearingImage* ret = nullptr;
	ret = new UI_AppearingImage(pos, atlas_rect, seconds_on_screen, not_in_world);
	ui_elements.push_back(ret);
	return ret;
}

UI_TextInput * j1Gui::CreateTextInput(iPoint pos, char * txt, FONT_NAME font_name, SDL_Color color, bool not_in_world)
{
	UI_TextInput* ret = nullptr;
	ret = new UI_TextInput(pos, txt, font_name, color, not_in_world);
	ui_elements.push_back(ret);
	return ret;
}

UI_PanelInfoSingleEntity * j1Gui::CreatePanel(Entity * selection)
{
	UI_PanelInfoSingleEntity* ret = nullptr;
	ret = new UI_PanelInfoSingleEntity(data.panel_info_pos, data.panel_background_rect, selection);
	ui_elements.push_back(ret);
	return ret;
}

UI_PanelInfoMultipleEntities* j1Gui::CreatePanel(std::vector<Entity*> selection)
{
	UI_PanelInfoMultipleEntities* ret = nullptr;
	ret = new UI_PanelInfoMultipleEntities(data.panel_info_pos, data.panel_background_rect, selection);
	ui_elements.push_back(ret);
	return ret;
}


void j1Gui::DeleteImage(UI_Image* img)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == img)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(img);
}

void j1Gui::DeleteButton(UI_Button* button)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == button)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(button);
}

void j1Gui::DeleteLabel(UI_Label * label)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == label)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(label);
}

void j1Gui::DeleteMultiLabel(UI_MultiLabel * multilabel)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == multilabel)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(multilabel);
}

void j1Gui::DeleteAppearingLabel(UI_AppearingLabel * appearing_label)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == appearing_label)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(appearing_label);
}

void j1Gui::DeleteAppearingImage(UI_AppearingImage * appearing_image)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == appearing_image)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(appearing_image);
}

void j1Gui::DeleteTextInput(UI_TextInput * txt_input)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == txt_input)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(txt_input);
}

void j1Gui::DeletePanelInfo(UI_PanelInfoSingleEntity * panel_info)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == panel_info)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(panel_info);
}

void j1Gui::DeletePanelInfo(UI_PanelInfoMultipleEntities * panel_info)
{
	for (std::vector<UI_Element*>::iterator it = ui_elements.begin(); it != ui_elements.end(); ++it)
		if (*it == panel_info)
		{
			ui_elements.erase(it);
			break;
		}

	DELETE_PTR(panel_info);
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void j1Gui::SetFocusedText(UI_TextInput* text)
{
	focused_text = text;
}

UI_TextInput * j1Gui::GetFocusedText()
{
	return focused_text;
}

GUI_DATA_NAME j1Gui::DataStr2Enum(const std::string name)
{
	if (name == "panel_info_pos")
		return PANEL_INFO_POS;

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

	else if (name == "hp_bar_selection_Y_displacement")
		return HP_SELECTION_BAR_Y_DISPLACEMENT;
		
	return NO_NAME;
}

SDL_Rect j1Gui::GetUnitIcon(Unit * unit)
{
	switch (unit->GetUnitType())
	{
		//TODO: Get All atlas icons pos
	case U_TWOHANDEDSWORDMAN:
		return data.twohanded_icon;
		break;
	case U_ARCHER:
		return data.archer_icon;
		break;

	default:
		return { 774, 962, 25, 25 };// contorno { 1092, 827, 29, 29 };
		break;
	}
	return { 0,0,0,0 };
}

char * j1Gui::GetUnitName(Unit * unit)
{
	switch (unit->GetUnitType())
	{
		//TODO: Get all units names
	case U_TWOHANDEDSWORDMAN:
		return "two handed sword man";
		break;
	case U_ARCHER:
		return "archer";
		break;

	default:
		return "name error";
		break;
	}
	return "name error";
}

/*
void j1Gui::ShowBuildingArmor(Building * building)
{
	CreateImage(data.armor_icon_pos, data.armor_icon);
	attribute_value_armor = std::to_string(building->GetArmor());
	iPoint attribute_pos = data.armor_icon_pos;
	attribute_pos.x += data.attributes_displacement;
	CreateLabel(attribute_pos, BACKGROUND_RECT_DEFAULT_TEXT, (char*)attribute_value_armor.c_str());
}
*/
// class Gui ---------------------------------------------------

UI_Element::UI_Element(UI_ELEMENT_TYPE type, iPoint pos, SDL_Rect atlas_rect, bool not_in_world) : element_type(type), pos(pos), atlas_rect(atlas_rect), not_in_world(not_in_world)
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

UI_ELEMENT_TYPE UI_Element::GetElementType()
{
	return element_type;
}

const int UI_Element::GetX()
{
	return pos.x;
}

const int UI_Element::GetY()
{
	return pos.y;
}

void UI_Element::SetX(int x)
{
	pos.x = x;
}

void UI_Element::SetY(int y)
{
	pos.y = y;
}

void UI_Element::TurnOn()
{
	showing = true;
}

void UI_Element::TurnOff()
{
	showing = false;
}

bool UI_Element::IsOn()
{
	return showing;
}

const SDL_Rect UI_Element::GetAtlasRect()
{
	return atlas_rect;
}
