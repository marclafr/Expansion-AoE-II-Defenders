#ifndef __UI_PANNEL_INFO_H__
#define __UI_PANNEL_INFO_H__

#include "j1Gui.h"
class Task;

#define MARGIN 5 //pixels
#define BUTTON_WIDTH 30 //pixels
#define BUTTON_HEIGHT 45 //pixels

class UI_PanelButtons : public UI_Element
{
public:
	UI_PanelButtons(iPoint pos, SDL_Rect panel_background_rect, Task* task, SDL_Rect rect_idle, int num_buttons_width, SDL_Rect rect_mouse_on_top = { 0,0,0,0 } , SDL_Rect rect_clicking = { 0,0,0,0 }, char* description = nullptr, SDL_Rect description_background_rect = { 0,0,0,0 });
	~UI_PanelButtons();

	bool Update();
	bool Draw(SDL_Texture* atlas, int alpha = 255);

	void AddButton(Task* task, SDL_Rect rect_idle, SDL_Rect rect_mouse_on_top = { 0,0,0,0 }, SDL_Rect rect_clicking = { 0,0,0,0 }, char* description = nullptr, SDL_Rect description_background_rect = { 0,0,0,0 });

private:
	int num_buttons_width;
	std::vector<UI_Button*> panel_buttons;
	std::vector<Task*> tasks;
};

class UI_PanelInfoSingleEntity : public UI_Element
{
public:
	UI_PanelInfoSingleEntity(iPoint pos, SDL_Rect panel_background_rect, Entity* entity);
	~UI_PanelInfoSingleEntity();

	bool Draw(SDL_Texture* atlas, int alpha = 255);

private:
	UI_Image* entity_icon;
	UI_Image* attack_icon;
	UI_Image* armor_icon;
	UI_Image* range_icon;
	UI_Label* attack_label;
	UI_Label* armor_label;
	UI_Label* range_label;

	std::string attribute_value_attack;
	std::string attribute_value_armor;
	std::string attribute_value_range;
};


class UI_PanelInfoMultipleEntities : public UI_Element
{
public:
	UI_PanelInfoMultipleEntities(iPoint pos, SDL_Rect panel_background_rect, std::vector<Entity*> selection);
	~UI_PanelInfoMultipleEntities();

	bool Update();
	bool Draw(SDL_Texture* atlas, int alpha = 255);

private:
	std::vector<UI_Button*> panel_buttons;
	std::vector<UI_HPBar*> panel_hpbars;
};

#endif //__UI_PANNEL_INFO_H__