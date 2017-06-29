#ifndef _CONSOLE
#define _CONSOLE

#include <vector>
#include "SDL\include\SDL.h"
#include "j1Module.h"
#include "p2Point.h"

#define RECT_WIDHT 375
#define RECT_HEIGHT 130

class UI_TextInput;

class j1Console : public j1Module
{
private:
	SDL_Rect rect {0,0,RECT_WIDHT,RECT_HEIGHT };
	char * text;
	std::vector<SDL_Texture*> text_textures;
	bool on = false;
	UI_TextInput* console_input_text;

public:
	// Called before the first frame
	virtual bool Start();

	// Called each loop iteration
	virtual bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool PostUpdate();

	// Called before quitting
	virtual bool CleanUp();

	bool PushText(char* text);
	void BlitText();

	void TurnOnOff();
	UI_TextInput* GetConsoleInputText();
};



#endif
