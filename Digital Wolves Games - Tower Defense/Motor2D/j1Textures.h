#ifndef __j1TEXTURES_H__
#define __j1TEXTURES_H__

#include <vector>
#include "SDL\include\SDL_rect.h"
#include "j1Module.h"
#include "p2Point.h"

enum TOWER_TYPE;
enum BUILDING_TYPE;
struct SDL_Texture;
struct SDL_Surface;

enum TextureID
{
	T_UI,
	T_PATH_DEBUG,
	T_WALKABILITY_DEBUG,
	T_TILESET,
	T_AOE_UI_FONT,
	T_CONSOLE_TEXT,
	T_TURRET,
	T_TURRET_ALPHA_DOWN,
	T_WALL,
	T_WALL_ALPHA_DOWN,
	T_TOWNHALL,
	T_TOWNHALL_ALPHA_DOWN,
	T_RESOURCE,
	T_ARROW_BOMB,
	T_BUILDINGS_FIRE,
	T_EXPLOSIONS_AND_FLOOR,
	T_MOUSE_ICONS,
	T_MOUSE_CLICK_MOVE,
	T_MILL_ANIMATION,
	T_FISH_ANIMATION,

	//Units:
	T_VILLAGER,
		//INFANTRY
	T_MILITIA,
	T_ENEMY_MILITIA,
	T_MANATARMS,
	T_ENEMY_MANATARMS,
	T_LONGSWORDMAN,
	T_ENEMY_LONGSWORDMAN,
	T_TWOHANDEDSWORDMAN,
	T_ENEMY_TWOHANDEDSWORDMAN,
	T_CHAMPION,
	T_ENEMY_CHAMPION,

	T_SPEARMAN,
	T_ENEMY_SPEARMAN,
	T_PIKEMAN,
	T_ENEMY_PIKEMAN,

		//ARCHERS
	T_ARCHER,
	T_ENEMY_ARCHER,
	T_ARBALEST,
	T_ENEMY_ARBALEST,
	T_CAVALRYARCHER,
	T_ENEMY_CAVALRYARCHER,
	T_HEAVYCAVALRYARCHER,
	T_ENEMY_HEAVYCAVALRYARCHER,
	
		//CAVALRY
	T_KNIGHT,
	T_ENEMY_KNIGHT,
	T_CAVALIER,
	T_ENEMY_CAVALIER,
	T_PALADIN,
	T_ENEMY_PALADIN,
		
		//SIEGE
	T_SIEGERAM,
	T_ENEMY_SIEGERAM,
	T_MANGONEL
	//--
};

enum BUILDING_TEXTURE_TYPES
{
	BTT_NONE = 0,
	BTT_RED,
	BTT_GREEN,
};

struct ConstructionRect
{
	ConstructionRect(uint const_num, SDL_Texture* texture, SDL_Rect rect, iPoint pt);
	~ConstructionRect();
	uint construction_num;
	SDL_Texture* tex;
	SDL_Rect rect;
	iPoint pivot;
};

struct TowerRect
{
	TowerRect(TOWER_TYPE type, BUILDING_TEXTURE_TYPES build_tex_type, SDL_Texture* texture, SDL_Rect rect, iPoint pt);
	~TowerRect();
	TOWER_TYPE type;
	BUILDING_TEXTURE_TYPES build_tex_type;
	SDL_Texture* tex;
	SDL_Rect rect;
	iPoint pivot;
};

struct BuildingRect
{
	BuildingRect(BUILDING_TYPE type, BUILDING_TEXTURE_TYPES build_tex_type, SDL_Texture* texture, SDL_Rect rect, iPoint pt);
	~BuildingRect();
	BUILDING_TYPE type;
	BUILDING_TEXTURE_TYPES build_tex_type;
	SDL_Texture* tex;
	SDL_Rect rect;
	iPoint pivot;
};

struct Texture
{
private:
	SDL_Texture* texture;
	TextureID id;

public:

	Texture(SDL_Texture* tex, TextureID id);
	Texture(const Texture& copy);
	~Texture();

	TextureID GetID() const;
	SDL_Texture* GetTexture() const;

};

class j1Textures : public j1Module
{
public:

	j1Textures();

	// Destructor
	virtual ~j1Textures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path, TextureID id);
	bool UnLoad(Texture* texture);
	bool UnLoad(TextureID id);
	bool UnloadVideo(SDL_Texture* texture);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface, TextureID id);
	SDL_Texture* const LoadSurfaceVideo(SDL_Surface* surface);

	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;
	SDL_Texture* GetTexture(const TextureID id) const;

	//Quick Solution for Labels on UIHUDPanelInfo
	bool UnloadLabel(SDL_Texture* texture);
	SDL_Texture* const j1Textures::LoadSurfaceLabel(SDL_Surface* surface);

	void GetConstructionTexture(SDL_Texture*& ptr, SDL_Rect& rect, iPoint& pivot, uint state_num);
	void GetTowerTexture(SDL_Texture*& ptr, SDL_Rect& rect, iPoint& pivot, TOWER_TYPE tower, BUILDING_TEXTURE_TYPES = BTT_NONE);

	void GetBuildingTexture(SDL_Texture*& ptr, SDL_Rect& rect, iPoint & pivot, BUILDING_TYPE building, BUILDING_TEXTURE_TYPES color = BTT_NONE);


private:

	std::vector<Texture*> textures;

	std::vector<ConstructionRect> construction_rects;
	std::vector<TowerRect> towers_rects;
	std::vector<BuildingRect> buildings_rects;

	std::vector<SDL_Texture*> texturesVideo;

	uint ConstrString2Uint(const std::string name);
	TOWER_TYPE TowerString2Enum(const std::string name, BUILDING_TEXTURE_TYPES &color);
	//TODO:BUILDING_TYPE BuildingString2Enum(const std::string name);
};


#endif // __j1TEXTURES_H__