#include "j1Mouse.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"

j1Mouse::j1Mouse() : j1Module()
{
	name.assign("mouse");
}

j1Mouse::~j1Mouse()
{
}

bool j1Mouse::Start()
{
	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

bool j1Mouse::Update(float dt)
{
	if (App->scene->placing_wall == true || App->scene->placing_tower != T_NO_TYPE)
	{
		icon_rect = { 24,0,35,35 };
		pivot_x = 6;
		pivot_y = 9;
	}
	else
	{
		icon_rect = { 0,0,24,32 };
		pivot_x = 0;
		pivot_y = 0;
	}
	App->render->SetMouseIconRect(icon_rect, pivot_x, pivot_y);
	return true;
}
