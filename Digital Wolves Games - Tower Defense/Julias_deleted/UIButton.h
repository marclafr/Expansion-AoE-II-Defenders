#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "UIComponents.h"
#include "UILabel.h"
#include "Task.h"

#define UI_PANEL_MARK_MARGIN 2

struct info_button;

enum BUTTON_STAT
{
	BS_NONE,
	BS_CLICKED,
	BS_MOUSE_ON_TOP
};

class UIButton : public UIComponents
{
private:
	BUTTON_STAT state = BS_NONE;

	SDL_Rect atlas_mouse_on_top = { 0,0,0,0 };
	SDL_Rect atlas_clicked = { 0,0,0,0 };

	int displacement = 0;
	uint fx_sound = 0;
	Task* task = nullptr;
	bool delete_task = true;

	bool panel_buttons;

	UILabel* title = nullptr;

public:
	UIButton(UICOMPONENT_TYPE type);
	~UIButton();

	void Set(const SDL_Rect& position, const SDL_Rect& atlas, bool panel_buttons = false);

	void Draw();
	void HandleInput(SDL_Event);
	bool Update();

	const BUTTON_STAT GetStat() const;

	void SetAtlasBtn(const SDL_Rect& rect);
	void SetMouseOnTopTextRect(const SDL_Rect & rect, int displacement = 0);
	void SetClickedTextRect(const SDL_Rect& rect);
	void SetTask(Task* task);
	void SetLabel(UILabel*);
	void SetNotDeleteTask();
	void SetFxSound(uint);
	UILabel* GetLabel() const;
};

#endif // __UIBUTTON_H__