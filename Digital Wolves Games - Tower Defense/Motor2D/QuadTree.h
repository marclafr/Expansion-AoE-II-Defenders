#ifndef _QUADTREE
#define _QUADTREE

#include <vector>
#include "SDL\include\SDL_rect.h"
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "Resources.h"

#define NODE_ENTITIES 3

class TiledIsoRect;

class QuadTreeNode
{
private:
	QuadTreeNode* parent = nullptr;
	QuadTreeNode* childs[4];
	TiledIsoRect* area;
	Entity* entities[NODE_ENTITIES];

public:
	QuadTreeNode(const iPoint& tile_pos, const uint x_tiles, const uint tile_width, const uint y_tiles, const uint tile_height, QuadTreeNode* parent);
	~QuadTreeNode();
	
	bool AddEntity(Entity* entity);
	bool Inside(const Entity* entity) const;

	Entity* SearchFirst(int pixel_range, const  iPoint from) const;
	Entity* SearchFirst(IsoRect rect,const Entity* exeption = nullptr) const;
	Entity* SearchFirst(const SDL_Rect rect) const;
	Entity* SearchFirstUnit(const SDL_Rect& rect) const;
	Entity* SearchFirstEnemy(int pixel_range, iPoint from, Side side, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	Entity* SearchFirstEnemy(IsoRect rect, const Side side, ENTITY_TYPE entity_type) const;
	void Search(int pixel_range, const iPoint from, std::vector<Entity*>& vec) const;
	void Search(const IsoRect rect, std::vector<Entity*>& vec) const;
	void SearchForEnemies(int pixel_range, iPoint from, std::vector<Entity*>& vec, const Side side, ENTITY_TYPE entity_type = E_NO_ENTITY);

	void Selection(const SDL_Rect rect, std::vector<Entity*>& vec) const;

	Entity* ClickSelect(const iPoint& mouse_pos) const;

	void Update(float dt);
	void DeleteEntities();
	void CheckUnitCollisions(const Unit* ptr) const;

	//Draw
	void Draw() const;
	void DrawEntities() const;
	void DrawTiles() const;

	//void SaveAll(pugi::xml_node& node);
	//void BlitMinimap();

private:
	bool PushToCorrectChild(Entity* entity);
	bool SubDivide(Entity* entity);
	void Reorganise();
	void RedoQuads();
	bool Empty() const;
	int NumEntities() const;
	void MoveUpEntities();
};

class QuadTree
{
private:
	QuadTreeNode* origin;

public:
	QuadTree(const iPoint& tile_pos, const uint x_tiles, const uint tile_width, const uint y_tiles, const uint tile_height);
	~QuadTree();

	bool PushBack(Entity* entity) const;
	
	Entity* SearchFirst(int pixel_range, iPoint from) const;
	Entity* SearchFirstEnemy(int pixel_range, iPoint from, Side side, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	Entity* SearchFirstEnemy(IsoRect rect, const Side side, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	bool CheckIfFull(IsoRect tile,const Entity* exeption = nullptr) const;
	void Search(int pixel_range, iPoint from, std::vector<Entity*>& vec) const;
	void SearchForEnemies(int pixel_range, iPoint from, std::vector<Entity*>& vec, const Side side, ENTITY_TYPE entity_type = E_NO_ENTITY);
	void SearchInIsoRect(const IsoRect rect, std::vector<Entity*>& vec);
	Entity* SearchFirst(const SDL_Rect& rect) const;
	Entity* SearchFirstUnit(const SDL_Rect& rect) const;

	void Selection(SDL_Rect rect, std::vector<Entity*>& vec) const;

	void UpdateAll(float dt) const;

	void DeleteEntities() const;

	//Draw
	void Draw() const;

	//void SaveAll(pugi::xml_node& node);

	//void BlitMinimap() const;

	Entity* ClickSelect(const iPoint& mouse_pos) const;
};

class TiledIsoRect
{
private:
	iPoint tile_pos; //top
	uint x_tiles;
	uint y_tiles;
	uint tile_width;
	uint tile_height;
	IsoRect* rect;

public:
	TiledIsoRect(const iPoint& tile_pos, const uint x_tiles, const uint tile_width, const uint y_tiles, const uint tile_height, const iPoint& displacement = { 0,0 }, const SDL_Color& color = { 0,0,0,255 });
	~TiledIsoRect();

	void SetColor(SDL_Color color);

	//Draw Tile
	void DrawSquare() const;
	void DrawTiles() const;
	void DrawVertex() const;
	void DrawDiagonals() const;
	void DrawCenter() const;

	//Get Methods
	uint GetPixelWidth()const;
	uint GetPixelHeight()const;
	uint GetXTiles()const;
	uint GetYTiles()const;
	uint GetTileWidth() const;
	uint GetTileHeight() const;
	const iPoint& GetTilePos() const; //top
	const fPoint& GetPixelCenter();
	const fPoint& GetTileCenter();

	//Functionality
	bool TileInside(const fPoint tile) const;
	bool TileInside(const iPoint tile) const;
	bool PixelInside(const iPoint pos) const;
	bool PixelInside(const fPoint pos) const;
	bool Overlaps(SDL_Rect rect) const;
	bool Overlaps(IsoRect rect) const;
	bool Overlaps(iPoint center, int radius);
};
#endif
