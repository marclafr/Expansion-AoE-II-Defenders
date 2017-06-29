#define _CRT_SECURE_NO_WARNINGS
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include <math.h>
#include "Camera.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.assign("map");
}
// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	bool ret = true;

	folder = (config.child("folder").child_value());

	return ret;
}
bool j1Map::Start()
{
	//load map
	if (App->map->Load("AlphaOne.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		uchar* data2 = NULL;
		if (CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		if (CreateConstructibleMap1(w, h, &data) && App->map->CreateConstructibleMap2(w, h, &data2))
			App->pathfinding->SetConstructibleMaps(w, h, data, data2);
		RELEASE_ARRAY(data2);
		RELEASE_ARRAY(data);
	}
	return false;
}

bool j1Map::Update(float dt)
{
	Draw();
	return true;
}

bool j1Map::CreateWalkabilityMap(int& width, int & height, uchar** buffer) {

	bool ret = false;

	for (std::vector<MapLayer*>::iterator item = data.layers.begin(); item != data.layers.end(); ++item)
	{
		MapLayer* layer = *item;

		if (layer->properties.Get("Navigation") == false)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;

}
bool j1Map::CreateConstructibleMap1(int& width, int & height, uchar** buffer) {

	bool ret = false;
	for (std::vector<MapLayer*>::iterator item = data.layers.begin(); item != data.layers.end(); ++item)
	{
		MapLayer* layer = *item;

		if (layer->name.compare("ConstructibleAlly") != 0)
			continue;
		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

 				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 10 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;

}
bool j1Map::CreateConstructibleMap2(int& width, int & height, uchar** buffer) {

	bool ret = false;

	for (std::vector<MapLayer*>::iterator item = data.layers.begin(); item != data.layers.end(); ++item)
	{
		MapLayer* layer = *item;

		if (layer->name.compare("ConstructibleNeutral") != 0)
			continue;
		
		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 10 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;

}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	for (std::vector<MapLayer*>::iterator item = data.layers.begin(); item != data.layers.end(); ++item)
	{
		MapLayer* layer = *item;

		if (layer->properties.Get("Nodraw") == true)
			if (App->debug_features.debug_mode == false || App->debug_features.print_walkability_map == false)
				continue;

		for (int y = 0; y < data.height; y++)
			for (int x = 0; x < data.width; x++)
			{
				int tile_id = layer->Get(x, y);

				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);
					SDL_Rect r = tileset->GetTileRect(tile_id);
					fPoint pos = MapToWorld((float)x, (float)y);
					pos.x -= data.tile_width / 2.0f;
					App->render->PushMapSprite(tileset->texture, pos.x, pos.y, &r);
				}
			}
	}
}

int Properties::Get(const char* value, bool default_value) const
{
	std::list<Property*>::const_iterator item = List.begin();

	while (item != List.end())
	{
		if (item._Ptr->_Myval->name == value)
			return item._Ptr->_Myval->value;
		item++;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	TileSet* set = nullptr;

	for (int i = 0; i < data.tilesets.size(); i++)
	{
		if (i + 1 == data.tilesets.size())
		{
			set = data.tilesets[i];
			break;
		}

		if (id > data.tilesets[i]->firstgid && id < data.tilesets[i + 1]->firstgid)
		{
			set = data.tilesets[i];
			break;
		}
	}

	return set;
}

iPoint j1Map::MapToWorld(iPoint pos) const
{
	return MapToWorld(pos.x, pos.y);
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	fPoint ret(-1.0f,-1.0f);

	if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * ((float)data.tile_width * 0.5f);
		ret.y = (x + y) * ((float)data.tile_height * 0.5f) + (x + y) / 2.0f;
		return iPoint(ret.x, ret.y);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}
	return iPoint(ret.x, ret.y);
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	fPoint ret;
	float tile_height = data.tile_height - 1;
	float tile_width = data.tile_width - 1;

	ret.y = (y - tile_height / 2.0f) / (tile_height + 2.0f) - x / tile_width + 0.5f;
	ret.x = 2.0f * x / tile_width + ((y - tile_height / 2.0f) / (tile_height + 2.0f) - x / tile_width) + 0.5f;

	//Correcció
	float diff = ret.x - ret.y;

	ret.x -= 0.4f * (diff / 75.0f);
	ret.y += 0.4f * (diff / 75.0f);

	if (ret.x < 0) ret.x = 0;
	if (ret.y < 0) ret.y = 0;
	if (ret.x > 74) ret.x = 74;
	if (ret.y > 74) ret.y = 74;

	return iPoint(ret.x, ret.y);
}

fPoint j1Map::MapToWorld(float x, float y) const //returns top corner
{
	fPoint ret(-1.0f, -1.0f);

	if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * ((float)data.tile_width * 0.5f);
		ret.y = (x + y) * ((float)data.tile_height * 0.5f) + (x + y) / 2.0f;
		return ret;
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}
	return ret;
}

fPoint j1Map::MapToWorld(fPoint pos) const
{
	return MapToWorld(pos.x, pos.y);
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	// Remove all tilesets
	for (std::vector<TileSet*>::iterator item = data.tilesets.begin(); item != data.tilesets.end(); ++item)
	{
		RELEASE(*item);
	}
	data.tilesets.clear();

	// Remove all layers
	for (std::vector<MapLayer*>::iterator item = data.layers.begin(); item != data.layers.end(); ++item)
	{
		RELEASE(*item);
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	std::string tmp = folder + std::string(file_name);

	char* buf;
	int size = App->fs->Load(tmp.c_str(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info -----------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.push_back(lay);
	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);
	
		for (std::vector<TileSet*>::iterator item = data.tilesets.begin(); item != data.tilesets.end(); ++item)
		{
			TileSet* tile_set = *item;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", tile_set->name.c_str(), tile_set->firstgid);
			LOG("tile width: %d tile height: %d", tile_set->tile_width, tile_set->tile_height);
			LOG("spacing: %d margin: %d", tile_set->spacing, tile_set->margin);
		}

		for (std::vector<MapLayer*>::iterator item = data.layers.begin(); item != data.layers.end(); ++item)
		{
			MapLayer* layer = *item;
			LOG("Layer ----");
			LOG("name: %s", layer->name.c_str());
			LOG("tile width: %d tile height: %d", layer->width, layer->height);
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();

		std::string orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name = (tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.c_str(), image.attribute("source").as_string()), T_TILESET);
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}


bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_bool();

			properties.List.push_back(p);
		}
	}

	return ret;
}