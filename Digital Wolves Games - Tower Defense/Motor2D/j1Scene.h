#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include <vector>
#include "j1Module.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Towers.h"

//tiles
#define TOWER_POS_1_X 4
#define TOWER_POS_1_Y 10
#define TOWER_POS_2_X 9
#define TOWER_POS_2_Y 6

struct SDL_Texture;
class Resources;
class Building;
enum TOWER_TYPE;
enum RESOURCE_TYPE;
enum BUILDING_TYPE;
class AnimationManager;
class UIComponents;
class UI_Element;

enum PlacingWallDirection 
{
	NORTH,
	SOUTH,
	WEST,
	EAST,
	NORTH_WEST,
	NORTH_EAST,
	SOUTH_WEST,
	SOUTH_EAST
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Enable Module with submodules
	void CreateSceneUI();

	/*void PlacingTower(TOWER_TYPE type);
	
	void PlacingWall();*/

	void HandleInput(SDL_Event event);

	//void TutorialUI();

	//const int GetTownHallHp();
	Building* townhall;

	const SDL_Rect& GetSelectionRect() const;

private:
	bool game_scene = true;

	j1Audio* scene_music;

	SDL_Rect select_rect;
 
	iPoint firstSelectedForWall;
	iPoint LastSelectedForWall;
	PlacingWallDirection direction;
	bool placing_wall_clicked = false;
	

	iPoint mouse_click_objective;
	AnimationManager* mouse_click_move_anim = nullptr;
	AnimationManager* fish_anim = nullptr;

public:
	bool win = false;
	bool lose = false;

	bool selecting = false;
	bool group_select = false;

	UIComponents* new_wave_exclamation;
	bool bring_next_wave = false;
	bool can_click_next_wave = false;
	j1Timer exclamation_timer;
	
	//Minimap, when click move the camera
	bool move_camera = false;
	iPoint camera_new_position;

	TOWER_TYPE placing_tower = T_NO_TYPE;
	bool placing_wall = false;

	//ResourceManager* resources = nullptr;

	std::vector<Entity*> selection;
	UI_Element* panel_info;
};

#endif // __j1SCENE_H__