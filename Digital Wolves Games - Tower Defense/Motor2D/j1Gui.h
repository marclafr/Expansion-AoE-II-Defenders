#ifndef __j1GUI_H__
#define __j1GUI_H__

#include <vector>
#include "SDL\include\SDL.h"
#include "j1Module.h"
#include "p2Point.h"

#define CURSOR_WIDTH 2
#define BACKGROUND_RECT_DEFAULT_TEXT { 400, 1750, 0, 0 }

	 //----------------||------------------||----------------\\
	//----------------||         UI         ||----------------\\
   //----------------||______________________||----------------\\

class UI_Image;
class UI_Text;
class UI_Label;
class UI_Button;
class UI_AppearingLabel;
class UI_HPBar;

class Entity;
class Building;
class Unit;

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
	//-----------------------

	void CreatePanel(std::vector<Entity*> selection);

	const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	std::string atlas_file_name;

	std::vector<UI_Element*> ui_elements;


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