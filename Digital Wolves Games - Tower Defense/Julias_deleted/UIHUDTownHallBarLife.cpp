#define BAR_LIFE_POSITION {0, 625, 1366, 20}
#define BAR_LIFE_ATLAS {0, 1483, 1366, 20}

#define GREENBAR_LIFE_POSITION {5, 628, 1356, 12}
#define GREENBAR_LIFE_ATLAS {5, 1470, 1356, 12}

#define LIFE_LABEL_X 640
#define LIFE_LABEL_Y 625

#define BAR_PIXELS 1356

#include "UIHUDTownHallBarLife.h"

#include "j1App.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "Camera.h"

#include "UIGetEntitiesInfo.h"

UIHUDTownHallBarLife::UIHUDTownHallBarLife(UICOMPONENT_TYPE type) : UIComponents(type)
{
	Set(BAR_LIFE_POSITION, BAR_LIFE_ATLAS);
	SetInteractive(false);

	green_bar = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
	green_bar->Set(GREENBAR_LIFE_POSITION, GREENBAR_LIFE_ATLAS);

	life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
	life->Set(LIFE_LABEL_X, LIFE_LABEL_Y, "init");

	rest_green_bar = GREENBAR_LIFE_ATLAS;
}

void UIHUDTownHallBarLife::SetTownHall(Building * th)
{
	townhall = th;
}

bool UIHUDTownHallBarLife::Update()
{
	rest_green_bar.w = ReturnValueBarHPBuilding(BUILDING_TYPE::B_TOWNHALL, townhall->GetHp(), BAR_PIXELS);

	hp_townhall = std::to_string(townhall->GetHp());
	hp_townhall += "/1500";
	life->ChangeText(hp_townhall.c_str());

	Draw();
	return true;
}

void UIHUDTownHallBarLife::Draw()
{
	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas);

	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), green_bar->GetPosRect().x - App->render->camera->GetPosition().x, green_bar->GetPosRect().y - App->render->camera->GetPosition().y, &rest_green_bar);

	App->render->PushUISprite(life->GetTexture(), life->GetPosRect().x - App->render->camera->GetPosition().x, life->GetPosRect().y - App->render->camera->GetPosition().y);
}