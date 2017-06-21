#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "UIComponents.h"

struct _TTF_Font;

enum LABEL_STAT
{
	LS_NONE,
	LS_MOUSE_ON_TOP
};

class UILabel : public UIComponents
{
private:
	std::string text;
	SDL_Texture* text_img = nullptr;
	_TTF_Font*  font = nullptr;

	LABEL_STAT stat = LS_NONE;
	SDL_Color color_none;
	SDL_Color color_mouse_on_top;
	SDL_Color actualcolor;
public:
	UILabel(UICOMPONENT_TYPE type);

	~UILabel();

	void Set(int pos_x, int pos_y, const char* text, SDL_Color color = { 255,255,255,0 }, _TTF_Font*  font = nullptr);

	void Draw();
	void HandleInput(SDL_Event);
	bool Update();

	//Before check if the text to change is diferent from actual
	void ChangeText(const char* text, SDL_Color color = { 255,255,255,0 });
	void ChangeColor(SDL_Color);
	void SetColorMouseOnTop(SDL_Color);
	void SetLabelStat(LABEL_STAT);
	bool CompareColor(SDL_Color);
	std::string* GetString();
	SDL_Texture* GetTexture();
};

#endif // __UILABEL_H__