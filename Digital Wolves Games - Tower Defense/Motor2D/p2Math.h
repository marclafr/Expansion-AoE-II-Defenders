#ifndef _P2MATH
#define _P2MATH

#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

bool RectsOverlap(const SDL_Rect& a, const SDL_Rect& b);

bool IsZero(const SDL_Rect& rect);

#endif // !_P2MATH

