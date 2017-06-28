#ifndef __j1GUI_H__
#define __j1GUI_H__

#include <vector>
#include "SDL\include\SDL.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2
#define BACKGROUND_RECT_DEFAULT_TEXT { 400, 1750, 0, 0 }
#define TEXT_RIGHT_DISPLACEMENT 5 //pixels

	 //----------------||------------------||----------------\\
	//----------------||         UI         ||----------------\\
   //----------------||______________________||----------------\\

class UI_Image;
class UI_Text;
class UI_TextInput;
class UI_Label;
class UI_Button;
class UI_AppearingLabel;
class UI_HPBar;

class Entity;
class Building;
class Unit;

enum GUI_DATA_NAME	//except rects in atlas
{
	NO_NAME,
	SELECTED_ICON_START_POS,
	ATTACK_ICON_POS,
	ARMOR_ICON_POS,
	RANGE_ICON_POS,
	ATTRIBUTES_TEXT_DISPLACEMENT,
	SPACE_BETWEEN_SELECTED_ICONS,
	MAX_ICONS_IN_ROW
};

struct GUI_Information
{
	GUI_Information(){}
	~GUI_Information(){}

	//Selection
	iPoint selection_start_pos;
	uint max_icons_in_row = 10;
	uint space_between_selected_icons = 30; //pixels
	//------

	//Icons
		//Units--------------
		SDL_Rect twohanded_icon;
		SDL_Rect archer_icon;
		//-------------------

		//Positions----------
		iPoint attack_icon_pos;
		iPoint armor_icon_pos;
		iPoint range_icon_pos;
		//-------------------

		//Atlas rectangles-----
		SDL_Rect attack_icon;
		SDL_Rect armor_icon;
		SDL_Rect range_icon;
		//---------------------

		//Others--------------------
		uint attributes_displacement = 40; //pixels
		//--------------------------
	//-----------------------------------------
};

class UI_Element
{
protected:
	UI_Element(iPoint pos, SDL_Rect atlas_rect, bool not_in_world = true);
	~UI_Element();

	iPoint pos;
	SDL_Rect atlas_rect;

	bool not_in_world = true;

public:
	virtual bool Update();
	virtual bool Draw(SDL_Texture* atlas);
	//bool HandleInput()

	const int GetX();
	const int GetY();
	const SDL_Rect GetAtlasRect();
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	UI_Image* CreateImage(iPoint pos, SDL_Rect atlas_rect, bool not_in_world = true);
	UI_Button* CreateButton(iPoint pos, SDL_Rect atlas_rect_idle, SDL_Rect atlas_rect_mouse_on_top, SDL_Rect atlas_rect_clicking, char* description = nullptr, bool not_in_world = true, SDL_Rect description_background_rect = BACKGROUND_RECT_DEFAULT_TEXT);
	UI_Label* CreateLabel(iPoint pos, SDL_Rect atlas_rect, char* txt, bool not_in_world = true);
	UI_TextInput* CreateTextInput(iPoint pos, char* txt, FONT_NAME font_name = OPENSANS_REGULAR, SDL_Color color = { (255), (255), (255), (255) }, bool not_in_world = true);
	//-----------------------

	void CreatePanel(std::vector<Entity*> selection);

	const SDL_Texture* GetAtlas() const;

	void SetFocusedText(UI_TextInput* text);
	UI_TextInput* GetFocusedText();

private:

	SDL_Texture* atlas;
	std::string atlas_file_name;

	std::vector<UI_Element*> ui_elements;
	UI_TextInput* focused_text = nullptr;

	GUI_Information data;
	GUI_DATA_NAME DataStr2Enum(const std::string name);
	SDL_Rect GetUnitIcon(Unit* unit);
	std::string attribute_value_attack;
	std::string attribute_value_armor;
	std::string attribute_value_range;

	void ShowBuildingArmor(Building* building);
};

//	\\----------------||______________________||----------------//
//	  \\----------------||__________________||----------------//
//		\\----------------||--------------||----------------//

#endif // __j1GUI_H__