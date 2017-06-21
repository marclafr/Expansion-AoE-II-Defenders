#pragma once

#ifndef __UIHUMENUINGAME_H__
#define __UIHUMENUINGAME_H__

#include "UIComponents.h"
#include "p2Point.h"

#define MAX_BUTTONS 6

class Task;
class UIButton;

struct button_menuingame
{
private:
	uint position;
	std::string title;
	UIButton* btn;
	Task* task;

public:
	button_menuingame(uint position, const char* title, Task* task) : position(position), task(task) { this->title.assign(title); }
	~button_menuingame()
	{
		DELETE_PTR(task);
	}

	void CreateButton();
	void ButtonToDelete();
};

class UIHUDMenuInGame : public UIComponents
{
private:
	UICheckbutton* enable;

	UIComponents* background;
	std::vector<button_menuingame*> buttons;

	bool deleted = true;

public:
	UIHUDMenuInGame(UICOMPONENT_TYPE type = UIT_UIHUDMENUINGAME) : UIComponents(type) { SetInteractive(false); }
	~UIHUDMenuInGame();

	void SetEnableButton(UICheckbutton* enable);

	//Max 7 buttons
	void AddButton(uint pos, const char* title, Task* task);

	bool Update();

	void CreatePanel();
	void DeletePanel();
};


#endif // __UIHUMENUINGAME_H__