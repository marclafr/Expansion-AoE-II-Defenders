#include "j1App.h"
#include "j1UIManager.h"
#include "UIButton.h"
#include "UICheckbutton.h"
#include "UIHUDMenuInGame.h"

#define WINDOW_POSITION_X 535
#define WINDOW_POSITION_Y 233
#define POSITION_WINDOW {WINDOW_POSITION_X,WINDOW_POSITION_Y,297,302}
#define ATLAS_WINDOW {0,1162,297,302}

#define BUTTON_POSITION_X 38
#define BUTTON_POSITION_Y 35
#define BUTTON_POSITION_W 220
#define BUTTON_POSITION_H 30
#define BUTTON_MARGIN 10
#define ATLAS_BUTTON {921,2693,BUTTON_POSITION_W,BUTTON_POSITION_H}

#define LABEL_POSITION_X 20
#define LABEL_POSITION_Y 5

UIHUDMenuInGame::~UIHUDMenuInGame()
{
	if(deleted == false)
		DeletePanel();
}

void UIHUDMenuInGame::SetEnableButton(UICheckbutton * enable)
{
	this->enable = enable;
}

void UIHUDMenuInGame::AddButton(uint pos, const char * title, Task * task)
{
	if (buttons.size() <= MAX_BUTTONS)
	{
		button_menuingame* new_button = new button_menuingame(pos, title, task);

		buttons.push_back(new_button);
	}
}

bool UIHUDMenuInGame::Update()
{
	if (enable->GetStat() == CB_CHECK && deleted == true)
		CreatePanel();
	else if (enable->GetStat() == CB_UNCHECK && deleted == false)
		DeletePanel();
	return true;
}

void UIHUDMenuInGame::CreatePanel()
{
	background = App->uimanager->AddComponent(UIT_UIIMAGE, POSITION_WINDOW, ATLAS_WINDOW);
	for (std::vector<button_menuingame*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
		(*it)->CreateButton();
	enable->SetStat(CB_CHECK);
	deleted = false;
}

void UIHUDMenuInGame::DeletePanel()
{
	background->SetToDelete();
	for (std::vector<button_menuingame*>::iterator it = buttons.begin(); it != buttons.end(); ++it)
		(*it)->ButtonToDelete();
	enable->SetStat(CB_UNCHECK);
	deleted = true;
}

void button_menuingame::CreateButton()
{
	btn = App->uimanager->AddButton(
	{ BUTTON_POSITION_X + WINDOW_POSITION_X, WINDOW_POSITION_Y + BUTTON_POSITION_Y + ((BUTTON_MARGIN + BUTTON_POSITION_H) * (int)position),BUTTON_POSITION_W, BUTTON_POSITION_H },
	ATLAS_BUTTON
	);
	btn->SetTask(task);
	btn->SetNotDeleteTask();
	btn->SetLabel(App->uimanager->AddLabel(btn->GetPosRect().x + LABEL_POSITION_X, btn->GetPosRect().y + LABEL_POSITION_Y, title.c_str()));
}

void button_menuingame::ButtonToDelete()
{
	btn->SetToDelete();
	btn->GetLabel()->SetToDelete();
}