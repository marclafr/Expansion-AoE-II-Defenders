#define Y_LABEL 1

#include "UIHUDResources.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"
#include "j1Scene.h"

#include "Resources.h"

#include "SDL\include\SDL_rect.h"

#include "UILabel.h"

#include "UIGetEntitiesInfo.h"

UIHUDResources::UIHUDResources(UICOMPONENT_TYPE type) : UIComponents(type)
{
	SetInteractive(false);
	stone = App->uimanager->AddLabel(197, Y_LABEL, "0");
	food = App->uimanager->AddLabel(76, Y_LABEL, "0");
	wood = App->uimanager->AddLabel(21, Y_LABEL, "0");
	gold = App->uimanager->AddLabel(143, Y_LABEL, "0");
}

bool UIHUDResources::Update()
{
	std::string num_stone_str = std::to_string(App->scene->resources->GetStone());
	stone->ChangeText(num_stone_str.c_str());

	std::string num_food_str = std::to_string(App->scene->resources->GetFood());
	food->ChangeText(num_food_str.c_str());

	std::string num_gold_str = std::to_string(App->scene->resources->GetGold());
	gold->ChangeText(num_gold_str.c_str());

	std::string num_wood_str = std::to_string(App->scene->resources->GetWood());
	wood->ChangeText(num_wood_str.c_str());

	return true;
}
