#ifndef __GETENTITYINFO__
#define __GETENTITYINFO__

#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "Resources.h"
#include "Towers.h"
#include "j1Investigations.h"

#define TURRET_BUILD 9
#define CANNON_BUILD 9
#define WOODWALL_BUILD 6

#define ICON_SIZE 25

iPoint GetUnitIconPositionFromAtlas(const UNIT_TYPE type);
const char* GetUnitName(const UNIT_TYPE type);
const char* GetUnitPrice(const UNIT_TYPE type);

iPoint GetResourceIconPositionFromAtlas(const RESOURCE_TYPE type);
const char* GetResourceName(const RESOURCE_TYPE type);

iPoint GetBuildingIconPositionFromAtlas(const BUILDING_TYPE type);
const char* GetBuildingName(const BUILDING_TYPE type);
const char* GetBuildingPrice(const BUILDING_TYPE type);

const uint GetBuildingPercentage(const Entity* build);
const double GetBuildTotalTime(const BUILDING_TYPE type);
const bool isBuilded(const Entity* build);

iPoint GetInvestigationIconPositionFromAtlas(const INVESTIGATION_TYPE type);
iPoint GetGreyInvestigationIconPositionFromAtlas(const INVESTIGATION_TYPE type);

const char* GetExpandButtonsDescription(SDL_Rect rect_pos);
const uint GetInvestigationPercentage(const Investigation* type);
const char* GetInvestigationName(const INVESTIGATION_TYPE type);
void GetInvestigationPrice(const INVESTIGATION_TYPE type, std::string & price);


iPoint GetTowerIconPositionFromAtlas(const TOWER_TYPE type);
const char* GetTowerName(const TOWER_TYPE type);
const char* GetTowerPrice(const TOWER_TYPE type);
const char* GetTowerUpgradeName(const INVESTIGATION_TYPE type);
const char* GetTowerUpgradePrice(const INVESTIGATION_TYPE type);

const char* GetWallUpgradeName(const BUILDING_TYPE type);
const char* GetWallUpgradePrice(const BUILDING_TYPE type);

const char* GetSideName(const Side type);

int ReturnValueBarHPUnit(const UNIT_TYPE type, const uint hp, const uint pixels = 32);
int ReturnValueBarHPBuilding(const BUILDING_TYPE type, const uint hp, const uint pixels = 32);
int ReturnValueBarHPTower(const TOWER_TYPE type, const uint hp, const uint pixels = 32);
int ReturnValueBarHPResource(const RESOURCE_TYPE type, const uint hp, const uint pixels = 32);

int ReturnValueHeightCorrectionUnit(const UNIT_CLASS u_class);
int ReturnValueHeightCorrectionBuilding(const BUILDING_TYPE type);
int ReturnValueHeightCorrectionResource(const RESOURCE_TYPE type);
int ReturnValueHeightCorrectionTowers(const TOWER_TYPE type);
#endif