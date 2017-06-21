#include "j1App.h"
#include "j1EntityManager.h"
#include "j1SceneManager.h"
#include "j1Scene.h"
#include "j1MainMenu.h"
#include "j1ScoreScene.h"
#include "Buildings.h"
#include "Towers.h"
#include "Units.h"
#include "p2Log.h"
#include "j1Tutorial.h"
#include "j1WaveManager.h"
/*#include "UIComponents.h"
#include "j1UIManager.h"
#include "UIHUDMenuInGame.h"*/
#include "j1Tutorial.h"

//#include "UIHUDPanelButtons.h"

#include "Task.h"

CreateTowerTask::CreateTowerTask() : type(B_NO_BUILDING), position(fPoint(0.0f, 0.0f)), side(S_NO_SIDE) {}

CreateTowerTask::CreateTowerTask(BUILDING_TYPE type, fPoint position, Side side) : type(type), position(position), side(side) {}

void CreateTowerTask::Set(BUILDING_TYPE type, fPoint position, Side side)
{
	this->type = type;
	this->position = position;
	this->side = side;
}

bool CreateTowerTask::Execute()
{
	//App->entity_manager->CreateBuilding(type, position, side);
	return true;
}

ChangeMainMenuSceneToTask::ChangeMainMenuSceneToTask(SCENES to_scene) : to_scene(to_scene) { }

bool ChangeMainMenuSceneToTask::Execute()
{
	App->scene_manager->ChangeScene(to_scene);

	if (to_scene == SC_GAME)
	{
		App->scene->win = false;
		App->scene->lose = false;
	}
	return true;
}

ChangeMainMenuSceneToTutorial::ChangeMainMenuSceneToTutorial(SCENES to_scene) : to_scene(to_scene) {}

bool ChangeMainMenuSceneToTutorial::Execute()
{
	App->scene_manager->ChangeScene(to_scene);

	if (to_scene == SC_GAME)
	{
		App->scene->win = false;
		App->scene->lose = false;
		//App->tutorial->tutorial = true;
	}
	return true;
}

bool SetPreUpdateFalseTask::Execute()
{
	App->main_menu->SetRetPreUpdate(false);
	return true;
}

ChangeScoreSceneToTask::ChangeScoreSceneToTask(SCENES to_scene) : to_scene(to_scene) {}

bool ChangeScoreSceneToTask::Execute()
{
	App->scene_manager->ChangeScene(to_scene);

	if (to_scene == SC_GAME)
	{
		App->score_scene->SetSceneChange(true);
		App->scene->win = false;
		App->scene->lose = false;
	}
	else if (SC_MAIN_MENU)
	{
		if (!App->score_scene->IsSceneChange())
		{
			App->scene->win = false;
			App->scene->lose = false;
		}
	}
	return true;
}

/*ChangeBackGroundTask::ChangeBackGroundTask(const SDL_Rect & atlas, const BUTTONSUNDERGROUND underground) : rect_atlas(atlas), underground(underground) {}

bool ChangeBackGroundTask::Execute()
{
	App->score_scene->GetUnderBackground()->SetAtlas(rect_atlas);
	App->score_scene->ChangeUnselected(underground);
	return true;
}*/

EntityTask::EntityTask(ENTITY_TASKTYPE type) : type(type) {}

const ENTITY_TASKTYPE EntityTask::GetEntityTaskType() const
{
	return type;
}

PlaceBasicTowerTask::PlaceBasicTowerTask(ENTITY_TASKTYPE type) : EntityTask(type) {}

bool PlaceBasicTowerTask::Execute()
{
	App->scene->placing_wall = false;
	App->scene->placing_tower = T_BASIC_TOWER;
	//if (App->tutorial->tutorial_num == TUTORIAL_2) App->tutorial->PanelSelected = true;
	return true;
}

PlaceBombardTowerTask::PlaceBombardTowerTask(ENTITY_TASKTYPE type) : EntityTask(type) {}

bool PlaceBombardTowerTask::Execute()
{
	App->scene->placing_wall = false;
	App->scene->placing_tower = T_BOMBARD_TOWER;
	//if (App->tutorial->tutorial_num == TUTORIAL_2) App->tutorial->PanelSelected = true;
	return true;
}

/*UpgradeTowerTask::UpgradeTowerTask(INVESTIGATION_TYPE type, ENTITY_TASKTYPE et_type) : EntityTask(et_type), type(type) {}

const INVESTIGATION_TYPE UpgradeTowerTask::GetUpgradeType() const
{
	return type;
}

void UpgradeTowerTask::SetTower(Tower * tower)
{
	this->tower = tower;
}

void UpgradeTowerTask::SetCanUpgrade(bool canU)
{
	canUpgrade = canU;
}


bool UpgradeTowerTask::Execute()
{
	if (canUpgrade && tower->IsAlive())
	{
		tower->UpgradeTurret(type);

		if (App->tutorial->tutorial_num == TUTORIAL_5)
		{
			App->tutorial->TowerUpgradeSelected = true;
		}

		App->entity_manager->UnselectEverything();
	}
	return true;
}

UpgradeWallTask::UpgradeWallTask(BUILDING_TYPE type, ENTITY_TASKTYPE et_type) : EntityTask(et_type), type(type) {}

const BUILDING_TYPE UpgradeWallTask::GetWallType() const
{
	return type;
}

void UpgradeWallTask::SetWall(Building * wall)
{
	this->wall = wall;
	type = wall->GetBuildingType();
}

bool UpgradeWallTask::Execute()
{
	if (wall->IsAlive())	wall->UpgradeWall(type);
	App->entity_manager->UnselectEverything();
	return true;
}

DoInvestigation::DoInvestigation(INVESTIGATION_TYPE type, ENTITY_TASKTYPE et_type) : EntityTask(et_type), type(type) {}

const INVESTIGATION_TYPE DoInvestigation::GetInvestigationType() const
{
	return type;
}

void DoInvestigation::SetCanInvestigate(bool canU)
{
	canInvestigate = canU;
}

bool DoInvestigation::Execute()
{
	if (canInvestigate)
	{
		App->investigations->WantToInvestigate(App->investigations->GetInvestigation(type));
		App->entity_manager->UnselectEverything();
	}
	return true;
}*/

DeleteTowerTask::DeleteTowerTask(ENTITY_TASKTYPE type) : EntityTask(type) {}

void DeleteTowerTask::SetTower(Tower * tower)
{
	this->tower = tower;
}

bool DeleteTowerTask::Execute()
{
	if (tower->IsAlive())	tower->SetHp(0);
	App->entity_manager->UnselectEverything();
	return true;
}

DeleteWallTask::DeleteWallTask(ENTITY_TASKTYPE type) : EntityTask(type) {}

void DeleteWallTask::SetWall(Building * wall)
{
	this->wall = wall;
}

bool DeleteWallTask::Execute()
{
	if (wall->IsAlive())	wall->ConvertToRubble();
	App->entity_manager->UnselectEverything();
	return true;
}

PlaceWallTask::PlaceWallTask(ENTITY_TASKTYPE type) : EntityTask(type) {}

bool PlaceWallTask::Execute()
{
	App->scene->placing_wall = true;
	App->scene->placing_tower = T_NO_TYPE;
	//if (App->tutorial->tutorial_num == TUTORIAL_2) App->tutorial->PanelSelected = true;
	return true;
}

/*TrainUnitTask::TrainUnitTask(UNIT_TYPE u_type, ENTITY_TASKTYPE type) : EntityTask(type), u_type(u_type) {}

bool TrainUnitTask::Execute()
{
	if (App->scene->resources->CanTrainSoldier(u_type))
	{
		App->scene->resources->TrainSoldier(u_type);
	}

	if (App->tutorial->tutorial_num == TUTORIAL_2) App->tutorial->PanelSelected = true;
	return true;
}

const UNIT_TYPE TrainUnitTask::GetUnitType() const
{
	return u_type;
}

ShowButtonsInside::ShowButtonsInside(ENTITY_TASKTYPE type) : EntityTask(type) {}

bool ShowButtonsInside::Execute()
{
	App->uimanager->GetPanelButtons()->WantReset(info_b, true);
	return true;
}

void ShowButtonsInside::SetInfoButton(info_button * info_b)
{
	this->info_b = info_b;
}

ExitButonsInside::ExitButonsInside(ENTITY_TASKTYPE type) : EntityTask(type) {}

bool ExitButonsInside::Execute()
{
	App->uimanager->GetPanelButtons()->WantReset(from_info_b, false);
	return true;
}

void ExitButonsInside::SetInfoButton(info_button * info_b)
{
	from_info_b = info_b;
}*/

bool ChangeToFacebook::Execute()
{
	ShellExecute(NULL, "open", "https://www.facebook.com/Digital-Wolves-Games-233798633695568/?ref=bookmarks", NULL, NULL, SW_SHOWMAXIMIZED);
	return true;
}

bool ChangeToTwitter::Execute()
{
	ShellExecute(NULL, "open", "https://twitter.com/DigitalWolvesG", NULL, NULL, SW_SHOWMAXIMIZED);
	return true;
}

bool ChangeToGithub::Execute()
{
	ShellExecute(NULL, "open", "https://github.com/marclafr/Digital-Wolves-Games/wiki", NULL, NULL, SW_SHOWMAXIMIZED);
	return true;
}

bool ChangeToBugs::Execute()
{
	ShellExecute(NULL, "open", "https://github.com/marclafr/Digital-Wolves-Games/issues", NULL, NULL, SW_SHOWMAXIMIZED);
	return true;
}
/*
bool BringNextWave::Execute()
{
	App->wave_manager->BringNextWave();
	if (App->tutorial->tutorial_num == TUTORIAL_6) App->tutorial->NextWaveButtonSelected = true;
	return true;
}*/

bool Surrender::Execute()
{
	App->scene->lose = true;
	App->scene_manager->ChangeScene(SC_SCORE);
	//App->tutorial->tutorial = false;
	App->FinishGame("save_game.xml");
	return true;
}

/*ReturnToGame::ReturnToGame(UIHUDMenuInGame * menu_ingame) : menu_ingame(menu_ingame) {}

bool ReturnToGame::Execute()
{
	menu_ingame->DeletePanel();
	return true;
}*/

bool LoadLastChackpoint::Execute()
{
	App->LoadGame("save_game.xml");
	return true;
}

bool InGameToMainMenuScene::Execute()
{
	App->scene_manager->ChangeScene(SC_MAIN_MENU);
	return true;
}

bool MainMenuLoadGame::Execute()
{
	if (App->LoadGameFromMenu("save_game.xml") == true)
	{
		App->GonnaLoadScene = true;
		App->scene_manager->ChangeScene(SC_GAME);
	}
	else
	{
		App->GonnaLoadScene = false;
		return false;
	}
}
