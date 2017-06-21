/*#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1ScoreScene.h"
#include "j1EntityManager.h"
#include "j1WaveManager.h"
#include "j1Tutorial.h"
#include "j1SceneManager.h"
#include "Task.h"

//#include "UIButton.h"
//#include "UILabel.h"
//#include "j1UIManager.h"

j1Tutorial::j1Tutorial() : j1Module()
{
	name.assign("tutorial");
}

// Destructor
j1Tutorial::~j1Tutorial()
{}

// Called before render is available
bool j1Tutorial::Awake()
{
	LOG("Loading Tutorial");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Tutorial::Start()
{

	return true;
}

// Called each loop iteration
bool j1Tutorial::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Tutorial::Update(float dt)
{
	switch (tutorial_num)
	{
	case TUTORIAL_1:
		Tutorial1();
		break;

	case TUTORIAL_2:
		Tutorial2();
		break;

	case TUTORIAL_3:
		Tutorial3();
		break;

	case TUTORIAL_4:
		Tutorial4();
		break;

	case TUTORIAL_5:
		Tutorial5();
		break;

	case TUTORIAL_6:
		Tutorial6();
		break;

	case TUTORIAL_7:
		Tutorial7();
		break;

	}

	TutorialsProgression();

	if (tutorial1_part1 && tutorial1_part2 && tutorial1_part3)
	{
		tutorial_num = TUTORIAL_2;
		tutorial1_part1, tutorial1_part2, tutorial1_part3 = false;
	}

	if (tutorial3_part1 && tutorial3_part2 && tutorial3_part3 && tutorial3_part4)
	{
		tutorial_num = TUTORIAL_4;
		tutorial3_part1, tutorial3_part2, tutorial3_part3, tutorial3_part4 = false;
	}

	if (tutorial4_part2 && tutorial4_part3)
	{
		tutorial_num = TUTORIAL_5;
		tutorial4_part2, tutorial4_part3 = false;
	}

	if (tutorial5_part2 && tutorial5_part3 && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		tutorial_num = TUTORIAL_6;
		tutorial5_part2, tutorial5_part3 = false;
	}

	if (tutorial6_part2 && tutorial6_part3)
	{
		tutorial_num = TUTORIAL_7;
		tutorial6_part2, tutorial6_part3 = false;
	}

	return true;
}

// Called each loop iteration
bool j1Tutorial::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Tutorial::CleanUp()
{
	LOG("Freeing  MainMenu");
	//App->uimanager->SetAllToDelete();
	tutorial = false;
	tutorial1_deleted = false;
	tutorial2_deleted = false;
	tutorial3_deleted = false;
	tutorial4_deleted = false;
	tutorial5_deleted = false;
	tutorial6_deleted = false;
	tutorial7_deleted = false;
	return true;
}


void j1Tutorial::TutorialsProgression()
{
	if (tutorial_num == TUTORIAL_2 && !tutorial1_deleted)
	{
		tutorial1->SetToDelete();
		text_tutorial1->SetToDelete();
		text1_tutorial1->SetToDelete();
		text2_tutorial1->SetToDelete();
		text3_tutorial1->SetToDelete();
		tutorial1_deleted = true;
	}

	if (tutorial_num == TUTORIAL_3 && !tutorial2_deleted)
	{
		tutorial2->SetToDelete();
		text_tutorial2->SetToDelete();
		text1_tutorial2->SetToDelete();
		text2_tutorial2->SetToDelete();
		tutorial2_deleted = true;
	}

	if (tutorial_num == TUTORIAL_4 && !tutorial3_deleted)
	{
		tutorial3->SetToDelete();
		text_tutorial3->SetToDelete();
		text1_tutorial3->SetToDelete();
		text2_tutorial3->SetToDelete();
		text3_tutorial3->SetToDelete();
		text4_tutorial3->SetToDelete();
		tutorial3_deleted = true;
	}

	if (tutorial_num == TUTORIAL_5 && !tutorial4_deleted)
	{
		tutorial4->SetToDelete();
		text_tutorial4->SetToDelete();
		text1_tutorial4->SetToDelete();
		text2_tutorial4->SetToDelete();
		text3_tutorial4->SetToDelete();
		text4_tutorial4->SetToDelete();
		tutorial4_deleted = true;
	}

	if (tutorial_num == TUTORIAL_6 && !tutorial5_deleted)
	{
		tutorial5->SetToDelete();
		text_tutorial5->SetToDelete();
		text1_tutorial5->SetToDelete();
		text2_tutorial5->SetToDelete();
		text3_tutorial5->SetToDelete();
		tutorial5_deleted = true;
	}

	if (tutorial_num == TUTORIAL_7 && !tutorial6_deleted)
	{
		tutorial6->SetToDelete();
		text_tutorial6->SetToDelete();
		text1_tutorial6->SetToDelete();
		text2_tutorial6->SetToDelete();
		text3_tutorial6->SetToDelete();
		tutorial6_deleted = true;
	}

	if (tutorial_num == TUTORIAL_8 && !tutorial7_deleted)
	{
		tutorial7->SetToDelete();
		text_tutorial7->SetToDelete();
		text1_tutorial7->SetToDelete();
		tutorial7_deleted = true;
	}
}

void j1Tutorial::TutorialReset()
{
	tutorial_num = TUTORIAL_1;
	tutorial1_part1 = false;
	tutorial1_part2 = false;
	tutorial1_part3 = false;
	tutorial2_part1 = false;
	TownHallSelected = false;
	PanelSelected = false;
	tutorial3_part1 = false;
	tutorial3_part2 = false;
	tutorial3_part3 = false;
	tutorial3_part4 = false;
	tutorial4_part1 = false;
	tutorial4_part2 = false;
	tutorial4_part3 = false;
	UniversitySelected = false;
	InvestigationSelected = false;
	InvestigationDone = false;
	tutorial5_part1 = false;
	tutorial5_part2 = false;
	tutorial5_part3 = false;
	TowerSelected = false;
	TowerUpgradeSelected = false;
	tutorial6_part1 = false;
	tutorial6_part2 = false;
	tutorial6_part3 = false;
	NextWaveButtonSelected = false;
}


void j1Tutorial::Tutorial1()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		tutorial1_part1 = true;
		text1_tutorial1->ChangeText("Press 1 to build a Simple Tower Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		tutorial1_part2 = true;
		text2_tutorial1->ChangeText("Press 2 to build a Bombard Tower Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		tutorial1_part3 = true;
		text3_tutorial1->ChangeText("Press 3 to build a Wall Done");
	}
}

void j1Tutorial::Tutorial2()
{
	if (TownHallSelected) {
		tutorial2_part1 = true;
		text_tutorial2->ChangeText("Click on the Town Hall Done");
	}

	if (PanelSelected) {
		tutorial_num = TUTORIAL_3;
	}
}

void j1Tutorial::Tutorial3()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		tutorial3_part1 = true;
		text1_tutorial3->ChangeText("W Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		tutorial3_part2 = true;
		text2_tutorial3->ChangeText("A Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		tutorial3_part3 = true;
		text3_tutorial3->ChangeText("S Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		tutorial3_part4 = true;
		text4_tutorial3->ChangeText("D Done");
	}
}

void j1Tutorial::Tutorial4()
{
	if (!tutorial4_part1)
	{
		App->render->camera->SetPosition(iPoint(-700, -500));
		tutorial4_part1 = true;
	}
	if (UniversitySelected) {
		tutorial4_part2 = true;
		text1_tutorial4->ChangeText("Select the University Done");
	}

	if (InvestigationDone)
	{
		tutorial4_part3 = true;
		text2_tutorial4->ChangeText("Click the fire upgrade on the panel Done");
	}
}

void j1Tutorial::Tutorial5()
{
	if (!tutorial5_part1)
	{
		App->render->camera->SetPosition(iPoint(700, 20));
		tutorial5_part1 = true;
	}

	if (TowerSelected) {
		tutorial5_part2 = true;
		text_tutorial5->ChangeText("Click one tower Done");
	}
	if (TowerUpgradeSelected)
	{
		tutorial5_part3 = true;
		text1_tutorial5->ChangeText("Click the fire upgrade icon Done");
	}
}

void j1Tutorial::Tutorial6()
{
	//App->wave_manager->Enable(); 
	tutorial = false;
	if (!tutorial6_part1)
	{
		App->render->camera->SetPosition(iPoint(1700, -2400));
		tutorial6_part1 = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		tutorial6_part2 = true;
		text1_tutorial6->ChangeText("Build a tower on the highground Done");
	}
	if (NextWaveButtonSelected)
	{
		tutorial6_part3 = true;
		text3_tutorial6->ChangeText("Click next wave button (big red button) Done");
	}
}

void j1Tutorial::Tutorial7()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		tutorial_num = TUTORIAL_8;
	}
}*/