#ifndef _CONSOLE
#define _CONSOLE

#include <vector>
#include "SDL\include\SDL.h"
#include "j1Module.h"
#include "p2Point.h"

#define RECT_WIDHT 375
#define RECT_HEIGHT 130

class UI_TextInput;
class UI_MultiLabel;
class UI_Label;

enum COMMAND
{
	NO_COMMAND = 0,
	CREATE_GOD = 1,
	ADD_FOOD = 2,
	ADD_WOOD = 3,
	ADD_GOLD = 4,
	ADD_STONE = 5,
	ADD_RESOURCES = 6,

	SHOW_FPS = 7,
	SHOW_MOUSE_POS_ = 8,
	SHOW_MAP_INFO = 9
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

	bool UnderstandCommand(const char* command);

	bool show_fps_data = false;
	void ShowCheckFPS();
	bool show_mouse_pos_data = false;
	void ShowMousePosition();
	bool show_map_data = false;
	void ShowMapData();


	UI_MultiLabel* console_multilabel;
	UI_Label* fps_label;

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
	bool IsOn();
	UI_TextInput* GetConsoleInputText();
};



#endif
