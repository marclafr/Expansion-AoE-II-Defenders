#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Audio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.assign("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}
	LoadAllFX();
	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if (!active)
		return true;

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	std::list<Mix_Chunk*>::iterator item;
	for (item = fx.begin(); item != fx.end(); ++item)
		Mix_FreeChunk(*item);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS_RW(App->fs->Load(path), 1);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(App->fs->Load(path), 1);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if (!active)
		return false;

	if (id > 0 && id <= fx.size())
	{
		std::list<Mix_Chunk*>::iterator item = fx.begin();
		advance(item, id - 1);
		Mix_PlayChannel(-1, (*item), repeat);
	}

	return ret;
}

void j1Audio::LoadAllFX()
{
	fx_twohanded_die01 = App->audio->LoadFx("audio/fx/Male_Death01.wav");
	fx_twohanded_die02 = App->audio->LoadFx("audio/fx/Male_Death02.wav");
	fx_twohanded_die03 = App->audio->LoadFx("audio/fx/Male_Death03.wav");
	fx_twohanded_die04 = App->audio->LoadFx("audio/fx/Male_Death04.wav");
	fx_twohanded_die05 = App->audio->LoadFx("audio/fx/Male_Death05.wav");
	fx_attack01 = App->audio->LoadFx("audio/fx/Swordfight01.wav");
	fx_attack02 = App->audio->LoadFx("audio/fx/Swordfight02.wav");
	fx_attack03 = App->audio->LoadFx("audio/fx/Swordfight03.wav");

	//BUILDINGS
	fx_building_destroyed = App->audio->LoadFx("audio/fx/Building_destroyed01.wav");
	fx_arrow = App->audio->LoadFx("audio/fx/Arrow01.wav");
	fx_siegeram_hit = App->audio->LoadFx("audio/fx/Battering_Ram01.wav");
	fx_cannon = App->audio->LoadFx("audio/fx/Cannon01.wav");
	fx_construction = App->audio->LoadFx("audio/fx/Construction01.wav");

	//FX
	fx_click_btn = App->audio->LoadFx("audio/fx/Click.wav");
	fx_cancelclick_btn = App->audio->LoadFx("audio/fx/Error_Click.wav");
}
