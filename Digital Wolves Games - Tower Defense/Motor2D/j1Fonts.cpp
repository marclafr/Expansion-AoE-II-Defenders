#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1FileSystem.h"
#include "j1Fonts.h"

#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

j1Fonts::j1Fonts() : j1Module()
{
	name.assign("fonts");
}

// Destructor
j1Fonts::~j1Fonts()
{}

// Called before render is available
bool j1Fonts::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		default = Load(path, size);
	}

	for (pugi::xml_node node = conf.child("font"); node; node = node.next_sibling("font"))
	{
		const char* path = node.attribute("file").as_string();
		int size = node.attribute("size").as_int();
		Load(path, size);
	}

	return ret;
}

// Called before quitting
bool j1Fonts::CleanUp()
{
	std::list<TTF_Font*>::iterator item;

	for (std::list<Font*>::iterator item = App->font->fonts.begin(); item != App->font->fonts.end(); item++)
		TTF_CloseFont(item._Ptr->_Myval->font);
	
	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const j1Fonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFontRW(App->fs->Load(path), 1, size);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		std::string s = path;
		int pos = s.find("OpenSans-"); 
		pos += 9;// 9 because OpenSans- 
		s = s.substr(pos, s.length() - pos - 4);	// 4 because ".ttf"
		Font* new_font;
		if (s.compare("Bold") == 0)
			new_font = new Font(font, OPENSANS_BOLD);

		else if (s.compare("Light") == 0)
			new_font = new Font(font, OPENSANS_LIGHT);

		else if (s.compare("Regular") == 0)
			new_font = new Font(font, OPENSANS_REGULAR);

		fonts.push_back(new_font);
	}

	return font;
}

// Print text using font
SDL_Texture* j1Fonts::Print(const char* text, TextureID id, SDL_Color color,  TTF_Font* font)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : default, text, color);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->tex->LoadSurfaceLabel(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

SDL_Texture* j1Fonts::Print(const char* text, SDL_Color color, TTF_Font* font)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : default, text, color);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->tex->LoadSurfaceLabel(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool j1Fonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : default, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}