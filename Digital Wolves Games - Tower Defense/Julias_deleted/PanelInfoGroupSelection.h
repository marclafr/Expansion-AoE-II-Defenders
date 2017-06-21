#ifndef _PANELINFO
#define _PANELINFO

#include "PanelInfoBasicStructure.h"
#include "UIButton.h"

class GroupSelection : public PanelInfo
{
private:
	struct entity_selected
	{
		UIButton* btn_selected = nullptr;

		Entity* pointer_entity = nullptr;
	};

	std::vector<entity_selected*> es_selection;
	SDL_Rect life_bar;

public:
	GroupSelection() : PanelInfo(GROUPSELECTION) {};

	~GroupSelection();

	void Prepare();
	void PrepareUnitSelection();
	void PrepareNoUnitSelection();

	void Update();
	void Draw();

	void SetOneEntitySelection(Entity* e_selected);
};

#endif