#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "Camera.h"
#include "j1SceneManager.h"
#include "j1Gui.h"
#include "UI_Text_Input.h"
#include "UI_MultiLabel.h"
#include "j1Input.h"
#include "j1FileSystem.h"
#include "p2Log.h"
#include "j1Console.h"

#define CONSOLE_POS { 5, 25 }
#define CONSOLE_INPUT_POS { 333, 333 }
#define CONSOLE_BACKGROUND_IMAGE_RECT { 0, 0, 200, 200 }
#define NUM_LINES_ON_SCREEN 5
#define CONSOLE_LINE_REMOVED 2

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
	return true;
}

bool j1Console::PostUpdate()
{
	if (on == true)
	{
		SDL_RenderDrawRect(App->render->renderer, &rect);
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 100);
		SDL_RenderFillRect(App->render->renderer, &rect);
		BlitText();
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
		//text_textures.push_back(App->font->Print(text));
		console_multilabel->AddLabel(text);
		return true;
	}
	return false;
}

void j1Console::BlitText()
{
	/*int x = -App->render->camera->GetPosition().x + 5;;
	int y;

	if (App->scene_manager->GetCurrentScene() == SC_GAME)
		y = -App->render->camera->GetPosition().y + 20;
	else
		y = -App->render->camera->GetPosition().y + 3;


	int height = 0;
	int width = 0;
	Uint32* format = 0;
	int* acces = 0;

	for (int i = 0; i < text_textures.size(); i++)
	{
		SDL_QueryTexture(text_textures[i], format, acces, &width, &height);
		App->render->Blit(text_textures[i], x, y);
		y += height;
	}*/
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
				App->debug_features.add_gold = true;
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

			case NO_COMMAND:
			default:
				return false;
				break;
			}
		}
	}
	if (command_understood == false)
		return false;

	/*
	static char mouse_pos[256];
	sprintf_s(mouse_pos, 256, "Mouse Pos: %d,%d", x, y);
	App->console->PushText(mouse_pos);

	static char map[256];
	sprintf_s(map, 256, "Map:%dx%d", App->map->data.width, App->map->data.height);
	App->console->PushText(map);

	static char tiles[256];
	sprintf_s(tiles, 256, "Tiles:%dx%d", App->map->data.tile_width, App->map->data.tile_height);
	App->console->PushText(tiles);

	static char tilesets[256];
	sprintf_s(tilesets, 256, "Tilesets:%d", App->map->data.tilesets.size());
	App->console->PushText(tilesets);

	static char tile[256];
	sprintf_s(tile, 256, "Tile:%d,%d", map_coordinates.x, map_coordinates.y);
	App->console->PushText(tile);

	static char current_frame[256];
	sprintf_s(current_frame, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f", avg_fps, last_frame_ms, frames_on_last_update, dt);
	App->console->PushText(current_frame);

	static char scince_startup[256];
	sprintf_s(scince_startup, 256, "Time since startup: %.3f Frame Count: %lu ", seconds_since_startup, frame_count);
	App->console->PushText(scince_startup);
	*/

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
