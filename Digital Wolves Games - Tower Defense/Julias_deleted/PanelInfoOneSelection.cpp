#define BLACK {0,0,0,0}

#define GREEN_BAR_SELECTED {1029, 861, 29, 6}
#define GREEN_BAR_PIXELS 29

#define BAR_LIFE_CENTER 16

#include "PanelInfoOneSelection.h"

#include "j1App.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Scene.h"

#include "UIGetEntitiesInfo.h"

OneSelection::~OneSelection()
{
	DELETE_PTR(image);
	DELETE_PTR(name);
	DELETE_PTR(life);
	DELETE_PTR(damage);
	DELETE_PTR(armor);
	DELETE_PTR(range);
	DELETE_PTR(building_percentage);
}

void OneSelection::Prepare()
{
	life_bar_selected = GREEN_BAR_SELECTED;

	e_ptr = App->scene->selection.front();

	switch (e_ptr->GetEntityType())
	{
	case E_UNIT:
		PrepareUnitInfo();
		break;
	case E_BUILDING:
		PrepareBuildInfo();
		App->uimanager->SetPanelButtons((Building*)e_ptr);
		break;
	case E_RESOURCE:
		PrepareResourceInfo();
		break;
	}
}

void OneSelection::PrepareUnitInfo()
{
	Unit* selected = (Unit*)e_ptr;

	image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
	iPoint u_position(GetUnitIconPositionFromAtlas(selected->GetUnitType()));
	image->Set({ 231, 667, 29, 33 }, { u_position.x,u_position.y, ICON_SIZE, ICON_SIZE });

	name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
	name->Set(231, 653, GetUnitName(selected->GetUnitType()), BLACK);

	std::string stats = std::to_string(selected->GetHp());
	life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
	life->Set(262, 690, stats.c_str(), BLACK);

	stats = std::to_string(selected->GetAttack());
	damage = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
	damage->Set(272, 705, stats.c_str(), BLACK);

	stats = std::to_string(selected->GetArmor());
	armor = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
	armor->Set(272, 722, stats.c_str(), BLACK);

	if (selected->GetUnitClass() == UNIT_CLASS::C_ARCHER)
	{
		stats = std::to_string(selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 745, stats.c_str(), BLACK);
	}
}

void OneSelection::PrepareBuildInfo()
{
	Building* b_selected = (Building*)e_ptr;
	Tower* t_selected = nullptr;
	iPoint atlas_position{ 0,0 };
	std::string stats;

	switch (b_selected->GetBuildingType())
	{
	case B_TURRET:
		t_selected = (Tower*)e_ptr;
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetTowerName(t_selected->GetTowerType()), BLACK);

		stats = std::to_string(t_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);

		stats = std::to_string(t_selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
		break;
	case B_TURRET_UPGRADED_FIRE:
		t_selected = (Tower*)e_ptr;
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetTowerName(t_selected->GetTowerType()), BLACK);

		stats = std::to_string(t_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);

		stats = std::to_string(t_selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
		break;
	case B_TURRET_UPGRADED_ICE:
		t_selected = (Tower*)e_ptr;
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetTowerName(t_selected->GetTowerType()), BLACK);

		stats = std::to_string(t_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);

		stats = std::to_string(t_selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
		break;
	case B_TURRET_UPGRADED_AIR:
		t_selected = (Tower*)e_ptr;
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetTowerName(t_selected->GetTowerType()), BLACK);

		stats = std::to_string(t_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);

		stats = std::to_string(t_selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
		break;
	case B_CANNON:
		t_selected = (Tower*)e_ptr;
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetTowerName(t_selected->GetTowerType()), BLACK);

		stats = std::to_string(t_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);

		stats = std::to_string(t_selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
		break;
	case B_CANNON_UPGRADED_FIRE:
		t_selected = (Tower*)e_ptr;
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetTowerName(t_selected->GetTowerType()), BLACK);

		stats = std::to_string(t_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);

		stats = std::to_string(t_selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
		break;
	case B_CANNON_UPGRADED_ICE:
		t_selected = (Tower*)e_ptr;
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetTowerName(t_selected->GetTowerType()), BLACK);

		stats = std::to_string(t_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);

		stats = std::to_string(t_selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
		break;
	case B_CANNON_UPGRADED_AIR:
		t_selected = (Tower*)e_ptr;
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetTowerIconPositionFromAtlas(t_selected->GetTowerType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetTowerName(t_selected->GetTowerType()), BLACK);

		stats = std::to_string(t_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);

		stats = std::to_string(t_selected->GetRange());
		range = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		range->Set(272, 710, stats.c_str(), BLACK);
		break;
	default:
		image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);
		atlas_position = GetBuildingIconPositionFromAtlas(b_selected->GetBuildingType());
		image->Set({ 231, 667, 29, 33 }, { atlas_position.x, atlas_position.y, ICON_SIZE, ICON_SIZE });

		name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		name->Set(231, 653, GetBuildingName(b_selected->GetBuildingType()), BLACK);

		stats = std::to_string(b_selected->GetHp());
		life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
		life->Set(262, 690, stats.c_str(), BLACK);
		break;
	}
}

void OneSelection::PrepareBuildingConstruction()
{
	building_percentage = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
	uint percentage = GetBuildingPercentage(e_ptr);
	std::string str_percentage("Constructing - ");
	str_percentage += std::to_string(percentage);
	str_percentage += "%";
	building_percentage->Set(400, 703, str_percentage.c_str(), BLACK);
}

void OneSelection::PrepareResourceInfo()
{
	Resources* selected = (Resources*)e_ptr;

	RESOURCE_TYPE r_type_selected = selected->GetResourceType();

	image = new UIComponents(UICOMPONENT_TYPE::UIT_UIIMAGE);			
	iPoint e_position(GetResourceIconPositionFromAtlas(selected->GetResourceType()));
	image->Set({ 231, 667, 29, 33 }, { e_position.x, e_position.y, ICON_SIZE, ICON_SIZE });

	name = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
	name->Set(231, 653, GetResourceName(r_type_selected), BLACK);

	std::string stats = std::to_string(selected->GetHp());
	life = new UILabel(UICOMPONENT_TYPE::UIT_UILABEL);
	life->Set(262, 690, stats.c_str(), BLACK);
}

void OneSelection::Update()
{
	if (e_ptr->GetHp() <= 0)
		delete_panel_info = true;
	else
	{
		UpdateHP();

		if (e_ptr->GetEntityType() == E_BUILDING)
		{
			Building* b_ptr = (Building*)e_ptr;
			if (b_ptr->IsBuilt() == false)
			{
				PrepareBuildingConstruction();
			}
		}
	}

	Draw();
}

void OneSelection::UpdateHP()
{
	std::string hp = std::to_string(e_ptr->GetHp());
	life->ChangeText(hp.c_str(), BLACK);
}

void OneSelection::UpdateBuildingPercentageStr()
{
	uint percentage = GetBuildingPercentage(e_ptr);
	std::string str_percentage("Constructing - ");
	str_percentage += std::to_string(percentage);
	str_percentage += "%";

	building_percentage->ChangeText(str_percentage.c_str(), BLACK);
}

void OneSelection::Draw()
{
	e_ptr = App->scene->selection.front();

	switch (e_ptr->GetEntityType())
	{
	case E_UNIT:
		DrawUnitInfo();
		break;
	case E_BUILDING:
		DrawBuildInfo();
		break;
	case E_RESOURCE:
		DrawResourceInfo();
		break;
	}
}

void OneSelection::DrawUnitInfo()
{
	Unit* unit_life_bar = (Unit*)e_ptr;

	if (unit_life_bar->GetHp() > 0)
	{
		life_bar_selected.w = ReturnValueBarHPUnit(unit_life_bar->GetUnitType(), unit_life_bar->GetHp(), GREEN_BAR_PIXELS);

		//Bar life
		int rest_life_bar = ReturnValueBarHPUnit(unit_life_bar->GetUnitType(), unit_life_bar->GetHp());
		int height_correction = ReturnValueHeightCorrectionUnit(unit_life_bar->GetUnitClass());
		SDL_Rect mark_life_bar_red{ 1059, 832, 32, 4 };
		SDL_Rect mark_life_bar_green{ 1059, 827, rest_life_bar, 4 };
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), unit_life_bar->GetX() - BAR_LIFE_CENTER, unit_life_bar->GetY() - height_correction, &mark_life_bar_red);
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), unit_life_bar->GetX() - BAR_LIFE_CENTER, unit_life_bar->GetY() - height_correction, &mark_life_bar_green);

		UIComponents* entity_image = image;

		SDL_Rect mark_btn{ 1029, 827, 29, 33 };
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x + 2 - App->render->camera->GetPosition().x, entity_image->GetPosRect().y + 2 - App->render->camera->GetPosition().y, &entity_image->GetAtlasRect());
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x - App->render->camera->GetPosition().x, entity_image->GetPosRect().y - App->render->camera->GetPosition().y, &mark_btn);
		//bar_life
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x - App->render->camera->GetPosition().x, entity_image->GetPosRect().y + 27 - App->render->camera->GetPosition().y, &life_bar_selected);

		UILabel* entity_label = name;
		App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);

		entity_label = life;
		App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);

		SDL_Rect mark_attack{ 956, 858, 38, 22 };
		entity_label = damage;
		App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), 230 - App->render->camera->GetPosition().x, 703 - App->render->camera->GetPosition().y, &mark_attack);

		SDL_Rect mark_armor{ 956, 902, 37, 19 };
		entity_label = armor;
		App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), 230 - App->render->camera->GetPosition().x, 725 - App->render->camera->GetPosition().y, &mark_armor);

		entity_label = range;
		if (entity_label != nullptr)
		{
			SDL_Rect mark_range{ 956, 881, 35, 20 };
			App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);
			App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), 231 - App->render->camera->GetPosition().x, 744 - App->render->camera->GetPosition().y, &mark_range);
		}
	}
}

void OneSelection::DrawBuildInfo()
{
	Building* building_life_bar = (Building*)e_ptr;

	if (building_life_bar->GetHp() > 0)
	{
		int rest_life_bar = 0;
		int height_correction = 0;

		if (building_life_bar->GetBuildingType() == B_CANNON ||
			building_life_bar->GetBuildingType() == B_TURRET ||
			building_life_bar->GetBuildingType() == B_TURRET_UPGRADED_FIRE ||
			building_life_bar->GetBuildingType() == B_TURRET_UPGRADED_ICE ||
			building_life_bar->GetBuildingType() == B_TURRET_UPGRADED_AIR ||
			building_life_bar->GetBuildingType() == B_CANNON_UPGRADED_FIRE ||
			building_life_bar->GetBuildingType() == B_CANNON_UPGRADED_ICE ||
			building_life_bar->GetBuildingType() == B_CANNON_UPGRADED_AIR)
		{
			Tower* info_tower = (Tower*)building_life_bar;

			life_bar_selected.w = ReturnValueBarHPTower(info_tower->GetTowerType(), info_tower->GetHp(), GREEN_BAR_PIXELS);

			//Bar life
			rest_life_bar = ReturnValueBarHPTower(info_tower->GetTowerType(), info_tower->GetHp());
			height_correction = ReturnValueHeightCorrectionTowers(info_tower->GetTowerType());
		}
		else 
		{
			life_bar_selected.w = ReturnValueBarHPBuilding(building_life_bar->GetBuildingType(), building_life_bar->GetHp(), GREEN_BAR_PIXELS);

			//Bar life
			rest_life_bar = ReturnValueBarHPBuilding(building_life_bar->GetBuildingType(), building_life_bar->GetHp());
			height_correction = ReturnValueHeightCorrectionBuilding(building_life_bar->GetBuildingType());
		}

		SDL_Rect mark_life_bar_red{ 1059, 832, 32, 4 };
		SDL_Rect mark_life_bar_green{ 1059, 827, rest_life_bar, 4 };
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), building_life_bar->GetX() - BAR_LIFE_CENTER, building_life_bar->GetY() - height_correction, &mark_life_bar_red);
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), building_life_bar->GetX() - BAR_LIFE_CENTER, building_life_bar->GetY() - height_correction, &mark_life_bar_green);

		UIComponents* entity_image = image;

		SDL_Rect mark_btn{ 1029, 827, 29, 33 };
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x + 2 - App->render->camera->GetPosition().x, entity_image->GetPosRect().y + 2 - App->render->camera->GetPosition().y, &entity_image->GetAtlasRect());
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x - App->render->camera->GetPosition().x, entity_image->GetPosRect().y - App->render->camera->GetPosition().y, &mark_btn);
		//bar_life
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x - App->render->camera->GetPosition().x, entity_image->GetPosRect().y + 27 - App->render->camera->GetPosition().y, &life_bar_selected);

		UILabel* entity_label = name;
		App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);

		entity_label = life;
		App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);

		entity_label = range;
		if (entity_label != nullptr)
		{
			SDL_Rect mark_range{ 956, 881, 35, 20 };
			App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);
			App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), 231 - App->render->camera->GetPosition().x, 711 - App->render->camera->GetPosition().y, &mark_range);
		}

		if (building_life_bar->IsBuilt() == false)
		{
			uint percentage = GetBuildingPercentage(e_ptr);
			if (percentage < 100)
			{
				UpdateBuildingPercentageStr();
				entity_label = building_percentage;
				App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);

				entity_label = name;
				App->render->PushUISprite(entity_label->GetTexture(), 400 - App->render->camera->GetPosition().x, 716 - App->render->camera->GetPosition().y);

				//Icon
				SDL_Rect mark_icon{ 339, 1162, 40, 40 };
				App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), 356 - App->render->camera->GetPosition().x, 701 - App->render->camera->GetPosition().y, &mark_icon);

				//Progress bar
				SDL_Rect mark_clear_bar{ 300, 1203, 100, 12 };
				SDL_Rect mark_last_bar{ 403, 1216, 1, 12 };
				SDL_Rect mark_full_bar{ 298, 1216, 2 + (int)percentage, 12 };
				App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), 400 - App->render->camera->GetPosition().x, 729 - App->render->camera->GetPosition().y, &mark_clear_bar);
				App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), 399 - App->render->camera->GetPosition().x, 729 - App->render->camera->GetPosition().y, &mark_full_bar);
				if (percentage < 98)
					App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), 401 + (int)percentage - App->render->camera->GetPosition().x, 729 - App->render->camera->GetPosition().y, &mark_last_bar);
			}

		}
	}
}

void OneSelection::DrawResourceInfo()
{
	Resources* resource_life_bar = (Resources*)e_ptr;

	if (resource_life_bar->GetHp() > 0)
	{
		life_bar_selected.w = ReturnValueBarHPResource(resource_life_bar->GetResourceType(), resource_life_bar->GetHp(), GREEN_BAR_PIXELS);

		//Bar life
		int rest_life_bar = ReturnValueBarHPResource(resource_life_bar->GetResourceType(), resource_life_bar->GetHp());
		int height_correction = ReturnValueHeightCorrectionResource(resource_life_bar->GetResourceType());
		SDL_Rect mark_life_bar_red{ 1059, 832, 32, 4 };
		SDL_Rect mark_life_bar_green{ 1059, 827, rest_life_bar, 4 };
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), resource_life_bar->GetX() - BAR_LIFE_CENTER, resource_life_bar->GetY() - height_correction, &mark_life_bar_red);
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), resource_life_bar->GetX() - BAR_LIFE_CENTER, resource_life_bar->GetY() - height_correction, &mark_life_bar_green);

		UIComponents* entity_image = image;

		SDL_Rect mark_btn{ 1029, 827, 29, 33 };
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x + 2 - App->render->camera->GetPosition().x, entity_image->GetPosRect().y + 2 - App->render->camera->GetPosition().y, &entity_image->GetAtlasRect());
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x - App->render->camera->GetPosition().x, entity_image->GetPosRect().y - App->render->camera->GetPosition().y, &mark_btn);
		//bar_life
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), entity_image->GetPosRect().x - App->render->camera->GetPosition().x, entity_image->GetPosRect().y + 27 - App->render->camera->GetPosition().y, &life_bar_selected);

		UILabel* entity_label = name;
		App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);

		entity_label = life;
		App->render->PushUISprite(entity_label->GetTexture(), entity_label->GetPosRect().x - App->render->camera->GetPosition().x, entity_label->GetPosRect().y - App->render->camera->GetPosition().y);
	}
}
