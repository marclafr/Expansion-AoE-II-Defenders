#include "p2Log.h"
#include "j1Investigations.h"
#include "j1App.h"
#include "Resources.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Tutorial.h"

j1Investigations::j1Investigations()
{
	name.assign("investigations");
}

j1Investigations::~j1Investigations()
{}

bool j1Investigations::Start()
{
	bool ret = true;

	//SOUNDS
	fx_inv_completed = App->audio->LoadFx("audio/fx/Investigation.wav");
	//--

	//RESOURCES
	CreateInvestigation(INV_GOLD, true, 150, 5.0);

	//TROPS
	CreateInvestigation(INV_CAVALRY_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_CAVALRY_DEFENSE, false, 300, 25.0);
	CreateInvestigation(INV_ARCHERS_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_ARCHERS_DEFENSE, false, 300, 25.0);
	CreateInvestigation(INV_INFANTRY_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_INFANTRY_DEFENSE, false, 300, 25.0);

	//TOWERS
	CreateInvestigation(INV_FIRE_TOWER, false, 450, 10.0);
	CreateInvestigation(INV_ICE_TOWER, false, 320, 10.0);
	CreateInvestigation(INV_AIR_TOWER, false, 400, 10.0);
	//--

	return ret;
}

bool j1Investigations::Update(float dt)
{
	bool ret = true;

	for (int i = 0; i < investigations.size(); i++)
	{
		switch (investigations[i]->inv_state)
		{
		case INV_S_IDLE:
			break;

		case INV_S_WAITING_TO_INVESTIGATE:
			CanInvestigate(investigations[i]);
			break; 

		case INV_S_IN_COURSE:
			UpgradeInvestigation(investigations[i]);
			break;

		case INV_S_COMPLETED:
			if (investigations[i]->has_levels == true)
				investigations[i]->inv_state = INV_S_IDLE;
			break;

		default:
			break;
		}
	}
	return ret;
}

bool j1Investigations::CleanUp()
{
	bool ret = true;

	for (int i = 0; i < investigations.size();)
		DeleteInvestigation(investigations[i]);
	
	return ret;
}

Investigation* j1Investigations::CreateInvestigation(INVESTIGATION_TYPE investigation, bool has_lvls, uint cost, float time_to_upgrade)
{
	Investigation* new_inv = new Investigation(investigation, has_lvls, cost, time_to_upgrade);
	investigations.push_back(new_inv);

	return new_inv;
}

bool j1Investigations::DeleteInvestigation(Investigation* ptr)
{
	for (std::vector<Investigation*>::iterator it = investigations.begin(); it != investigations.end();)
	{
		if (*it == ptr)
		{
			investigations.erase(it);
			delete ptr;
			return true;
		}
		else
			it++;
	}
	return false;
}

bool j1Investigations::CanInvestigate(Investigation* investigation)
{
	if (App->scene->gold->GetGold() >= investigation->cost)
	{
		investigation->inv_state = INV_S_IN_COURSE;
		DoInvestigationUpgrade(investigation);
		//TODO: If there is a visual feedback when you click an investigation delete the fx line
		App->audio->PlayFx(fx_inv_completed);
		return true;
	}

	if (investigation->investigation_level == INV_LVL_UNLOCKED)
	{
		investigation->inv_state = INV_S_COMPLETED;
	}
		
	else
		investigation->inv_state = INV_S_IDLE;
	return false;
}

void j1Investigations::DoInvestigationUpgrade(Investigation* investigation)
{
	App->scene->gold->UseGold(investigation->cost);

	if (investigation->has_levels == true)
		investigation->cost += COST_INCREASE_BY_LVL;

	investigation->upgrade_timer.Start();
}

Investigation * j1Investigations::GetInvestigation(INVESTIGATION_TYPE name)
{
	for (int i = 0; i < investigations.size(); i++)
	{
		if (investigations[i]->investigation_type == name)
			return investigations[i];
	}
	return nullptr;
}

LEVEL j1Investigations::GetLevel(Investigation * investigation)
{
	return investigation->investigation_level;
}

bool j1Investigations::WantToInvestigate(Investigation * investigation)
{
	if (investigation->inv_state == INV_S_IDLE)
	{
		investigation->inv_state = INV_S_WAITING_TO_INVESTIGATE;
		return true;
	}
	return false;
}

int j1Investigations::GetInvestigationCost(Investigation* investigation)
{
	return investigation->cost;
}

void j1Investigations::InstaUnlockInvestigation(INVESTIGATION_TYPE name, LEVEL level)
{
	Investigation* actual_inv = GetInvestigation(name);
	actual_inv->investigation_level = level;
	if (level == INV_LVL_UNLOCKED)
	{
		actual_inv->inv_state = INV_S_COMPLETED;
	}
}

void j1Investigations::InstaUnlockAllInvestigations()
{
	for (int i = 0; i < investigations.size(); i++)
	{
		if (investigations[i]->investigation_type != INV_FOOD && investigations[i]->investigation_type != INV_WOOD &&
			investigations[i]->investigation_type != INV_GOLD && investigations[i]->investigation_type != INV_STONE)
		{
			investigations[i]->inv_state = INV_S_COMPLETED;
			investigations[i]->investigation_level = INV_LVL_UNLOCKED;
		}
	}
	App->audio->PlayFx(fx_inv_completed);
}

void j1Investigations::SetInvestigationCost(INVESTIGATION_TYPE name, int costt)
{
	GetInvestigation(name)->cost = costt;
}

bool j1Investigations::CanUpgradeTower(TOWER_TYPE new_type)
{
	switch (new_type)
	{
	case T_FIRE_TOWER:
	case T_BOMBARD_FIRE_TOWER:
		return App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED;
	case T_ICE_TOWER:
	case T_BOMBARD_ICE_TOWER:
		return App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ICE_TOWER)) == INV_LVL_UNLOCKED;
	case T_AIR_TOWER:
	case T_BOMBARD_AIR_TOWER:
		return App->investigations->GetLevel(App->investigations->GetInvestigation(INV_AIR_TOWER)) == INV_LVL_UNLOCKED;
	default:
		LOG("tower type unidentified, can't know if upgrade is posible");
	}
	return false;
}

bool j1Investigations::UpgradeInvestigation(Investigation* investigation)
{
	if (investigation->upgrade_timer.ReadSec() >= investigation->time_to_upgrade)
	{
		investigation->inv_state = INV_S_COMPLETED;
		App->audio->PlayFx(fx_inv_completed);

		if (investigation->has_levels == false)
		{
			investigation->investigation_level = INV_LVL_UNLOCKED;
			/* TODO dont know about tuto
			if (App->tutorial->tutorial_num == TUTORIAL_4 && investigation->investigation_type == INV_FIRE_TOWER)
			{
				App->tutorial->InvestigationDone = true;
			}*/
			return true;
		}

		switch (investigation->investigation_type)
		{
		case INV_GOLD:
			/* TODO probablly should increase gold per kill
			if(!App->scene->gold->ReduceGoldCollectTime(2.0f))
				App->scene->resources->IncreaseResourceAmount(R_GOLD, 25.0f);*/
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}
