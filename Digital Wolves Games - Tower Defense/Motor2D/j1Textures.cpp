#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "Entity.h"
#include "Towers.h"
#include "Buildings.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

j1Textures::j1Textures() : j1Module()
{
	name.assign("textures");
}

// Destructor
j1Textures::~j1Textures()
{}

// Called before render is available
bool j1Textures::Awake(pugi::xml_node& config)
{
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Textures::Start()
{
	bool ret = true;

	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	//Units
		//INFANTRY
	App->tex->Load("animations/Units/Militia.png", T_MILITIA);
	App->tex->Load("animations/Units/MilitiaRed.png", T_ENEMY_MILITIA);
	App->tex->Load("animations/Units/Manatarms.png", T_MANATARMS);
	App->tex->Load("animations/Units/ManatarmsRed.png", T_ENEMY_MANATARMS);
	App->tex->Load("animations/Units/Longswordman.png", T_LONGSWORDMAN);
	App->tex->Load("animations/Units/LongswordmanRed.png", T_ENEMY_LONGSWORDMAN);
	App->tex->Load("animations/Units/Twohandedswordman.png", T_TWOHANDEDSWORDMAN);
	App->tex->Load("animations/Units/TwohandedswordmanRed.png", T_ENEMY_TWOHANDEDSWORDMAN);
	App->tex->Load("animations/Units/Champion.png", T_CHAMPION);
	App->tex->Load("animations/Units/ChampionRed.png", T_ENEMY_CHAMPION);

	App->tex->Load("animations/Units/Spearman.png", T_SPEARMAN);
	App->tex->Load("animations/Units/SpearmanRed.png", T_ENEMY_SPEARMAN);
	App->tex->Load("animations/Units/Pikeman.png", T_PIKEMAN);
	App->tex->Load("animations/Units/PikemanRed.png", T_ENEMY_PIKEMAN);

	//ARCHERS
	App->tex->Load("animations/Units/Archer.png", T_ARCHER);
	App->tex->Load("animations/Units/ArcherRed.png", T_ENEMY_ARCHER);
	App->tex->Load("animations/Units/Arbalest.png", T_ARBALEST);
	App->tex->Load("animations/Units/ArbalestRed.png", T_ENEMY_ARBALEST);
	App->tex->Load("animations/Units/Cavalryarcher.png", T_CAVALRYARCHER);
	App->tex->Load("animations/Units/CavalryarcherRed.png", T_ENEMY_CAVALRYARCHER);
	App->tex->Load("animations/Units/Heavycavalryarcher.png", T_HEAVYCAVALRYARCHER);
	App->tex->Load("animations/Units/HeavycavalryarcherRed.png", T_ENEMY_HEAVYCAVALRYARCHER);

	//CAVALRY
	App->tex->Load("animations/Units/Knight.png", T_KNIGHT);
	App->tex->Load("animations/Units/KnightRed.png", T_ENEMY_KNIGHT);
	App->tex->Load("animations/Units/Cavalier.png", T_CAVALIER);
	App->tex->Load("animations/Units/CavalierRed.png", T_ENEMY_CAVALIER);
	App->tex->Load("animations/Units/Paladin.png", T_PALADIN);
	App->tex->Load("animations/Units/PaladinRed.png", T_ENEMY_PALADIN);

	//SIEGE
	App->tex->Load("animations/Units/Siegeram.png", T_SIEGERAM);
	App->tex->Load("animations/Units/SiegeramRed.png", T_ENEMY_SIEGERAM);
	App->tex->Load("animations/Units/Mangonel.png", T_MANGONEL);
	//--

	App->tex->Load("textures/Towers.png", T_TURRET);
	App->tex->Load("textures/Towers.png", T_TURRET_ALPHA_DOWN);
	SDL_SetTextureAlphaMod(GetTexture(T_TURRET_ALPHA_DOWN), 160);
	App->tex->Load("animations/StoneWall.png", T_WALL);
	App->tex->Load("animations/StoneWall.png", T_WALL_ALPHA_DOWN);
	SDL_SetTextureAlphaMod(GetTexture(T_WALL_ALPHA_DOWN), 160);
	App->tex->Load("textures/Extras.png", T_TOWNHALL);
	App->tex->Load("textures/Extras.png", T_TOWNHALL_ALPHA_DOWN);
	SDL_SetTextureAlphaMod(GetTexture(T_TOWNHALL_ALPHA_DOWN), 160);
	App->tex->Load("textures/Resouces.png", T_RESOURCE);
	App->tex->Load("animations/ArrowsBombs.png", T_ARROW_BOMB);
	App->tex->Load("animations/BuildingsFire.png", T_BUILDINGS_FIRE); 
	App->tex->Load("animations/ExplosionsAndFloor.png", T_EXPLOSIONS_AND_FLOOR); 
	App->tex->Load("animations/MillAnimation.png", T_MILL_ANIMATION);
	App->tex->Load("animations/Fish_anim.png", T_FISH_ANIMATION);

	//MOUSE ICONS
	App->tex->Load("textures/MouseIcons.png", T_MOUSE_ICONS);
	App->tex->Load("animations/MouseClickMove.png", T_MOUSE_CLICK_MOVE);
	//--

	std::string towers_walls_folder = "textures/Towers.xml";

	//Load UNIT animations data from animations folder
	char* buff = nullptr;
	int size = App->fs->Load(towers_walls_folder.c_str(), &buff);
	pugi::xml_document towers_walls_data;
	pugi::xml_parse_result result = towers_walls_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading TOWERS/WALLS XML data: %s", result.description());
		return false;
	}

	//Loading units
	pugi::xml_node sprite_node = towers_walls_data.child("TextureAtlas").first_child();	

		//CONSTRUCTIONS
	pugi::xml_node const_node = sprite_node.first_child();
	while (const_node != NULL)
	{
		std::string const_name = const_node.attribute("n").as_string();
		uint const_num = ConstrString2Uint(const_name);
		construction_rects.push_back(ConstructionRect(const_num, GetTexture(T_TURRET), { const_node.attribute("x").as_int(),const_node.attribute("y").as_int(),const_node.attribute("w").as_int(),const_node.attribute("h").as_int() }, { (int)(const_node.attribute("w").as_int() * const_node.attribute("pX").as_float()), (int)(const_node.attribute("h").as_int() * const_node.attribute("pY").as_float()) }));
		const_node = const_node.next_sibling();
	}

		//TOWERS
	sprite_node = sprite_node.next_sibling();
	pugi::xml_node tower_node = sprite_node.first_child();
	while (tower_node != NULL)
	{
		std::string tower_name = tower_node.attribute("n").as_string();
		BUILDING_TEXTURE_TYPES color;
		TOWER_TYPE type = TowerString2Enum(tower_name, color);
		towers_rects.push_back(TowerRect(type, color, GetTexture(T_TURRET), { tower_node.attribute("x").as_int(),tower_node.attribute("y").as_int(),tower_node.attribute("w").as_int(),tower_node.attribute("h").as_int() }, { (int)(tower_node.attribute("w").as_int() * tower_node.attribute("pX").as_float()), (int)(tower_node.attribute("h").as_int() * tower_node.attribute("pY").as_float()) }));
		tower_node = tower_node.next_sibling();
	}

		//WALLS
	sprite_node = sprite_node.next_sibling();
	pugi::xml_node walls_node = sprite_node.first_child();
	while (walls_node != NULL)
	{
		std::string walls_name = sprite_node.attribute("n").as_string();
		//TODO: CHARGE WALLS/BUILDINGS RECTS

		walls_node = walls_node.next_sibling();
	}

	return ret;
}

// Called before quitting
bool j1Textures::CleanUp()
{
	std::vector<Texture*>::iterator item;
	for (item = textures.begin(); item != textures.end(); ++item)
	{
		delete (*item);
	}
	textures.clear();
	construction_rects.clear();
	towers_rects.clear();
	buildings_rects.clear();

	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const j1Textures::Load(const char* path, TextureID id)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load_RW(App->fs->Load(path), 1);

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		//ignores pink from images
		SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));

		texture = LoadSurface(surface, id);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool j1Textures::UnLoad(Texture* texture)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (texture == textures[i])
		{
			delete (textures[i]);
			std::vector<Texture*>::iterator item = textures.begin() + i;
			textures.erase(item);
			return true;
		}
	}

	return false;
}

bool j1Textures::UnLoad(TextureID id)
{
	std::vector<Texture*>::iterator item;

	for (int i = 0; i < textures.size(); i++)
	{
		if (id == textures[i]->GetID())
		{
			delete (textures[i]);
			std::vector<Texture*>::iterator item = textures.begin() + i;
			textures.erase(item);
			return true;
		}
	}

	return false;
}

bool j1Textures::UnloadVideo(SDL_Texture* image)
{


	for (std::vector<SDL_Texture*>::iterator item = texturesVideo.begin(); item != texturesVideo.end(); ++item)
	{

		if (image == (*item))
		{
			texturesVideo.erase(item);
			SDL_DestroyTexture(*item);
			return true;
		}
	}
	return false;
}

// Translate a surface into a texture
SDL_Texture* const j1Textures::LoadSurface(SDL_Surface* surface, TextureID id)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		Texture* ptr = new Texture(texture, id);
		textures.push_back(ptr);
	}

	return texture;
}

SDL_Texture * const j1Textures::LoadSurfaceVideo(SDL_Surface * surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture for video!! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		texturesVideo.push_back(texture);
	}

	return texture;
}

// Retrieve size of a texture
void j1Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}

//-------------------------------------------------------------------------

Texture::Texture(SDL_Texture * tex, TextureID id) : texture(tex), id(id)
{}

Texture::Texture(const Texture & copy): id(copy.id)
{}

Texture::~Texture()
{
	//SDL_DestroyTexture(texture);
}

TextureID Texture::GetID() const
{
	return id;
}

SDL_Texture * Texture::GetTexture() const
{
	return texture;
}

SDL_Texture * j1Textures::GetTexture(const TextureID id) const
{
	for (int i = 0; i < textures.size(); i++)
		if (textures[i]->GetID() == id)
			return textures[i]->GetTexture();
	return nullptr;
}

//Quick fix for labels on HUDUIPanelInfo
bool j1Textures::UnloadLabel(SDL_Texture * texture)
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
	return true;
}

SDL_Texture * const j1Textures::LoadSurfaceLabel(SDL_Surface * surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}

	return texture;
}

void j1Textures::GetConstructionTexture(SDL_Texture *& ptr, SDL_Rect & rect, iPoint & pivot, uint state_num)
{
	for (int i = 0; i < construction_rects.size(); i++)
	{
		if (construction_rects[i].construction_num == state_num)
		{
			ptr = construction_rects[i].tex;
			rect = construction_rects[i].rect;
			pivot = construction_rects[i].pivot;
			break;
		}
	}
}

void j1Textures::GetTowerTexture(SDL_Texture *& ptr, SDL_Rect & rect, iPoint & pivot, TOWER_TYPE tower, BUILDING_TEXTURE_TYPES color)
{
	for (int i = 0; i < towers_rects.size(); i++)
	{
		if (towers_rects[i].type == tower && towers_rects[i].build_tex_type == color)
		{
			ptr = towers_rects[i].tex;
			rect = towers_rects[i].rect;
			pivot = towers_rects[i].pivot;
			break;
		}
	}
}

void j1Textures::GetBuildingTexture(SDL_Texture *& ptr, SDL_Rect & rect, iPoint & pivot, BUILDING_TYPE building, BUILDING_TEXTURE_TYPES color)
{
	for (int i = 0; i < buildings_rects.size(); i++)
	{
		if (buildings_rects[i].type == building && buildings_rects[i].build_tex_type == color)
		{
			ptr = buildings_rects[i].tex;
			rect = buildings_rects[i].rect;
			pivot = buildings_rects[i].pivot;
			break;
		}
	}
}

uint j1Textures::ConstrString2Uint(const std::string name)
{
	if (name == "build_construct_1")
		return 1;

	else if (name == "build_construct_2")
		return 2;

	else if (name == "build_construct_3")
		return 3;

}

TOWER_TYPE j1Textures::TowerString2Enum(const std::string name, BUILDING_TEXTURE_TYPES &btt)
{
	btt = BTT_NONE;

	if (name == "Basic_tower")
		return T_BASIC_TOWER;

	else if (name == "Basic_tower_red")
	{
		btt = BTT_RED;
		return T_BASIC_TOWER;
	}
	else if (name == "Basic_tower_green")
	{
		btt = BTT_GREEN;
		return T_BASIC_TOWER;
	}
	
	else if (name == "Bombard_tower")
		return T_BOMBARD_TOWER;

	else if (name == "Bombard_tower_red")
	{
		btt = BTT_RED;
		return T_BOMBARD_TOWER;
	}
	else if (name == "Bombard_tower_green")
	{
		btt = BTT_GREEN;
		return T_BOMBARD_TOWER;
	}

	else if (name == "Fire_tower")
		return T_FIRE_TOWER;

	else if (name == "Bombard_fire_tower")
		return T_BOMBARD_FIRE_TOWER;

	else if (name == "Ice_tower")
		return T_ICE_TOWER;

	else if (name == "Bombard_ice_tower")
		return T_BOMBARD_ICE_TOWER;

	else if (name == "Air_tower")
		return T_AIR_TOWER;

	else if (name == "Bombard_air_tower")
		return T_BOMBARD_AIR_TOWER;

	return T_NO_TYPE;
}

BuildingRect::BuildingRect(BUILDING_TYPE type, BUILDING_TEXTURE_TYPES build_tex_type, SDL_Texture * texture, SDL_Rect rect, iPoint pt) : type(type), build_tex_type(build_tex_type), tex(texture), rect(rect), pivot(pt)
{}

BuildingRect::~BuildingRect()
{}

TowerRect::TowerRect(TOWER_TYPE type, BUILDING_TEXTURE_TYPES build_tex_type, SDL_Texture * texture, SDL_Rect rect, iPoint pt) : type(type), build_tex_type(build_tex_type), tex(texture), rect(rect), pivot(pt)
{}

TowerRect::~TowerRect()
{}

ConstructionRect::ConstructionRect(uint const_num, SDL_Texture * texture, SDL_Rect rect, iPoint pt) : construction_num(const_num), tex(texture), rect(rect), pivot(pt)
{}

ConstructionRect::~ConstructionRect()
{}