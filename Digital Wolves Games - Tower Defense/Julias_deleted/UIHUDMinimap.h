#pragma once

#ifndef __UIHUDMINIMAP_H__
#define __UIHUDMINIMAP_H__

#include "UIComponents.h"
#include "p2Point.h"

#define GREEN_POINT_ATLAS {1164, 2575 ,2, 2}
#define RED_POINT_ATLAS {1164, 2572, 2, 2}

class IsoRect;

class UIHUDMinimap : public UIComponents
{
private:
	UIComponents* minimap_image = nullptr;

	IsoRect* minimap;

	iPoint quad_minimap_position;

	SDL_Rect rect_map;
	SDL_Rect quad_atlas;

public:
	UIHUDMinimap(UICOMPONENT_TYPE type = UIT_UIHUDMINIMAP);
	~UIHUDMinimap();

	bool Update();

	void Draw();
	void DrawQuad();

	iPoint WorldToMinimap(iPoint world_point);
	iPoint MinimapToWorld(iPoint minimap_point);

	const SDL_Rect GetRectMap() const;
};

#endif // __UIHUDMINIMAP_H__