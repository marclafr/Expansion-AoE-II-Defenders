#include "p2Log.h"
#include "j1App.h"
#include "j1Map.h"
#include "Buildings.h"
#include "Wall.h"

Wall::Wall(iPoint pos, bool builded): Building(B_WALL, pos, builded), orientation(WO_CORNER), level(WL_WOOD)
{}

Wall::~Wall()
{}

void Wall::ChangeTexture()
{
	switch (state)
	{
	case BS_IN_CONSTRUCTION_BEGINING:
		SetRect({ 0,0,96,65 });
		SetPivot(0.53125f * 96, 0.59375f * 65);
		break;

	case BS_IN_CONSTRUCTION_MIDDLE:
		SetRect({ 98,0,100,75 });
		SetPivot(0.55f * 100, 0.643836f * 75);
		break;

	case BS_IN_CONSTRUCTION_END:
		SetRect({ 202,0,100,75 });
		SetPivot(0.55f * 100, 0.643836f * 75);
		break;

	case BS_BUILT:
		switch (level)
		{
		case WL_WOOD:
			switch (orientation)
			{
			case WO_CORNER:
				SetRect({ 610,289,100,106 });
				SetPivot(0.49f * 100, 0.754717f * 106);
				break;
			case WO_VERTICAL:
				SetRect({ 610,389,51,97 });
				SetPivot(0.490196f * 51, 0.742268f * 97);
				break;
			case WO_HORIZONTAL:
				SetRect({ 661,389,112,68 });
				SetPivot(0.491071f * 112, 0.808824f * 68);
				break;
			case WO_DIAGONAL_BOT_TOP:
				SetRect({ 773,389,76,82 });
				SetPivot(0.526316f * 76, 0.743902f * 82);
				break;
			case WO_DIAGONAL_TOP_BOT:
				SetRect({ 849,389,75,83 });
				SetPivot(0.466667f * 75, 0.746988f * 83);
				break;
			default:
				LOG("Invalid wall orientation, cant get texture");
				break;
			}
			break;

		case WL_STONE:
			switch (orientation)
			{
			case WO_CORNER:
				SetRect({ 1042,420,80,100 });
				SetPivot(0.5f * 80, 0.806122f * 100);
				break;
			case WO_VERTICAL:
				SetRect({ 118,443,80,100 });
				SetPivot(0.493506f * 80, 0.806122f * 100);
				break;
			case WO_HORIZONTAL:
				SetRect({ 1019,12,101,180 });
				SetPivot(0.494949f * 101, 0.865169f * 180);
				break;
			case WO_DIAGONAL_BOT_TOP:
				SetRect({ 242,446,98,78 });
				SetPivot(0.5f * 98, 0.871795f * 78);
				break;
			case WO_DIAGONAL_TOP_BOT:
				SetRect({ 197,443,42,110 });
				SetPivot(0.490196f * 42, 0.781818f * 110);
				break;
			default:
				LOG("Invalid wall orientation, cant get texture");
				break;
			}
			break;

		case WL_BRICK:
			switch (orientation)
			{
			case WO_CORNER:
				SetRect({ 95,75,89,113 });
				SetPivot(0.494382f * 89, 0.80531f * 113);
				break;
			case WO_VERTICAL:
				SetRect({ 184,75,89,114 });
				SetPivot(0.483146f * 89, 0.807018f * 114);
				break;
			case WO_HORIZONTAL:
				SetRect({ 0,60,95,169 });
				SetPivot(0.484211f * 95, 0.869822f * 169);
				break;
			case WO_DIAGONAL_BOT_TOP:
				SetRect({ 1042,1092,98,102 });
				SetPivot(0.489796f * 98, 0.843137f * 102);
				break;
			case WO_DIAGONAL_TOP_BOT:
				SetRect({ 1058,294,62,121 });
				SetPivot(0.419355f * 62, 0.785124f * 121);
				break;
			default:
				LOG("Invalid wall orientation, cant get texture");
				break;
			}
		default:
			LOG("Unidentified wall level, can´t give texture");
			break;
		}
		break;

	case BS_DESTROYED:
		SetRect({ 313, 1, 91, 51 });
		SetPivot(0.362637 * 91, 0.431373 * 51);
		break;

	default:
		LOG("Can not change building texture with null building state state");
		break;
	}
}

void Wall::UpgradeWall()
{
	switch (level)
	{
	case WL_WOOD:
		level = WL_STONE;
		SetLife(GetMaxHp() + HP_PER_LEVEL_UP);
		break;
	case WL_STONE:
		level = WL_BRICK;
		SetLife(GetMaxHp() + HP_PER_LEVEL_UP);
		break;
	case WL_BRICK:
		LOG("can not upgrade anymore");
		break;
	default:
		LOG("Invalid level, can not upgrade wall");
		break;
	}
}

void Wall::SetOrientation(WALL_ORIENTATION orientation)
{
	this->orientation = orientation;
}

void Wall::SetLevel(WALL_LEVEL level)
{
	this->level = level;
}

const iPoint & Wall::GetPixelPos() const
{
	iPoint ret (App->map->MapToWorld(GetPosition()));
	ret.y += App->map->data.tile_height / 2.0f;
	return ret;
}
