#include "j1App.h"
#include "Intro.h"
#include "j1Video.h"
#include "j1Audio.h"
#include "j1SceneManager.h"
#include "j1Input.h"
#include "j1MainMenu.h"

j1Intro::j1Intro() : j1Module()
{
	name.assign("intro");
}

// Destructor
j1Intro::~j1Intro()
{}

// Called before render is available
bool j1Intro::Awake()
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Intro::Start()
{
	App->video->Initialize("intro_anim.avi");
	App->audio->PlayMusic("audio/music/logo_anim_audio.ogg", 0.0f);
	time.Start();
	return true;
}

// Called each loop iteration
bool j1Intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Intro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->scene_manager->ChangeScene(SC_MAIN_MENU);
	}
	if (time.ReadSec() < 5.2)
	{
		App->video->GrabAVIFrame();
	}
	if (time.ReadSec() >= 5.2)
	{
		App->scene_manager->ChangeScene(SC_MAIN_MENU);
	}
	//-------
	return true;
}

// Called each loop iteration
bool j1Intro::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Intro::CleanUp()
{
	App->video->CloseAVI();
	return true;
}

