#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1FileSystem.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Fonts.h"
#include "j1Animation.h"
//#include "j1UIManager.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "j1MainMenu.h"
#include "j1ScoreScene.h"
#include "j1Video.h"
#include "j1App.h"
#include "j1SceneManager.h"
#include "j1Console.h"
#include "j1WaveManager.h"
#include "ProjectileManager.h"
#include "j1Investigations.h"
#include "j1Score.h"
#include "j1Achievements.h"
#include "Intro.h"
#include "j1Tutorial.h"
#include "j1Mouse.h"
#include "j1Gui.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	fs = new j1FileSystem();
	pathfinding = new j1PathFinding();
	entity_manager = new j1EntityManager();
	projectile_manager = new ProjectileManager();
	font = new j1Fonts();
	anim = new j1Animation();
	map = new j1Map();
	//uimanager = new j1UIManager();
	main_menu = new j1MainMenu();
	scene = new j1Scene();
	score_scene = new j1ScoreScene();
	//collision = new j1Collision();
	scene_manager = new j1SceneManager();
	//wave_manager = new j1WaveManager();
	console = new j1Console();
	//investigations = new j1Investigations();
	//score = new j1Score();
	achievements = new j1Achievements();
	//tutorial = new j1Tutorial();
	video = new j1Video();
	intro = new j1Intro();
	mouse = new j1Mouse();
	gui = new j1Gui();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp

	//Active at start
	AddModule(fs);
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(font);
	//AddModule(uimanager);
	AddModule(console);
	AddModule(scene_manager);
	AddModule(video);
	AddModule(mouse);
	//Not-Active at start
	AddModule(pathfinding, false);
	AddModule(map, false);
	AddModule(anim, false);
	//AddModule(collision, false);
	AddModule(entity_manager, false);
	AddModule(projectile_manager, false);
	//AddModule(wave_manager, false);
	//AddModule(investigations, false);
	//AddModule(score, false);
	AddModule(achievements, false);
	//AddModule(tutorial, false);
	AddModule(gui, true);

	// Scenes	
	AddModule(intro);
	AddModule(main_menu,false);
	AddModule(scene, false);
	AddModule(score_scene, false);	

	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	// release modules
	std::list<j1Module*>::reverse_iterator item = modules.rbegin();

	while (item != modules.rend())
	{
		RELEASE(*item);
		item++;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module, bool active)
{
	if (active)
		module->Init();

	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = true;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if (cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}

	if (ret == true)
	{
		std::list<j1Module*>::iterator item = modules.begin();

		while (item != modules.end() && ret == true)
		{
			ret = (*item)->Awake(config.child((*item)->name.c_str()));
			item++;
		}
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	std::list<j1Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		if(item._Ptr->_Myval->active == true)
		ret = (*item)->Start();
		item++;
	}
	startup_time.Start();
	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if (want_to_save == true)
		SavegameNow();

	if (want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / (startup_time.ReadMs()/1000);
	float seconds_since_startup = startup_time.ReadMs();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera->GetPosition().x, y - App->render->camera->GetPosition().y); // = App->map->WorldToMap(x - App->render->camera->GetPosition().x, y - App->render->camera->GetPosition().y);

	static char mouse_pos[256];
	sprintf_s(mouse_pos, 256, "Mouse Pos: %d,%d", x, y);
	App->console->PushText(mouse_pos);

	static char map[256];
	sprintf_s(map, 256, "Map:%dx%d", App->map->data.width, App->map->data.height);
	App->console->PushText(map);

	static char tiles[256];
	sprintf_s(tiles, 256, "Tiles:%dx%d", App->map->data.tile_width, App->map->data.tile_height);
	App->console->PushText(tiles);

	static char tilesets[256];
	sprintf_s(tilesets, 256, "Tilesets:%d", App->map->data.tilesets.size());
	App->console->PushText(tilesets);

	static char tile[256];
	sprintf_s(tile, 256, "Tile:%d,%d", map_coordinates.x, map_coordinates.y);
	App->console->PushText(tile);

	static char current_frame[256];
	sprintf_s(current_frame, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f", avg_fps, last_frame_ms, frames_on_last_update, dt);
	App->console->PushText(current_frame);

	static char scince_startup[256];
	sprintf_s(scince_startup, 256, "Time since startup: %.3f Frame Count: %lu ", seconds_since_startup, frame_count);
	App->console->PushText(scince_startup);

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		j1PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator item;
	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator item;
	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if (pModule->active == false)
			continue;

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	std::list<j1Module*>::iterator item;
	j1Module* pModule = NULL;

	for (item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	std::list<j1Module*>::reverse_iterator item = modules.rbegin();

	while (item != modules.rend() && ret == true)
	{
		if((*item)->active)
			ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
float j1App::GetDT() const
{
	return dt;
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.c_str();
}

// Load / Save
void j1App::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
	char dir[512];
	sprintf_s(dir, "%s%s", fs->GetSaveDirectory(), file);
	load_game.assign(dir);
}

bool j1App::LoadGameFromMenu(const char * file)
{
	char dir[512];
	sprintf_s(dir, "%s%s", fs->GetSaveDirectory(), file);
	load_game.assign(dir);

	bool ret = false;

	char* buffer;
	uint size = fs->Load(load_game.c_str(), &buffer);

	if (size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		root = data.child("game_state");

		if (root.child("entity_manager").child("Saved_Game").attribute("saved_game").as_int() == 1)
		{
			data.reset();
			return true;
		}
		else 
		{
			data.reset();
			return false;
		}
	}
}

void j1App::FinishGame(const char * file)
{
	save_game.assign(file);

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	root.append_child("entity_manager").append_child("Saved_Game").append_attribute("saved_game") = 0;

	std::stringstream stream;
	data.save(stream);

		// we are done, so write data to disk
	fs->Save(save_game.c_str(), stream.str().c_str(), stream.str().length());
	data.reset();

}

void j1App::SaveAchievements(const char * file)
{
	save_game.assign(file);

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	pugi::xml_node Achievements = root.append_child("Achievements");

	if (App->scene->townhall->GetHp() == 1500)
	{
		Achievements.append_attribute("townhall_life") = 1;
	}
	else
	{
		Achievements.append_attribute("townhall_life") = 0;
	}

	if (App->score_scene->units_count >= 100)
	{
		Achievements.append_attribute("units_spawneds") = 1;
	}
	else
	{
		Achievements.append_attribute("units_spawneds") = 0;
	}

	if (App->score_scene->build_simple_tower == true)
	{
		Achievements.append_attribute("only_bombs") = 1;
	}
	else
	{
		Achievements.append_attribute("only_bombs") = 0;
	}

	/*if (App->score->GetScore() >= 100000)
	{
		Achievements.append_attribute("points") = 1;
	}
	else
	{
		Achievements.append_attribute("points") = 0;
	}*/

	std::stringstream stream;
	data.save(stream);

	// we are done, so write data to disk
	fs->Save(save_game.c_str(), stream.str().c_str(), stream.str().length());

	data.reset();
}

void j1App::LoadAchievements(const char * file)
{
	char dir[512];
	sprintf_s(dir, "%s%s", fs->GetSaveDirectory(), file);
	load_game.assign(dir);

	bool ret = false;

	char* buffer;
	uint size = fs->Load(load_game.c_str(), &buffer);

	if (size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		root = data.child("game_state");

		pugi::xml_node Achievements = root.append_child("Achievements");

		if (Achievements.append_attribute("townhall_life").as_int() == 1)
		{
			App->score_scene->townlifeAchievement = true;
		}
		else 
		{
			App->score_scene->townlifeAchievement = false;
		}
	

		if (Achievements.append_attribute("units_spawneds").as_int() == 1)
		{
			App->score_scene->SpawnedunitsAchievement = true;
		}
		else 
		{
			App->score_scene->SpawnedunitsAchievement = false;
		}

		if (Achievements.append_attribute("only_bombs").as_int() == 1)
		{
			App->score_scene->onlybombsAchievement = true;
		}
		else
		{
			App->score_scene->onlybombsAchievement = true;
		}

		if (Achievements.append_attribute("points").as_int() == 1)
		{
			App->score_scene->pointsAchievement = true;
		}
		else
		{
			App->score_scene->pointsAchievement = false;
		}

		data.reset();
	}
}

// ---------------------------------------
void j1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.assign(file);
}

// ---------------------------------------
void j1App::GetSaveGames(std::list<std::string>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	bool ret = false;

	char* buffer;
	uint size = fs->Load(load_game.c_str(), &buffer);

	if (size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if (result != NULL)
		{
			root = data.child("game_state");

			std::list<j1Module*>::iterator item = modules.begin();
			ret = true;

			while (item != modules.end() && ret == true)
			{
				ret = (*item)->Load(root.child((*item)->name.c_str()));
				item++;
			}

			data.reset();
			if (ret == false)
				LOG("...loading process interrupted with error on module %s", (*item != NULL) ? (*item)->name.c_str() : "unknown");
		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", load_game.c_str(), result.description());
	}
	else
		LOG("Could not load game state xml file %s", load_game.c_str());

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	bool ret = true;

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	std::list<j1Module*>::const_iterator item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		ret = (*item)->Save(root.append_child((*item)->name.c_str()));
		item++;
	}

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		// we are done, so write data to disk
		fs->Save(save_game.c_str(), stream.str().c_str(), stream.str().length());
	}
	else
		LOG("Save process halted from an error in module %s", (*item != NULL) ? (*item)->name.c_str() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

void DebugFeatures::UpdateDebug()
{
	/*if (add_food)
	{
		App->scene->resources->AddFood(1000);
		App->debug_features.add_food = false;
	}
	if (add_wood)
	{
		App->scene->resources->AddWood(1000);
		App->debug_features.add_wood = false;
	}

	if (add_stone)
	{
		App->scene->resources->AddStone(1000);
		App->debug_features.add_stone = false;
	}

	if (add_gold)
	{
		App->scene->resources->AddGold(1000);
		App->debug_features.add_gold = false;
	}*/

	if (pf)
		App->pathfinding->Debug();

	/*
	if (unlock_investigations)
	{
		App->investigations->InstaUnlockAllInvestigations();
		unlock_investigations = false;
	}*/
}
