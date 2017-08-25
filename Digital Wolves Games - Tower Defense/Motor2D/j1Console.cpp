#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "Camera.h"
#include "j1SceneManager.h"
#include "j1Gui.h"
#include "UI_Text_Input.h"
#include "UI_MultiLabel.h"
#include "UI_Label.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1FileSystem.h"
#include "j1Scene.h"
#include "j1Console.h"

#define CONSOLE_POS { 5, 25 }
#define CONSOLE_INPUT_POS { 333, 333 }
#define CONSOLE_BACKGROUND_IMAGE_RECT { 359, 1579, 200, 200 }
#define NUM_LINES_ON_SCREEN 10
#define CONSOLE_LINE_REMOVED 1

#define FPS_LABEL_POS { 1300, 25 }

j1Console::j1Console() : j1Module()
{
	name.assign("console");
}

j1Console::~j1Console()
{
}

bool j1Console::Awake(pugi::xml_node & config)
{
	bool ret = true;

	config = config.child("command");
	while (config != NULL)
	{
		commands.push_back(Command(config.attribute("text").as_string(), config.attribute("action").as_int()));
		config = config.next_sibling("command");
	}
	
	return ret;
}

bool j1Console::Start()
{
	console_input_text = App->gui->CreateTextInput(CONSOLE_INPUT_POS, "Console input text", OPENSANS_BOLD);
	console_multilabel = App->gui->CreateMultiLabel(CONSOLE_POS, CONSOLE_BACKGROUND_IMAGE_RECT, "Console Output:", NUM_LINES_ON_SCREEN, CONSOLE_LINE_REMOVED, true, true);
	console_multilabel->TurnOff();
	fps_label = App->gui->CreateLabel(FPS_LABEL_POS, CONSOLE_BACKGROUND_IMAGE_RECT, "FPS: ");
	fps_label->TurnOff();
	return true;
}

bool j1Console::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN)
	{
		if(!UnderstandCommand(console_input_text->GetText()))
			LOG("Command wasn't understood");

		console_input_text->DeleteText();
	}
	
	return true;
}

bool j1Console::Update(float dt)
{
	if (show_fps_data)
		ShowCheckFPS();

	if (show_map_data)
	{
		ShowMapData();
		show_map_data = false;
	}

	if (show_mouse_pos_data && data_showing_timer.ReadSec() >= 1)
	{
		ShowMousePosition();
		data_showing_timer.Start();
	}

	return true;
}

bool j1Console::PostUpdate()
{
	if (on == true)
	{
		SDL_RenderDrawRect(App->render->renderer, &rect);
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 70);
		SDL_RenderFillRect(App->render->renderer, &rect);
	}

	return true;
}

bool j1Console::CleanUp()
{
	for (int i = 0; i < text_textures.size(); i++)
		SDL_DestroyTexture(text_textures[i]);
	
	text_textures.clear();

	return true;
}

bool j1Console::PushText(char * text)
{
	if (on)
	{
		console_multilabel->AddLabel(text);
		return true;
	}
	return false;
}

void j1Console::TurnOnOff()
{
	if (on == true)
	{
		SDL_StopTextInput();
		on = false;
		console_multilabel->TurnOff();
		App->gui->SetFocusedText(nullptr);
	}

	else
	{
		SDL_StartTextInput();
		on = true;
		console_multilabel->TurnOn();
		App->gui->SetFocusedText(console_input_text);
	}
}

bool j1Console::IsOn()
{
	return on;
}

UI_TextInput * j1Console::GetConsoleInputText()
{
	return console_input_text;
}

bool j1Console::UnderstandCommand(const char * command)
{
	bool command_understood = false;
	for (int i = 0; i < commands.size(); i++)
	{
		if (command == (commands[i].name))
		{
			command_understood = true;

			switch (commands[i].action)
			{

				//Cheats
			case CREATE_GOD:
				//TODO
				LOG("God Created");
				break;

			case ADD_FOOD:
				App->debug_features.add_food = true;
				LOG("1000 Food added");
				break;

			case ADD_WOOD:
				App->debug_features.add_wood = true;
				LOG("1000 Wood added");
				break;

			case ADD_GOLD:
				App->scene->gold->AddGold(1000);
				LOG("1000 Gold added");
				break;

			case ADD_STONE:
				App->debug_features.add_stone = true;
				LOG("1000 Stone added");
				break;

			case ADD_RESOURCES:
				App->debug_features.add_food = true;
				App->debug_features.add_wood = true;
				App->debug_features.add_gold = true;
				App->debug_features.add_stone = true;
				LOG("1000 of every resource added");
				break;


				//General Information
			case SHOW_FPS:
				show_fps_data = !show_fps_data;
				if (!fps_label->IsOn())
				{
					LOG("Showing FPS info");
					fps_label->TurnOn();
				}
				else
				{
					LOG("Not showing FPS info");
					fps_label->TurnOff();
				}
				break;

			case SHOW_MOUSE_POS_:
				LOG("Showing mouse position info each second");
				show_mouse_pos_data = !show_mouse_pos_data;
				break;

			case SHOW_MAP_INFO:
				LOG("Map info:");
				show_map_data = !show_map_data;
				break;


			case NO_COMMAND:
			default:
				return false;
				break;
			}
		}
	}
	if (command_understood == false)
		return false;
}

void j1Console::ShowCheckFPS()
{
	fps_label->SetText(App->FPSCalculations());
}

void j1Console::ShowMousePosition()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera->GetPosition().x, y - App->render->camera->GetPosition().y);

	static char mouse_tile_pos[256];
	sprintf_s(mouse_tile_pos, 256, "Mouse Pos: %d,%d, Tile:%d,%d", x, y, map_coordinates.x, map_coordinates.y);
	PushText(mouse_tile_pos);
}

void j1Console::ShowMapData()
{
	static char map_data[256];
	sprintf_s(map_data, 256, "Map:%dx%d, Tiles:%dx%d, Tilesets:%d", App->map->data.width, App->map->data.height, App->map->data.tile_width, App->map->data.tile_height, App->map->data.tilesets.size());
	PushText(map_data);
}

Command::Command(const char* command_name, int action_num) : name(command_name), action((COMMAND)action_num)
{
}

Command::Command(const char * command_name, COMMAND action_num) : name(command_name), action(action_num)
{
}

Command::~Command()
{
}
