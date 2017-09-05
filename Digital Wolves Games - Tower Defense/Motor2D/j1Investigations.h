#ifndef _j1Investigations_
#define _j1Investigations_

#include "j1Module.h"
#include "j1Timer.h"
#include <vector>

#define COST_INCREASE_BY_LVL 100

class Resource;
enum TOWER_TYPE;

enum LEVEL
{
	INV_LVL_LOCKED,
	INV_LVL_UNLOCKED,
};

enum INVESTIGATION_TYPE
{
//RESOURCES
	INV_FOOD,
	INV_WOOD,
	INV_GOLD,
	INV_STONE,
//TROPS
	INV_CAVALRY_ATTACK,
	INV_CAVALRY_DEFENSE,
	INV_ARCHERS_ATTACK,
	INV_ARCHERS_DEFENSE,
	INV_INFANTRY_ATTACK,
	INV_INFANTRY_DEFENSE,
//TOWERS
	INV_FIRE_TOWER,
	INV_ICE_TOWER,
	INV_AIR_TOWER,

	INV_NONE
};

enum INVESTIGATION_STATE
{
	INV_S_IDLE,
	INV_S_WAITING_TO_INVESTIGATE,
	INV_S_IN_COURSE,
	INV_S_COMPLETED
};

class Investigation
{
public:
	Investigation(INVESTIGATION_TYPE investigation, bool has_lvls, uint cost, float time) : investigation_type(investigation), has_levels(has_lvls), cost(cost), time_to_upgrade(time), investigation_level(INV_LVL_LOCKED){}
	~Investigation(){}

public:
	INVESTIGATION_TYPE investigation_type;
	bool has_levels = true;
	LEVEL investigation_level = INV_LVL_LOCKED;
	INVESTIGATION_STATE inv_state = INV_S_IDLE;
	float time_to_upgrade;
	j1Timer upgrade_timer;
	uint cost = 0;
};

class j1Investigations : public j1Module
{
public:

	j1Investigations();
	~j1Investigations();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	Investigation* GetInvestigation(INVESTIGATION_TYPE name);
	LEVEL GetLevel(Investigation* investigation);
	bool WantToInvestigate(Investigation* investigation);
	int GetInvestigationCost(Investigation* investigation);
	void InstaUnlockInvestigation(INVESTIGATION_TYPE name, LEVEL level);
	void InstaUnlockAllInvestigations();
	void SetInvestigationCost(INVESTIGATION_TYPE name, int cost);

	bool CanUpgradeTower(TOWER_TYPE new_type);

private:
	Investigation* CreateInvestigation(INVESTIGATION_TYPE investigation, bool has_lvls, uint cost, float time_to_upgrade);
	bool DeleteInvestigation(Investigation* ptr);

	std::vector<Investigation*> investigations;
	bool CanInvestigate(Investigation* investigation);
	void DoInvestigationUpgrade(Investigation* investigation);
	bool UpgradeInvestigation(Investigation* investigation);

	int fx_inv_completed = 0;

};
#endif //_j1Investigations_
