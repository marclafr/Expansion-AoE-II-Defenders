#ifndef __j1GUIMANAGER_H__
#define __j1GUIMANAGER_H__

#include "j1Module.h"
#include <list>
#include <vector>
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;
union SDL_Event;
class UIComponents;
class UIButton;
class UILabel;
struct _TTF_Font;
class UICheckbutton;
class UIHUDPanelButtons;
enum UICOMPONENT_TYPE;
class UIHUDTownHallBarLife;
class UIHUDResources;
class UIHUDPanelInfo;
class UIHUDDescription;
class UIHUDScoreBar;
class UIHUDMinimap;
class UIHUDMenuInGame;
struct info_button;
class Building;
class Resources;
class Entity;

#define CURSOR_WIDTH 2

class j1UIManager : public j1Module
{
public:

	j1UIManager();

	// Destructor
	virtual ~j1UIManager();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Update all guis
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	//Handle inputs
	void HandleInput(SDL_Event);

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	UIComponents* AddComponent(UICOMPONENT_TYPE type, const SDL_Rect & position, const SDL_Rect & atlas);
	UIButton * AddButton(const SDL_Rect & position, const SDL_Rect & atlas, bool panel_buttons = false);
	UILabel* AddLabel(int pos_x, int pos_y, const char * text, SDL_Color color = { 255,255,255,0 }, _TTF_Font*  font = nullptr);
	UICheckbutton* AddCheckButton(const SDL_Rect& position, const SDL_Rect& atlas, const SDL_Rect & atlas_clicked);
	void AddPanelInfo();
	UIHUDDescription* AddPanelDescription();
	UIHUDPanelButtons* AddPanelButtons();
	void AddResourcesPanel();
	UIHUDScoreBar* AddScoreBar();
	void AddTownHallBarLife();
	void AddMinimap();
	UIHUDMenuInGame* AddMenuInGame(UICheckbutton* enable);

	UIHUDMinimap* GetMinimap();
	const SDL_Texture* GetAtlas() const;
	const std::list<UIComponents*>::iterator GetLastComponent();
	const bool InUse() const;
	void SetAllToDelete();

	void SetTownHall(Building*);
	void CreatePanelInfo(const std::vector<Entity*>& selection);
	void DeleteSelectionPanelInfo();
	void SetDescriptionHUDDescription(info_button*);
	void ClearDescription();
	void SetInfoUIComponent(UIComponents*);
	SDL_Rect GetPosRectFromInfoUI();
	void SetPanelButtons(Building*);
	UIHUDPanelButtons* GetPanelButtons() const;
	void DeletePanelButtons();
	void ShowInGameMenu();
	void DrawMinimapQuad();

private:
	std::list<UIComponents*> components;
	SDL_Texture* atlas;
	std::string atlas_file_name;
	UIComponents* focus = nullptr;

	//Pointers of some compontnts from scenes
	UIHUDTownHallBarLife* townhall_bar_life = nullptr;
	UIHUDResources* resources_panel = nullptr;
	UIHUDPanelInfo* panel_info = nullptr;
	UIHUDPanelButtons* panel_buttons = nullptr;
	UIHUDDescription* hud_description = nullptr;
	UIComponents* info_ui = nullptr;
	UIHUDMinimap* minimap = nullptr;
	UICheckbutton* menu_btn = nullptr;
};
#endif // __j1GUIMANAGER_H__