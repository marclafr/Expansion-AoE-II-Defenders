#include "UIButton.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "Camera.h"
#include "j1Scene.h"
#include "j1Audio.h"

#include "UIHUDPanelButtons.h"
#include "UIHUDDescription.h"

UIButton::UIButton(UICOMPONENT_TYPE type) : UIComponents(type)
{}

UIButton::~UIButton()
{
	if(delete_task)
		DELETE_PTR(task);
}

void UIButton::Set(const SDL_Rect & position, const SDL_Rect & atlas, bool panel_buttons)
{
	rect_position = position;
	rect_atlas = atlas;
	atlas_clicked = atlas;
	atlas_mouse_on_top = atlas;
	this->panel_buttons = panel_buttons;
}

void UIButton::Draw()
{
	SDL_Texture* atlas = (SDL_Texture*)App->uimanager->GetAtlas();
	SDL_Rect rect(GetPosRect());
	rect.x -= App->render->camera->GetPosition().x;
	rect.y -= App->render->camera->GetPosition().y;

	switch (state)
	{
	case BS_NONE:
		App->render->PushUISprite(atlas, rect.x, rect.y, &GetAtlasRect());
		if (panel_buttons)
		{
			SDL_Rect mark_btn{ 969, 827, 29, 29 };
			App->render->PushUISprite(atlas, rect.x - UI_PANEL_MARK_MARGIN, rect.y - UI_PANEL_MARK_MARGIN, &mark_btn);
		}
		break;
	case BS_CLICKED:
		App->render->PushUISprite(atlas, rect.x - displacement, rect.y - displacement, &atlas_clicked);
		if (panel_buttons)
		{
			SDL_Rect mark_btn{ 1092, 827, 29, 29 };
			App->render->PushUISprite(atlas, rect.x - UI_PANEL_MARK_MARGIN, rect.y - UI_PANEL_MARK_MARGIN, &mark_btn);
		}
		break;
	case BS_MOUSE_ON_TOP:
		App->render->PushUISprite(atlas, rect.x - displacement, rect.y - displacement, &atlas_mouse_on_top);
		if (panel_buttons)
		{
			SDL_Rect mark_btn{ 969, 827, 29, 29 };
			App->render->PushUISprite(atlas, rect.x - UI_PANEL_MARK_MARGIN, rect.y - UI_PANEL_MARK_MARGIN, &mark_btn);
		}
		break;
	}
}

void UIButton::HandleInput(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == MK_LEFT)
			state = BS_CLICKED;
		break;

	case SDL_MOUSEBUTTONUP:
		if (event.button.button == MK_LEFT)
		{
			state = BS_MOUSE_ON_TOP;
			task->Execute();
		}
		break;
	}
}

bool UIButton::Update()
{
	if (IsFocus() && (state != BS_CLICKED))
	{
		state = BS_MOUSE_ON_TOP;
		if(title != nullptr)
			title->SetLabelStat(LS_MOUSE_ON_TOP);
	}
	else if (!IsFocus())
	{
		state = BS_NONE;
		if (title != nullptr)
			title->SetLabelStat(LS_NONE);
	}

	if (IsFocus())
		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_DOWN)
			state = BS_CLICKED;
		else if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_UP)
		{
			state = BS_MOUSE_ON_TOP;
			if (fx_sound != 0)
				App->audio->PlayFx(fx_sound);
			if (task != nullptr)
				task->Execute();
		}

	Draw();

	return true;
}

const BUTTON_STAT UIButton::GetStat() const
{
	return state;
}

void UIButton::SetAtlasBtn(const SDL_Rect & rect)
{
	if (SDL_RectEquals(&atlas_clicked, &rect_atlas) == SDL_TRUE) { atlas_clicked = rect; }
	if (SDL_RectEquals(&atlas_mouse_on_top, &rect_atlas) == SDL_TRUE) { atlas_mouse_on_top = rect; }
	rect_atlas = rect;
}

void UIButton::SetMouseOnTopTextRect(const SDL_Rect & rect, int displacement)
{
	atlas_mouse_on_top = rect;
	this->displacement = displacement;
}

void UIButton::SetClickedTextRect(const SDL_Rect & rect)
{
	atlas_clicked = rect;
}

void UIButton::SetTask(Task* task)
{
	this->task = task;
}

void UIButton::SetLabel(UILabel * label)
{
	title = label;
}

void UIButton::SetNotDeleteTask()
{
	delete_task = false;
}

void UIButton::SetFxSound(uint fx)
{
	fx_sound = fx;
}

UILabel * UIButton::GetLabel() const
{
	return title;
}
