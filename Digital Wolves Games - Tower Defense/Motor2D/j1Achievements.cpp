#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Achievements.h"
#include "Task.h"

/*#include "UIButton.h"
#include "UILabel.h"
#include "j1UIManager.h"*/

j1Achievements::j1Achievements() : j1Module()
{
	name.assign("achievements");
}

// Destructor
j1Achievements::~j1Achievements()
{}

// Called before render is available
bool j1Achievements::Awake()
{
	LOG("Loading Achievements");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Achievements::Start()
{

	return true;
}

// Called each loop iteration
bool j1Achievements::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Achievements::Update(float dt)
{
	App->render->BlitMainMenu();
	return true;
}

// Called each loop iteration
bool j1Achievements::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Achievements::CleanUp()
{
	LOG("Freeing  MainMenu");
	//App->uimanager->SetAllToDelete();
	return true;
}

// Set Achievements
void j1Achievements::SetAchievements(int num, bool achi)
{
	switch (num)
	{
	case 1:
		achievement1 = achi;;
		break;
	}
}

// Get Achievements
bool j1Achievements::GetAchievements(int num)
{
	switch (num)
	{
	case 1:
		return achievement1;
		break;
	}

	return false;
}

void j1Achievements::AchievementComplete()
{
	
}