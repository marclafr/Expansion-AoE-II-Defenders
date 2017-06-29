#ifndef __j1MOUSE__
#define __j1MOUSE__

#include "SDL\include\SDL.h"
#include "j1Module.h"

class j1Mouse : public j1Module
{
private:
	SDL_Rect icon_rect;
	int pivot_x = 0;
	int pivot_y = 0;

public:
	j1Mouse();
	~j1Mouse();
	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);
};

#endif //__j1MOUSE__
