#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "Camera.h"
#include "j1Console.h"


bool j1Console::Start()
{
	return true;
}

bool j1Console::PreUpdate()
{
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
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(App->render->renderer, &rect);
		BlitText();
	}

	for (int i = 0; i < text_textures.size(); i++)
	{
		SDL_DestroyTexture(text_textures[i]);
	}

	text_textures.clear();

	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

void j1Console::PushText(char * text)
{
	text_textures.push_back(App->font->Print(text));
}

void j1Console::BlitText()
{
	int x = - App->render->camera->GetPosition().x + 5;
	int y = - App->render->camera->GetPosition().y + 5;
	int height = 0;
	int width = 0;
	Uint32* format = 0;
	int* acces = 0;

	for (int i = 0; i < text_textures.size(); i++)
	{
		SDL_QueryTexture(text_textures[i], format, acces, &width, &height);
		App->render->Blit(text_textures[i],x,y);
		y += height;
	}
}

void j1Console::TurnOnOff()
{
	if (on == true)
		on = false;
	else
		on = true;
}
