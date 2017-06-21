#pragma once
#ifndef __UICOMPONENTS_H__
#define ___UICOMPONENTS_H__

#include "p2Defs.h"
#include "j1App.h"
#include "j1UIManager.h"
#include "SDL\include\SDL.h"

enum UICOMPONENT_TYPE
{
	UIT_UILABEL,
	UIT_UIBUTTON,
	UIT_UICHECKBUTTON,
	UIT_UISELECTOPTION,
	UIT_UIHUDPANELBUTTONS,
	UIT_UIHUDPANELINFO,
	UIT_UIIMAGE,
	UIT_UIHUDDESCRIPTION,
	UIT_UIHUDRESOURCES,
	UIT_UIHUDTOWNHALLBARLIFE,
	UIT_UIHUDSCOREBAR,
	UIT_UIHUDMINIMAP,
	UIT_UIHUDMENUINGAME
};

class UIComponents
{
protected:
	bool interactive = true;
	bool is_focus = false;
	bool to_delete = false;
	UIComponents* from = nullptr;
	SDL_Rect rect_position;
	SDL_Rect rect_atlas;
	UICOMPONENT_TYPE type;

public:
	UIComponents(UICOMPONENT_TYPE type);

	virtual ~UIComponents();

	virtual void Set(const SDL_Rect& position, const SDL_Rect& atlas);

	virtual void Draw();
	virtual void HandleInput(SDL_Event);
	virtual bool Update();

	const bool GetInteractive() const;
	const bool IsFocus() const;
	const SDL_Rect GetAtlasRect() const;
	const SDL_Rect GetPosRect() const;
	const bool ToDelete() const;
	const UICOMPONENT_TYPE GetType() const;
	const UIComponents* GetFrom()const;
	const UIComponents* GetThisComponent() const;
	void SetInteractive(bool interactive);
	void SetIsFocus(bool is_focus);
	void SetFrom(UIComponents* from);
	void SetToDelete();
	void SetAtlas(const SDL_Rect& atlas);
	void SetPos(const SDL_Rect& pos);
};

#endif // __UICOMPONENTS_H__