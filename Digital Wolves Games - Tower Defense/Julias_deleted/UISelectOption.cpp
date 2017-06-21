#include "UISelectOption.h"

#include "p2Defs.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1UIManager.h"
#include "Camera.h"

#include "UILabel.h"

UISelectOption::UISelectOption(UICOMPONENT_TYPE type) : UIComponents(type) {}

void UISelectOption::Set(int pos_x, int pos_y, int pos_w, int pos_h, uint atlas_x, uint atlas_y, uint atlas_w, uint atlas_h)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;
	rect_position.w = pos_w;
	rect_position.h = pos_h;

	rect_atlas.x = atlas_x;
	rect_atlas.y = atlas_y;
	rect_atlas.w = atlas_w;
	rect_atlas.h = atlas_h;
}

void UISelectOption::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

void UISelectOption::Draw()
{
	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas);

	if (selecting)
	{
		for (uint i = 1; i < num_options + 1; i++)
			App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, (rect_position.y - App->render->camera->GetPosition().y) + rect_atlas.h * i, &rect_atlas);
	}
}

bool UISelectOption::Update()
{
	/*
	switch (GetStat())
	{
	//Expand options from UISelectOption
	case CLICKL_DOWN:
	selecting = true;
	ChangeDrawAllOptions();
	break;
	}
	Draw();
	*/
	return true;
}

void UISelectOption::AddOption(const char * text)
{
	UILabel* newoption_label = App->uimanager->AddLabel(rect_position.x + 2, (rect_position.y + (rect_position.h / 4)) + (num_options*rect_position.h), text);

	num_options++;

	UIComponents* newoptions_component = (UIComponents*)newoption_label;

	//newoptions_component->SetDraw(false);

	newoptions_component->SetFrom(this);

	if (current == nullptr)
		ChangeCurrent(newoption_label);
	else
		options.push_back(newoption_label);
}

const char * UISelectOption::CheckSelected()
{
	return current->GetString()->c_str();
}

void UISelectOption::ChangeCurrent(UILabel * change)
{
	if (current == nullptr)
	{
		current = change;
		//current->GetPosRect().x = rect_position.x;
		//current->GetPosRect().y = rect_position.y;
		//current->SetDraw(true);
	}
	else
	{
		std::list<UILabel*>::iterator item = options.begin();

		while (item._Ptr->_Myval != change)
		{
			item++;
		}

		options.erase(item);

		//SWAP<int>(current->GetPosRect().x, change->GetPosRect().x);
		//SWAP<int>(current->GetPosRect().y, change->GetPosRect().y);

		options.push_back(current);


		current = change;
		//current->SetDraw(true);
	}
}

void UISelectOption::ChangeDrawAllOptions()
{
	std::list<UILabel*>::iterator item;
	item = options.begin();

	while (item != options.end())
	{
		UIComponents* draw_label = (UIComponents*)item._Ptr->_Myval;
		/*
		if (draw_label->GetDraw())
		draw_label->SetDraw(false);
		else
		draw_label->SetDraw(true);
		*/
		item++;
	}
}