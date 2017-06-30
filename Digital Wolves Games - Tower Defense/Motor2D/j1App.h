#ifndef __j1APP_H__
#define __j1APP_H__

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"
#include <list>

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1FileSystem;
class j1Map;
class j1PathFinding;
class j1Fonts;
class j1Animation;
class j1UIManager;
class j1EntityManager;
class ProjectileManager;
class j1Collision;
class j1SceneManager;
class j1WaveManager;
class j1Console;
class j1Investigations;
class j1Score;
class j1Achievements;
class j1Tutorial;
class j1Video;
class j1Mouse;
class j1Gui;

	//Scenes
class j1Intro;
class j1MainMenu;
class j1Scene;
class j1ScoreScene;


struct DebugFeatures
{
public:
	DebugFeatures()
	{}

	~DebugFeatures(){}

	void UpdateDebug();

public:
	bool debug_mode = false;
	bool print_walkability_map = false;
	bool add_food = false;
	bool add_wood = false;
	bool add_stone = false;
	bool add_gold = false;
	bool pf = false;
	bool unlock_investigations = false;
	bool qt_entities = false;
	bool qt_area = false;
	bool tiles = false;
	bool qt_vertex = false;
	bool qt_diagonals = false;
	bool qt_center = false;
};

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module, bool active = true);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;

	void LoadGame(const char* file);
	bool LoadGameFromMenu(const char* file);
	void FinishGame(const char* file);
	void SaveAchievements(const char* file);
	void LoadAchievements(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(std::list<std::string>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1FileSystem*		fs = nullptr;
	j1Map*				map = nullptr;
	j1PathFinding*		pathfinding = nullptr;
	j1Fonts*			font = nullptr;
	j1Animation*		anim = nullptr;
	j1UIManager*		uimanager = nullptr;
	j1EntityManager*    entity_manager = nullptr;
	ProjectileManager*  projectile_manager = nullptr;
	j1Collision*		collision = nullptr;
	j1SceneManager*		scene_manager = nullptr;
	j1WaveManager*		wave_manager = nullptr;
	j1Console*			console = nullptr;
	j1Investigations*	investigations = nullptr;
	j1Score*			score = nullptr;
	j1Achievements*		achievements = nullptr;
	j1Tutorial*			tutorial = nullptr;
	j1Video*			video = nullptr;
	j1Mouse*			mouse = nullptr;
	j1Gui*				gui = nullptr;

		//Scenesko
	j1Intro*			intro = nullptr;
	j1MainMenu*			main_menu = nullptr;
	j1Scene*			scene = nullptr;
	j1ScoreScene*		score_scene = nullptr;

	DebugFeatures		debug_features;
	std::string			load_game;
	bool GonnaLoadScene = false;
private:

	std::list<j1Module*> modules;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	
	mutable std::string	save_game;
	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1PerfTimer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				dt = 0.0f;
	int					capped_ms = -1;
};

extern j1App* App;

#endif