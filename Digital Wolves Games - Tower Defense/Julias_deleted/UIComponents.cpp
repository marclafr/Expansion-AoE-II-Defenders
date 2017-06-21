#include "UIComponents.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1UIManager.h"
#include "Camera.h"
#include "SDL/include/SDL.h"

UIComponents::UIComponents(UICOMPONENT_TYPE type)
{
	this->type = type;
}

UIComponents::~UIComponents() {}

void UIComponents::Set(const SDL_Rect & position, const SDL_Rect & atlas)
{
	rect_position = position;
	rect_atlas = atlas;
}

void UIComponents::Draw()
{
	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas);
}

bool UIComponents::Update()
{
	Draw();
	return true;
}

void UIComponents::HandleInput(SDL_Event event) {}

void UIComponents::SetInteractive(bool interactive)
{
	this->interactive = interactive;
}

void UIComponents::SetIsFocus(bool is_focus)
{
	this->is_focus = is_focus;
}

void UIComponents::SetFrom(UIComponents * from)
{
	this->from = from;
}

void UIComponents::SetToDelete()
{
	this->to_delete = true;
}

void UIComponents::SetAtlas(const SDL_Rect & atlas)
{
	rect_atlas = atlas;
}

void UIComponents::SetPos(const SDL_Rect & pos)
{
	rect_position = pos;
}

const bool UIComponents::GetInteractive() const
{
	return interactive;
}

const bool UIComponents::IsFocus() const
{
	return is_focus;
}

const SDL_Rect UIComponents::GetAtlasRect() const
{
	return rect_atlas;
}

const SDL_Rect UIComponents::GetPosRect() const
{
	return rect_position;
}

const bool UIComponents::ToDelete() const
{
	return to_delete;
}

const UICOMPONENT_TYPE UIComponents::GetType() const
{
	return type;
}

const UIComponents* UIComponents::GetFrom() const
{
	return from;
}

const UIComponents * UIComponents::GetThisComponent() const
{
	return this;
}