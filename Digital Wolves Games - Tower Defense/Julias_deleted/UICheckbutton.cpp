#include "UICheckbutton.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "Camera.h"

UICheckbutton::UICheckbutton(UICOMPONENT_TYPE type) : UIComponents(type) {}

void UICheckbutton::Set(const SDL_Rect & position, const SDL_Rect & atlas, const SDL_Rect & atlas_clicked)
{
	rect_position = position;
	rect_atlas = atlas;
	rect_atlas_clicked = atlas_clicked;
}

void UICheckbutton::Draw()
{
	switch (cb_stat)
	{
	case CB_CHECK:
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas_clicked);
		break;
	case CB_UNCHECK:
		App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas);
		break;
	}
}


void UICheckbutton::HandleInput(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_DOWN)
			if (cb_stat == CB_CHECK)
				cb_stat == CB_UNCHECK;
			else
				cb_stat == CB_CHECK;
		break;
	}
}

bool UICheckbutton::Update()
{
	if (IsFocus())
		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_DOWN)
			if (cb_stat == CB_CHECK)
				cb_stat = CB_UNCHECK;
			else
				cb_stat = CB_CHECK;

	Draw();
	return true;
}

void UICheckbutton::SetStat(CHECKBUTTON_STAT cb_stat)
{
	this->cb_stat = cb_stat;
}

const CHECKBUTTON_STAT UICheckbutton::GetStat() const
{
	return cb_stat;
}
