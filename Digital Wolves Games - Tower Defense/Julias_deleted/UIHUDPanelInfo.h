#ifndef __UIHUDPANELINFO_H__
#define __UIHUDPANELINFO_H__

#include "PanelInfoBasicStructure.h"
#include "UIComponents.h"

#include <list>

class Entity;

class UIHUDPanelInfo : public UIComponents
{
private:
	//Actual panel info
	PanelInfo* actual_panelinfo = nullptr;

public:
	UIHUDPanelInfo(UICOMPONENT_TYPE type);
	~UIHUDPanelInfo();

	//Main Functions of list fill o remove
	void DeletePanelInfo();

	//Prepare the Panel Info structure
	void CreatePanel(const std::vector<Entity*>& selection);

	bool Update();
};

#endif // __UIHUDPANELINFO_H__