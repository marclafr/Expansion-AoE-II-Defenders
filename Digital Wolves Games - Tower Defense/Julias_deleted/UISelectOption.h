#ifndef __UISELECTOPTION_H__
#define __UISELECTOPTION_H__

#include "UIComponents.h"
#include "UILabel.h"
#include <list>

class UISelectOption : public UIComponents
{
public:
	UILabel* title;

	UILabel* current = nullptr;
	std::list<UILabel*> options;

	bool selecting = false;

	int num_options = -1;

public:
	UISelectOption(UICOMPONENT_TYPE type);

	void Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h);
	void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	void Draw();

	bool Update();

	void AddOption(const char* text);
	const char* CheckSelected();
	void ChangeCurrent(UILabel* change);

	void ChangeDrawAllOptions();
};

#endif // __UISELECTOPTION_H__