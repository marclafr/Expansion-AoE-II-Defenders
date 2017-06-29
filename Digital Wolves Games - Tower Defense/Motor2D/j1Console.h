#ifndef _CONSOLE
#define _CONSOLE

#include <vector>
#include "SDL\include\SDL.h"
#include "j1Module.h"
#include "p2Point.h"

#define RECT_WIDHT 375
#define RECT_HEIGHT 130

class UI_TextInput;

enum COMMAND
{
	NO_COMMAND = 0,
	CREATE_GOD = 1
};

struct Command
{
	Command(const char* command_name, int action_num);
	Command(const char* command_name, COMMAND action_num);
	~Command();

	std::string name;
	COMMAND action;
};

class j1Console : public j1Module
{
private:
	SDL_Rect rect {0,0,RECT_WIDHT,RECT_HEIGHT };
	char * text;
	std::vector<SDL_Texture*> text_textures;
	bool on = false;
	UI_TextInput* console_input_text;
	std::vector<Command> commands;

public:

	j1Console();
	~j1Console();
	
	bool Awake(pugi::xml_node& config);	

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
