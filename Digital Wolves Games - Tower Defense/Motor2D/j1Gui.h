#ifndef __j1GUI_H__
#define __j1GUI_H__

#include <vector>
#include "SDL\include\SDL.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2
#define BACKGROUND_RECT_DEFAULT_TEXT { 359, 1579, 200, 200 }
#define NO_RECT { 0, 0, 0, 0 }
#define TEXT_RIGHT_DISPLACEMENT 5 //pixels

	 //----------------||------------------||----------------\\
	//----------------||         UI         ||----------------\\
   //----------------||______________________||----------------\\

class UI_Image;
class UI_Text;
class UI_TextInput;
class UI_Label;
class UI_MultiLabel;
class UI_Button;
class UI_AppearingLabel;
class UI_AppearingImage;
class UI_HPBar;
class UI_PanelInfoSingleEntity;
class UI_PanelInfoMultipleEntities;

class Entity;
class Building;
class Unit;

enum GUI_DATA_NAME	//except rects in atlas
{
	NO_NAME,
	PANEL_INFO_POS,
	SELECTED_ICON_START_POS,
	ATTACK_ICON_POS,
	ARMOR_ICON_POS,
	RANGE_ICON_POS,
	ATTRIBUTES_TEXT_DISPLACEMENT,
	HP_SELECTION_BAR_Y_DISPLACEMENT,
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
		iPoint panel_info_pos;
		iPoint attack_icon_pos;
		iPoint armor_icon_pos;
		iPoint range_icon_pos;
		//-------------------

		//Atlas rectangles-----
		SDL_Rect panel_background_rect;
		SDL_Rect attack_icon;
		SDL_Rect armor_icon;
		SDL_Rect range_icon;
		
		SDL_Rect green_hp_bar_rect;
		SDL_Rect red_hp_bar_rect;
		//---------------------

		//Others--------------------
		uint attributes_displacement = 40;			//pixels
		uint hp_bar_selection_Y_displacement = 26;	//pixels
		//--------------------------
	//-----------------------------------------
};

enum UI_ELEMENT_TYPE
{
	UI_E_UNKNOWN = 0,
	UI_E_IMAGE,
	UI_E_BUTTON,
	UI_E_LABEL,
	UI_E_MULTILABEL,
	UI_E_APPEARING_ELEMENT,
	UI_E_TEXT_INPUT,
	UI_E_HP_BAR,
	UI_E_PANEL_INFO_SINGLE,
	UI_E_PANEL_INFO_MULTIPLE,
};

class UI_Element
{
protected:
	UI_Element(UI_ELEMENT_TYPE type, iPoint pos, SDL_Rect atlas_rect, bool not_in_world = true);
	~UI_Element();

	iPoint pos;
	SDL_Rect atlas_rect;
	UI_ELEMENT_TYPE element_type = UI_E_UNKNOWN;
	bool not_in_world = true;
	bool showing = true;



public:
	virtual bool Update();
	virtual bool Draw(SDL_Texture* atlas);
	//bool HandleInput()

	UI_ELEMENT_TYPE GetElementType();
	const int GetX();
	const int GetY();
	void SetX(int x);
	void SetY(int y);
	void TurnOn();
	void TurnOff();
	bool IsOn();
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
	UI_Label* CreateLabel(iPoint pos, SDL_Rect atlas_rect, char* txt, bool has_background = true, bool not_in_world = true);
	UI_MultiLabel* CreateMultiLabel(iPoint pos, SDL_Rect atlas_rect, char* txt, int max_labels = 1, int label_to_remove = 0, bool has_max_labels = false, bool has_background = true, bool not_in_world = true);
	UI_AppearingLabel* CreateAppearingLabel(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, char* txt, bool has_background = true, bool not_in_world = true);
	UI_AppearingImage* CreateAppearingImage(iPoint pos, SDL_Rect atlas_rect, float seconds_on_screen, bool not_in_world = true);
	UI_TextInput* CreateTextInput(iPoint pos, char* txt, FONT_NAME font_name = OPENSANS_REGULAR, SDL_Color color = { (0), (0), (0), (255) }, bool not_in_world = true);
	UI_PanelInfoSingleEntity* CreatePanel(Entity* selection);
	UI_PanelInfoMultipleEntities* CreatePanel(std::vector<Entity*> selection);
	//-----------------------
	// Gui creation functions
	void DeleteImage(UI_Image* img);
	void DeleteButton(UI_Button* button);
	void DeleteLabel(UI_Label* label);
	void DeleteMultiLabel(UI_MultiLabel* multilabel);
	void DeleteAppearingLabel(UI_AppearingLabel* appearing_label);
	void DeleteAppearingImage(UI_AppearingImage* appearing_image);
	void DeleteTextInput(UI_TextInput* txt_input);
	void DeletePanelInfo(UI_PanelInfoSingleEntity* panel_info);
	void DeletePanelInfo(UI_PanelInfoMultipleEntities* panel_info);
	//-----------------------


	const SDL_Texture* GetAtlas() const;

	void SetFocusedText(UI_TextInput* text);
	UI_TextInput* GetFocusedText();

	GUI_Information data;
	SDL_Rect GetUnitIcon(Unit* unit);
	char* GetUnitName(Unit* unit);

private:
	SDL_Texture* atlas;
	std::string atlas_file_name;

	std::vector<UI_Element*> ui_elements;
	UI_TextInput* focused_text = nullptr;

	GUI_DATA_NAME DataStr2Enum(const std::string name);


	void ShowBuildingArmor(Building* building);
};

//	\\----------------||______________________||----------------//
//	  \\----------------||__________________||----------------//
//		\\----------------||--------------||----------------//

#endif // __j1GUI_H__