#ifndef _WALL
#define _WALL

#include "Buildings.h"

#define HP_PER_LEVEL_UP 200

enum WALL_LEVEL
{
	WL_NO_LEVEL = 0,
	WL_WOOD,
	WL_STONE,
	WL_BRICK
};

enum WALL_ORIENTATION
{
	WO_NO_ORIENTATION = 0,
	WO_HORIZONTAL,
	WO_VERTICAL,
	WO_DIAGONAL_TOP_BOT,
	WO_DIAGONAL_BOT_TOP,
	WO_CORNER
};

class Wall: public Building
{
private:
	WALL_LEVEL level = WL_WOOD;
	WALL_ORIENTATION orientation = WO_HORIZONTAL;

public:
	//Constructors & Destructors
	Wall(iPoint pos, bool builded = false);
	~Wall();

	//Main Functions
	void ChangeTexture();

	//Usefull
	void UpgradeWall();

	//Setters
	void SetOrientation(WALL_ORIENTATION orientation);
	void SetLevel(WALL_LEVEL level);

	//Getters
	const iPoint& GetPixelPos() const;
};

#endif //!_WALL