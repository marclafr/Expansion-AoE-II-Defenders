#include "j1UIManager.h"

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1FileSystem.h"
#include "Camera.h"

#include "UIComponents.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UICheckbutton.h"
#include "UISelectOption.h"
#include "UIHUDPanelButtons.h"
#include "UIHUDPanelInfo.h"
#include "UIHUDDescription.h"
#include "UIHUDResources.h"
#include "UIHUDTownHallBarLife.h"
#include "UIHUDScoreBar.h"
#include "UIHUDMinimap.h"
#include "UIHUDMenuInGame.h"

j1UIManager::j1UIManager() : j1Module()
{
	name.assign("uimanager");
}

// Destructor
j1UIManager::~j1UIManager()
{}

// Called before render is available
bool j1UIManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1UIManager::Start()
{
	atlas = App->tex->Load(atlas_file_name.c_str(), T_UI);

	return true;
}

// Update all guis
bool j1UIManager::PreUpdate()
{
	int x_mouse = 0;
	int y_mouse = 0;

	App->input->GetMousePosition(x_mouse, y_mouse);

	for (std::list<UIComponents*>::iterator item = components.begin(); item != components.end(); ++item)
	{
		if ((*item)->GetInteractive() &&
			(x_mouse > (*item)->GetPosRect().x) &&
			(x_mouse < (*item)->GetPosRect().x + (*item)->GetPosRect().w) &&
			(y_mouse > (*item)->GetPosRect().y) &&
			(y_mouse < (*item)->GetPosRect().y + (*item)->GetPosRect().h))
		{
			focus = (*item);
			focus->SetIsFocus(true);
			break;
		}
		else
			(*item)->SetIsFocus(false);
	}
	return true;
}

// Update all guis
bool j1UIManager::Update(float dt)
{
	for (std::list<UIComponents*>::iterator it = components.begin(); it != components.end(); ++it)
		if (!(*it)->Update())
			return false;

	return true;
}

// Called after all Updates
bool j1UIManager::PostUpdate()
{
	for (std::list<UIComponents*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->ToDelete())
		{
			delete *it;
			components.erase(it);
		}
	}
	return true;
}

void j1UIManager::HandleInput(SDL_Event event)
{
	focus->HandleInput(event);
}

// Called before quitting
bool j1UIManager::CleanUp()
{
	LOG("Freeing GUI");

	std::list<UIComponents*>::iterator item;
	item = components.begin();

	while (item != components.end())
	{
		delete *item;

		item++;
	}

	components.clear();

	return true;
}

UIComponents* j1UIManager::AddComponent(UICOMPONENT_TYPE type, const SDL_Rect & position, const SDL_Rect & atlas)
{
	UIComponents* new_component;
	components.push_back(new_component = new UIComponents(type));
	new_component->Set(position, atlas);
	if (type == UIT_UIIMAGE)
		new_component->SetInteractive(false);
	return new_component;
}

UIButton* j1UIManager::AddButton(const SDL_Rect & position, const SDL_Rect & atlas, bool panel_buttons)
{
	UIButton* new_btn;
	components.push_back(new_btn = new UIButton(UIT_UIBUTTON));
	new_btn->Set(position, atlas, panel_buttons);
	return new_btn;
}

UILabel * j1UIManager::AddLabel(int pos_x, int pos_y, const char * text, SDL_Color color, _TTF_Font * font)
{
	UILabel* new_label;
	components.push_back(new_label = new UILabel(UIT_UILABEL));
	new_label->Set(pos_x, pos_y, text, color, font);
	return new_label;
}

UICheckbutton * j1UIManager::AddCheckButton(const SDL_Rect & position, const SDL_Rect & atlas, const SDL_Rect & atlas_clicked)
{
	UICheckbutton* new_checkbutton;
	components.push_back(new_checkbutton = new UICheckbutton(UIT_UICHECKBUTTON));
	new_checkbutton->Set(position, atlas, atlas_clicked);
	return new_checkbutton;
}

void j1UIManager::AddPanelInfo()
{
	UIHUDPanelInfo* new_panelinfo;
	components.push_back(new_panelinfo = new UIHUDPanelInfo(UIT_UIHUDPANELINFO));
	panel_info = new_panelinfo;
}

UIHUDDescription * j1UIManager::AddPanelDescription()
{
	UIHUDDescription* new_paneldescription;
	components.push_back(new_paneldescription = new UIHUDDescription(UIT_UIHUDDESCRIPTION));
	hud_description = new_paneldescription;
	return new_paneldescription;
}

UIHUDPanelButtons * j1UIManager::AddPanelButtons()
{
	UIHUDPanelButtons* new_panelbuttons;
	components.push_back(new_panelbuttons = new UIHUDPanelButtons(UIT_UIHUDPANELBUTTONS));
	panel_buttons = new_panelbuttons;
	return new_panelbuttons;
}

void j1UIManager::AddResourcesPanel()
{
	UIHUDResources* new__resourcespanel;
	components.push_back(new__resourcespanel = new UIHUDResources(UIT_UIHUDRESOURCES));
	resources_panel = new__resourcespanel;
}

UIHUDScoreBar* j1UIManager::AddScoreBar()
{
	UIHUDScoreBar* new_scorebar;
	components.push_back(new_scorebar = new UIHUDScoreBar(UIT_UIHUDSCOREBAR));
	return new_scorebar;
}

void j1UIManager::AddTownHallBarLife()
{
	UIHUDTownHallBarLife* new_thbarlife;
	components.push_back(new_thbarlife = new UIHUDTownHallBarLife(UICOMPONENT_TYPE::UIT_UIHUDTOWNHALLBARLIFE));
	townhall_bar_life = new_thbarlife;
}

void j1UIManager::AddMinimap()
{
	UIHUDMinimap* new_minimap;
	components.push_back(new_minimap = new UIHUDMinimap());
	minimap = new_minimap;
}

UIHUDMenuInGame* j1UIManager::AddMenuInGame(UICheckbutton* enable)
{
	UIHUDMenuInGame* new_menuingame;
	components.push_back(new_menuingame = new UIHUDMenuInGame());
	new_menuingame->SetEnableButton(enable);
	menu_btn = enable;
	return new_menuingame;
}

UIHUDMinimap * j1UIManager::GetMinimap()
{
	return minimap;
}

// const getter for atlas
const SDL_Texture* j1UIManager::GetAtlas() const
{
	return atlas;
}

const std::list<UIComponents*>::iterator j1UIManager::GetLastComponent()
{
	std::list<UIComponents*>::iterator temp = components.end();

	--temp;

	return temp;
}

const bool j1UIManager::InUse() const
{
	if (focus != nullptr)
		return true;
	return false;
}

void j1UIManager::SetAllToDelete()
{
	for (std::list<UIComponents*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->SetToDelete();
	}
}

void j1UIManager::SetTownHall(Building * th)
{
	townhall_bar_life->SetTownHall(th);
}

void j1UIManager::CreatePanelInfo(const std::vector<Entity*>& selection)
{
	panel_info->CreatePanel(selection);
}

void j1UIManager::DeleteSelectionPanelInfo()
{
	panel_info->DeletePanelInfo();
}

void j1UIManager::SetDescriptionHUDDescription(info_button* infobutton_desc)
{
	if(hud_description != nullptr)
		hud_description->SetDescription(infobutton_desc);
}

void j1UIManager::ClearDescription()
{
	if(hud_description != nullptr)
		hud_description->Clear();
}

void j1UIManager::SetInfoUIComponent(UIComponents* component)
{
	info_ui = component;
}

SDL_Rect j1UIManager::GetPosRectFromInfoUI()
{
	return info_ui->GetPosRect();
}

void j1UIManager::SetPanelButtons(Building* building)
{
	if(panel_buttons != nullptr)
		panel_buttons->SetPanel(building);
}

UIHUDPanelButtons* j1UIManager::GetPanelButtons() const
{
	return panel_buttons;
}

void j1UIManager::DeletePanelButtons()
{
	if(panel_buttons != nullptr)
		panel_buttons->DeletePanel();
}

void j1UIManager::ShowInGameMenu()
{
	if (menu_btn->GetStat() == CB_UNCHECK)
		menu_btn->SetStat(CB_CHECK);
	else
		menu_btn->SetStat(CB_UNCHECK);
}

void j1UIManager::DrawMinimapQuad()
{
	minimap->DrawQuad();
}
