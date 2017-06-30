#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "Camera.h"
#include "j1SceneManager.h"
#include "j1Gui.h"
#include "UI_Text_Input.h"
#include "j1Input.h"
#include "j1FileSystem.h"
#include "p2Log.h"
#include "j1Console.h"

#define CONSOLE_INPUT_POS { 333, 333 }

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
	return true;
}

bool j1Console::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RETURN2) == KEY_DOWN)
	{
		std::string input = console_input_text->GetText();
		console_input_text->DeleteText();
		bool command_understood = false;
		for (int i = 0; i < commands.size(); i++)
		{
			if (input.compare(commands[i].name) == 0)
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
					LOG("Command wasn't understood");
					break;
				}
			}
		}
		if (!command_understood)
			LOG("Command wasn't understood");
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
	return true;
}

bool j1Console::PushText(char * text)
{
	if (on)
	{
		text_textures.push_back(App->font->Print(text));
		return true;
	}
	return false;
}

void j1Console::BlitText()
{
	int x = -App->render->camera->GetPosition().x + 5;;
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
		
		SDL_DestroyTexture(text_textures[i]);
	}

	text_textures.clear();
}

void j1Console::TurnOnOff()
{
	if (on == true)
	{
		SDL_StopTextInput();
		on = false;
		App->gui->SetFocusedText(nullptr);
	}

	else
	{
		SDL_StartTextInput();
		on = true;
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

Command::Command(const char* command_name, int action_num) : name(command_name), action((COMMAND)action_num)
{
}

Command::Command(const char * command_name, COMMAND action_num) : name(command_name), action(action_num)
{
}

Command::~Command()
{
}
