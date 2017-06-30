#ifndef __UI_PANNEL_INFO_H__
#define __UI_PANNEL_INFO_H__

#include "j1Gui.h"

class UI_PanelInfo : UI_Element
{
private:
	std::vector<UI_Element*> panel_elements;

public:
	UI_PanelInfo();
	~UI_PanelInfo();
	void PushBack(UI_Element* element);
};

#endif //__UI_PANNEL_INFO_H__