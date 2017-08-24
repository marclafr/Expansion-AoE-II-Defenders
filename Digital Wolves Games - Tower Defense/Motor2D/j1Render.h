#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"
#include <deque>
#include <vector>

class Entity;
class Camera;

struct Sprite
{
	SDL_Texture* texture;
	int x;
	int y;
	const SDL_Rect* section;
	SDL_RendererFlip flip;
	int pivot_x;
	int pivot_y;
	float speed;
	int alpha;
	double angle;

	Sprite(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, const int alpha = 255, float speed = 1.0f, double angle = 0);
	~Sprite();
};

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;
	fPoint ScreenToWorldf(int x, int y) const;
	iPoint WorldToScreen(int x, int y) const;

	// Draw & Blit
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0, bool not_in_world = false, const int alpha = 255) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawElipse(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, float angle, Uint8 a = 255, bool use_camera = true) const;

	void PushMapSprite(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0, bool not_in_world = false);
	void PushInGameSprite(const Entity* entity, const int alpha = 255);
	void PushInGameSprite(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0, bool not_in_world = false);
	void PushInGameSprite(SDL_Texture* texture, iPoint pos, const SDL_Rect* section = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, iPoint pivot = {0,0}, float speed = 1.0f, double angle = 0, bool not_in_world = false);
	void PushInGameSprite(Sprite* sprite);
	void PushUISprite(SDL_Texture* texture, int x, int y, const SDL_Rect* section = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0, const int alpha = 255);
	
	void BlitMap() const;
	void BlitInGame() const;
	void BlitSelection() const;
	void BlitUI() const;
	void BlitMouse() const;
	void SetMouseIconRect(SDL_Rect rect, int pivotX, int pivotY);
	void BlitSelectingRect();
	
	void BlitMainMenu();
	void BlitGameScene();
	void BlitScoreScene();

	void CleanUpMapVec();
	void CleanUpInGameSpriteQueue();
	void CleanUpUISpriteVec();

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Get width and height from texture
	void GetDimensionsFromTexture(SDL_Texture* tex, int& w, int& h);

public:

	SDL_Renderer*	renderer;
	Camera*			camera;
	SDL_Rect		viewport;
	SDL_Color		background;

private:
	std::vector<Sprite*> map_sprite_vec;
	std::deque<Sprite*> in_game_sprite_queue;
	std::vector<Sprite*> ui_sprite_vec;
	SDL_Rect mouse_icon_rect;
	int pivot_mouse_x = 0;
	int pivot_mouse_y = 0;
};


#endif // __j1RENDER_H__