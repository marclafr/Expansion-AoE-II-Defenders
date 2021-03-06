#ifndef __UI_MULTILABEL_H__
#define __UI_MULTILABEL_H__

#include "j1Gui.h"

class UI_MultiLabel : public UI_Element
{
public:
	UI_MultiLabel(iPoint pos, SDL_Rect atlas_rect, char* txt, int max_labels = 1, int label_to_remove = 0, bool has_max_labels = false, bool has_background = true, bool not_in_world = true);
	~UI_MultiLabel();

	bool Update();
	bool Draw(SDL_Texture* atlas, int alpha = 255);

	void AddLabel(char* txt);

private:
	std::vector<UI_Label*> labels;
	bool has_max_labels = false;
	int max_labels = 1;
	int label_to_remove = 1;

	int CalculateYDisplacement(int num_lines);
	bool EraseLabel(UI_Label* label);
};

#endif //__UI_MULTILABEL_H__