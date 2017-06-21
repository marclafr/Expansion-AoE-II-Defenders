#ifndef __UIHUDRESORCES_H__
#define __UIHUDRESORCES_H__

#include "UIComponents.h"

#include "UILabel.h"

class Resources;

enum RESOURCE_TYPE;

class UIHUDResources : public UIComponents
{
private:
	UILabel* stone;
	UILabel* gold;
	UILabel* food;
	UILabel* wood;

public:
	UIHUDResources(UICOMPONENT_TYPE type);

	bool Update();
};

#endif // __UIHUDRESORCES_H__