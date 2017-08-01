#include "UI_Label.h"
#include "UI_MultiLabel.h"

#define TEXT_HEIGHT_DISPLACEMENT 5

UI_MultiLabel::UI_MultiLabel(iPoint pos, SDL_Rect atlas_rect, char * txt, int max_labels, int label_to_remove, bool has_max_labels, bool has_background, bool not_in_world) : UI_Element(UI_E_MULTILABEL, pos, atlas_rect, not_in_world), max_labels(max_labels), label_to_remove(label_to_remove), has_max_labels(has_max_labels)
{
	labels.push_back(new UI_Label(pos, atlas_rect, txt, has_background, not_in_world));
}

UI_MultiLabel::~UI_MultiLabel()
{
	for (int i = 0; i < labels.size(); i++)
		DELETE_PTR(labels[i]);
}

bool UI_MultiLabel::Update()
{


	return true;
}

bool UI_MultiLabel::Draw(SDL_Texture * atlas)
{
	if (showing)
		for (int i = 0; i < labels.size(); i++)
			labels[i]->Draw(atlas);
	
	return true;
}

void UI_MultiLabel::AddLabel(char * txt)
{
	iPoint correct_pos = pos;
	int y_displacement = 0;

	if (has_max_labels && labels.size() >= max_labels)
	{
		int all_displacement_y = labels[label_to_remove]->GetTextHeight();
		
		for (int i = labels.size() - 2; i >= label_to_remove; i--)
		{
			//TODO:
			//labels[i - 1]->SetY(labels[i - 1]->GetY() - all_displacement_y);
			int y_pos = labels[i]->GetY();
			labels[i] = labels[i + 1];
			labels[i]->SetY(y_pos);
		}
 		EraseLastLabel();
	}

	y_displacement = CalculateYDisplacement(labels.size());

	correct_pos.y += y_displacement + (TEXT_HEIGHT_DISPLACEMENT * labels.size());
	labels.push_back(new UI_Label(correct_pos, atlas_rect, txt, labels[0]->HasBackground(), not_in_world));
}

int UI_MultiLabel::CalculateYDisplacement(int num_lines)
{
	int y_displacement = 0;
	for (int i = 0; i < num_lines; i++)
		y_displacement += labels[i]->GetTextHeight();
	
	return y_displacement;
}

bool UI_MultiLabel::EraseLastLabel()
{
	if (labels.size() > 0)
	{
		std::vector<UI_Label*>::iterator it = labels.end();
		DELETE_PTR(labels[labels.size() - 1]);
		labels.erase(--it);
		return true;
	}
	return false;
}