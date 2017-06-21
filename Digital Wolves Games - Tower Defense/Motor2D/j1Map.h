#ifndef __j1MAP_H__
#define __j1MAP_H__

#include <vector>
#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Module.h"

#define TILE_ANGLE (26.3283 / 360.0f * PI * 2.0f)

// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		std::string name;
		int value;
	};

	~Properties()
	{
		std::list<Property*>::iterator item = List.begin();

		while (item != List.end())
		{
			RELEASE(item._Ptr->_Myval);
			item++;
		}

		List.clear();
	}

	int Get(const char* name, bool default_value = false) const;

	std::list<Property*> List;
};

// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	SDL_Color				background_color;
	MapTypes				type;
	std::vector<TileSet*>	tilesets;
	std::vector<MapLayer*>	layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called each loop iteration
	void Draw();
	bool Update(float dt);
	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	//returns top corner of tile or area
	iPoint MapToWorld(iPoint pos, TileSet* tileset = nullptr) const;
	iPoint MapToWorld(int x, int y, TileSet* tileset = nullptr) const;
	fPoint MapToWorld(float x, float y, TileSet* tileset = nullptr) const;
	fPoint MapToWorld(fPoint pos) const;

	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer);
	bool CreateConstructibleMap1(int& width, int & height, uchar** buffer);
	bool CreateConstructibleMap2(int& width, int & height, uchar** buffer);
	TileSet* GetTilesetFromTileId(int id) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	//CHANGE
public:
	MapData data;

private:
	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__