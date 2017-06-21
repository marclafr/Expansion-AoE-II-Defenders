#define BAR_SCORE_POSITION {360, 310, 703, 19}
#define BAR_SCORE_ATLAS {0, 2585, 703, 19}

#define YELLOWBAR_SCORE_POSITION {365, 313, 698, 11}
#define YELLOWBAR_SCORE_ATLAS {0, 2572, 698, 11}

#define BAR_PIXELS 698

#include "UIHUDScoreBar.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1EntityManager.h"
#include "j1Score.h"

#include "UIGetEntitiesInfo.h"

UIHUDScoreBar::UIHUDScoreBar(UICOMPONENT_TYPE type) : UIComponents(type)
{
	Set(BAR_SCORE_POSITION, BAR_SCORE_ATLAS);
	SetInteractive(false);

	score_bar = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
	score_bar->Set(YELLOWBAR_SCORE_POSITION, YELLOWBAR_SCORE_ATLAS);

	rest_score_bar = YELLOWBAR_SCORE_ATLAS;
}

bool UIHUDScoreBar::Update()
{
	//rest_score_bar.w = ReturnValueBarHPBuilding(BUILDING_TYPE::B_TOWNHALL, townhall->GetHp(), BAR_PIXELS);
	rest_score_bar.w = App->score->GetScore() / 120;

	Draw();
	return true;
}

void UIHUDScoreBar::Draw()
{
	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas);
	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), score_bar->GetPosRect().x - App->render->camera->GetPosition().x, score_bar->GetPosRect().y - App->render->camera->GetPosition().y, &rest_score_bar);
}