#include "p2Math.h"

bool RectsOverlap(const SDL_Rect & a, const SDL_Rect & b)
{
	//first rect with second
	iPoint top_left(a.x, a.y);
	if (b.x < top_left.x && b.x + b.w > top_left.x
		&& b.y < top_left.y && b.y + b.h > top_left.y)
		return true;
	iPoint top_right(a.x + a.w, a.y);
	if (b.x < top_right.x && b.x + b.w > top_right.x
		&& b.y < top_right.y && b.y + b.h > top_right.y)
		return true;
	iPoint bot_left(a.x, a.y + a.h);
	if (b.x < bot_left.x && b.x + b.w > bot_left.x
		&& b.y < bot_left.y && b.y + b.h > bot_left.y)
		return true;
	iPoint bot_right(a.x + a.w, a.y + a.w);
	if (b.x < bot_right.x && b.x + b.w > bot_right.x
		&& b.y < bot_right.y && b.y + b.h > bot_right.y)
		return true;

	//second rect with first
	top_left.x = b.x;
	top_left.x = b.y;
	if (a.x < top_left.x && a.x + a.w > top_left.x
		&& a.y < top_left.y && a.y + a.h > top_left.y)
		return true;
	top_right.x = b.x + b.w;
	top_right.y = b.y;
	if (a.x < top_right.x && a.x + a.w > top_right.x
		&& a.y < top_right.y && a.y + a.h > top_right.y)
		return true;
	bot_left.x = b.x;
	bot_left.y = b.y + b.h;
	if (a.x < bot_left.x && a.x + a.w > bot_left.x
		&& a.y < bot_left.y && a.y + a.h > bot_left.y)
		return true;
	bot_right.x = b.x + b.w;
	bot_right.y = b.y + b.w;
	if (a.x < bot_right.x && a.x + a.w > bot_right.x
		&& a.y < bot_right.y && a.y + a.h > bot_right.y)
		return true;

	return false;
}

bool IsZero(const SDL_Rect & rect)
{
	return rect.x == 0 && rect.y == 0 && rect.w == 0 && rect.h == 0;
}
