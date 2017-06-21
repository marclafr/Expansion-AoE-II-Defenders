#include <deque>

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Map.h"
#include "Entity.h"
#include "j1Animation.h"
#include "j1EntityManager.h"
//#include "j1UIManager.h"
#include "Camera.h"
#include "j1Scene.h"
#include "j1Input.h"

#define VSYNC true

j1Render::j1Render() : j1Module(), camera(nullptr)
{
	name.assign("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera = new Camera(SDL_Rect{0, 0, App->win->screen_surface->w, App->win->screen_surface->h});
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	camera->UpdateCamera();
	return true;
}

bool j1Render::PostUpdate()
{
	camera->UpdateTransitions();

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	delete camera; 

	LOG("Destroying SDL render");
	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera->SetPosition(iPoint(data.child("camera").attribute("x").as_int(), data.child("camera").attribute("y").as_int()));
	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera->GetPosition().x;
	cam.append_attribute("y") = camera->GetPosition().y;

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::GetDimensionsFromTexture(SDL_Texture* tex, int& w, int& h)
{
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
}


void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint j1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera->GetPosition().x / scale);
	ret.y = (y - camera->GetPosition().y / scale);

	return ret;
}

fPoint j1Render::ScreenToWorldf(int x, int y) const
{
	fPoint ret;
	int scale = App->win->GetScale();

	ret.x = float(x - camera->GetPosition().x / scale);
	ret.y = float(y - camera->GetPosition().y / scale);

	return ret;
}
iPoint j1Render::WorldToScreen(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x + camera->GetPosition().x / scale);
	ret.y = (y + camera->GetPosition().y / scale);

	return ret;
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle, bool not_in_world) const
{
	bool ret = true;

	SDL_Rect rect;
	rect.x = (int)(camera->GetPosition().x * speed) + x;
	rect.y = (int)(camera->GetPosition().y * speed) + y;

	if (section != nullptr)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	if (flip == SDL_FLIP_HORIZONTAL)
	{
		rect.x -= (rect.w - pivot_x);
		rect.y -= pivot_y;
	}

	else if (flip == SDL_FLIP_VERTICAL)
	{
		rect.x -= pivot_x;
		rect.y -= (rect.h - pivot_y);
	}

	else if (flip == SDL_FLIP_NONE)
	{		
		rect.x -= pivot_x;
		rect.y -= pivot_y;
	}

	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
	}
	
	//SDL_SetTextureAlphaMod(texture, App->render->camera->GetOpacity());

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, &pivot, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera->GetPosition().x + rect.x * scale);
		rec.y = (int)(camera->GetPosition().y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera->GetPosition().x + x1 * scale, camera->GetPosition().y + y1 * scale, camera->GetPosition().x + x2 * scale, camera->GetPosition().y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = ((float)M_PI / 180.0f);

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawElipse(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, float angle, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[120];

	float factor = ((float)M_PI / 180.0f) * 3;

	for (uint i = 0; i < 120; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor) * sin(angle * (PI / 180)));
	}

	result = SDL_RenderDrawPoints(renderer, points, 120);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void j1Render::PushMapSprite(SDL_Texture * texture, int x, int y, const SDL_Rect * section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle, bool not_in_world)
{
	iPoint in_camera_pos;
	in_camera_pos.x = (int)(camera->GetPosition().x * speed) + x;
	in_camera_pos.y = (int)(camera->GetPosition().y * speed) + y;

	if (camera->InsideRenderTarget(in_camera_pos.x, in_camera_pos.y))
	{
		Sprite* map_sprite = new Sprite(texture, x, y, section, flip, pivot_x, pivot_y, speed, angle);
		map_sprite_vec.push_back(map_sprite);
	}
}

void j1Render::PushInGameSprite(const Entity* entity)
{
	iPoint in_camera_pos;
	in_camera_pos.x = (int)(camera->GetPosition().x) + entity->GetPosition().x;
	in_camera_pos.y = (int)(camera->GetPosition().y) + entity->GetPosition().y;

	if (camera->InsideRenderTarget(in_camera_pos.x, in_camera_pos.y))
	{
		Sprite* entity_sprite;

		if (entity->GetEntityType() == E_UNIT)
		{
			Unit* unit = (Unit*)entity;

			if (unit->GetDir() == D_NORTH_EAST || unit->GetDir() == D_EAST || unit->GetDir() == D_SOUTH_EAST)
				entity_sprite = new Sprite(App->tex->GetTexture(unit->GetTextureID()), unit->GetX(), unit->GetY(), &unit->GetRect(), SDL_FLIP_HORIZONTAL, unit->GetPivot().x, unit->GetPivot().y);
			else
				entity_sprite = new Sprite(App->tex->GetTexture(unit->GetTextureID()), unit->GetX(), unit->GetY(), &unit->GetRect(), SDL_FLIP_NONE, unit->GetPivot().x, unit->GetPivot().y);
		}
		else
			entity_sprite = new Sprite(App->tex->GetTexture(entity->GetTextureID()), entity->GetX(), entity->GetY(), &entity->GetRect(), SDL_FLIP_NONE, entity->GetPivot().x, entity->GetPivot().y);

		PushInGameSprite(entity_sprite);
	}
}

void j1Render::PushInGameSprite(SDL_Texture * texture, int x, int y, const SDL_Rect * section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle, bool not_in_world)
{
	iPoint in_camera_pos;
	in_camera_pos.x = (int)(camera->GetPosition().x * speed) + x;
	in_camera_pos.y = (int)(camera->GetPosition().y * speed) + y;

	if (camera->InsideRenderTarget(in_camera_pos.x, in_camera_pos.y))
	{
		Sprite* in_game_sprite = new Sprite(texture, x, y, section, flip, pivot_x, pivot_y, speed, angle);
		PushInGameSprite(in_game_sprite);
	}
}

void j1Render::PushInGameSprite(SDL_Texture * texture, iPoint pos, const SDL_Rect * section, SDL_RendererFlip flip, iPoint pivot, float speed, double angle, bool not_in_world)
{
	PushInGameSprite(texture, pos.x, pos.y, section, flip, pivot.x, pivot.y, speed, angle, not_in_world);
}

void j1Render::PushInGameSprite(Sprite * sprite)
{
	iPoint in_camera_pos;
	in_camera_pos.x = (int)(camera->GetPosition().x * sprite->speed) + sprite->x;
	in_camera_pos.y = (int)(camera->GetPosition().y * sprite->speed) + sprite->y;

	if (camera->InsideRenderTarget(in_camera_pos.x, in_camera_pos.y))
	{
		std::deque<Sprite*>::iterator it = in_game_sprite_queue.begin();

		if (in_game_sprite_queue.size() != 0)
			while (it != in_game_sprite_queue.end())
			{
				if (sprite->y <= (*it)->y)
					break;
				it++;
			}

		in_game_sprite_queue.insert(it, sprite);
	}
}

void j1Render::PushUISprite(SDL_Texture * texture, int x, int y, const SDL_Rect * section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle)
{
	iPoint in_camera_pos;
	in_camera_pos.x = (int)(camera->GetPosition().x * speed) + x;
	in_camera_pos.y = (int)(camera->GetPosition().y * speed) + y;

	if (camera->InsideRenderTarget(in_camera_pos.x, in_camera_pos.y))
	{
		Sprite* ui_text = new Sprite(texture, x, y, section, flip, pivot_x, pivot_y, speed, angle);
		ui_sprite_vec.push_back(ui_text);
	}
}

void j1Render::BlitMap() const
{
	for (std::vector<Sprite*>::const_iterator it = map_sprite_vec.begin(); it != map_sprite_vec.end(); ++it)
		Blit((*it)->texture, (*it)->x, (*it)->y, (*it)->section, (*it)->flip, (*it)->pivot_x, (*it)->pivot_y, (*it)->speed, (*it)->angle);
}

void j1Render::BlitInGame() const
{
	for (std::deque<Sprite*>::const_iterator it = in_game_sprite_queue.begin(); it != in_game_sprite_queue.end(); ++it)
		Blit((*it)->texture, (*it)->x, (*it)->y, (*it)->section, (*it)->flip, (*it)->pivot_x, (*it)->pivot_y, (*it)->speed, (*it)->angle);
}

void j1Render::BlitSelection() const
{
	for (std::vector<Entity*>::iterator it = App->scene->selection.begin(); it != App->scene->selection.end(); ++it)
	{
		if ((*it)->GetEntityType() == E_UNIT)
		{
			Unit* unit = (Unit*)*it;
			DrawElipse(unit->GetX() + camera->GetPosition().x, unit->GetY() + camera->GetPosition().y, unit->GetUnitRadius(), 255, 255, 255, 20);
		}

		if ((*it)->GetEntityType() == E_BUILDING)
		{
			Building* building = (Building*)*it;
			building->GetBuildRectangle().Draw();

			if (building->GetBuildingType() == B_TURRET || building->GetBuildingType() == B_CANNON || building->GetBuildingType() == B_TURRET_UPGRADED_FIRE  || building->GetBuildingType() == B_TURRET_UPGRADED_ICE || building->GetBuildingType() == B_TURRET_UPGRADED_AIR || building->GetBuildingType() == B_CANNON_UPGRADED_FIRE || building->GetBuildingType() == B_CANNON_UPGRADED_ICE || building->GetBuildingType() == B_CANNON_UPGRADED_AIR)
			{
				int range = ((Tower*)building)->GetRange();
				DrawCircle(building->GetX() + camera->GetPosition().x, building->GetY() + camera->GetPosition().y, range, 255, 255, 255, 255);
			}
		}
	}
}

void j1Render::BlitUI() const
{
	for (std::vector<Sprite*>::const_iterator it = ui_sprite_vec.begin(); it != ui_sprite_vec.end(); ++it)
		Blit((*it)->texture, (*it)->x, (*it)->y, (*it)->section, (*it)->flip, (*it)->pivot_x, (*it)->pivot_y, (*it)->speed, (*it)->angle, true);
}

void j1Render::BlitMouse() const
{
	int x, y;
	App->input->GetMousePosition(x, y);
	Blit(App->tex->GetTexture(T_MOUSE_ICONS), x - camera->GetPosition().x, y - camera->GetPosition().y, &mouse_icon_rect, SDL_FLIP_NONE, pivot_mouse_x, pivot_mouse_y);
}

void j1Render::SetMouseIconRect(SDL_Rect rect, int pivotX, int pivotY)
{
	mouse_icon_rect = rect;
	pivot_mouse_x = pivotX;
	pivot_mouse_y = pivotY;
}

void j1Render::BlitSelectingRect()
{
	if (App->scene->group_select)
	{
		SDL_Rect rect = App->scene->GetSelectionRect();
		DrawQuad(rect, 255, 255, 255, 255, false);
	}
}

void j1Render::BlitMainMenu()
{
	BlitUI();
	BlitMouse();
	CleanUpUISpriteVec();
}

void j1Render::BlitGameScene()
{
	BlitMap();
	BlitInGame();
	BlitSelection();

	if(App->debug_features.quadtree)
		App->entity_manager->DrawQuadTree();

	BlitSelectingRect();
	BlitUI();
	//App->entity_manager->BlitMinimap();
	BlitMouse();
	//App->uimanager->DrawMinimapQuad();

	CleanUpMapVec();
	CleanUpInGameSpriteQueue();
	CleanUpUISpriteVec();
}

void j1Render::BlitScoreScene()
{
	BlitUI();
	BlitMouse();
	CleanUpUISpriteVec();
}

void j1Render::CleanUpMapVec()
{
	for (std::vector<Sprite*>::iterator it = map_sprite_vec.begin(); it != map_sprite_vec.end(); ++it)
		DELETE_PTR((*it));

	map_sprite_vec.clear();
}

void j1Render::CleanUpInGameSpriteQueue()
{
	for (std::deque<Sprite*>::iterator it = in_game_sprite_queue.begin(); it != in_game_sprite_queue.end(); ++it)
		DELETE_PTR((*it));

	in_game_sprite_queue.clear();
}

void j1Render::CleanUpUISpriteVec()
{
	for (std::vector<Sprite*>::iterator it = ui_sprite_vec.begin(); it != ui_sprite_vec.end(); ++it)
		DELETE_PTR((*it));

	ui_sprite_vec.clear();
}

Sprite::Sprite(SDL_Texture * texture, int x, int y, const SDL_Rect * section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle): texture(texture), x(x), y(y), flip(flip), pivot_x(pivot_x), pivot_y(pivot_y), speed(speed), angle(angle)
{
	if (section != nullptr)
	{
		this->section = new SDL_Rect(*section);
	}
	else
		this->section = nullptr;
}

Sprite::~Sprite()
{
	if(section != nullptr)
		DELETE_PTR(section);
}
