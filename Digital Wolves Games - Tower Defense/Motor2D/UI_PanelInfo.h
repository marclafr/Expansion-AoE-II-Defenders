#ifndef __UI_PANNEL_INFO_H__
#define __UI_PANNEL_INFO_H__

#include "j1Gui.h"

class UI_PanelInfoSingleEntity : public UI_Element
{
public:
	UI_PanelInfoSingleEntity(iPoint pos, SDL_Rect panel_background_rect, Entity* entity);
	~UI_PanelInfoSingleEntity();

	bool Draw(SDL_Texture* atlas);

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

private:
	std::vector<UI_Button*> panel_buttons;
	std::vector<UI_Label*> panel_labels;
};

#endif //__UI_PANNEL_INFO_H__