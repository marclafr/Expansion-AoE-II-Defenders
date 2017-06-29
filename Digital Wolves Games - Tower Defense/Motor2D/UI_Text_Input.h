#ifndef __UI_TEXT_INPUT_H__
#define __UI_TEXT_INPUT_H__

#include "j1Gui.h"
#include "j1Timer.h"

class _TTF_Font;

class UI_TextInput : public UI_Element
{
public:
	UI_TextInput(iPoint pos, std::string text, FONT_NAME font_name = OPENSANS_REGULAR, SDL_Color color = { (0), (0), (0), (255) }, bool not_in_world = true);
	~UI_TextInput();

	bool SetFont(FONT_NAME name);
	void InputTextChanged();

private:
	std::string text;
	SDL_Rect text_atlas_rect;
	_TTF_Font* font;
	SDL_Texture* tex;
	SDL_Color color = { (0), (0), (0), (255) };
	bool text_changed = false;
	int writing_pos = 0; // character number we are writing on
	j1Timer writing_pos_appear_timer;
	j1Timer repeat_timer;

	bool Update();
	bool Draw(SDL_Texture* atlas);

	bool MouseInsideText();
	void DeleteChar();
};

#endif //__UI_TEXT_INPUT_H__