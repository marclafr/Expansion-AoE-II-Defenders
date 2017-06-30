#ifndef _ISOPRIMITVES_
#define _ISOPRIMITVES_

#include"p2Defs.h"
#include "p2Point.h"
#include"SDL_image/include/SDL_image.h"

class IsoRect;

///Class Primitive ------------------------------
//Base primitive class
class Primitive
{
public:

	Primitive();
	Primitive(const fPoint& position, const iPoint& displacement = { 0,0 }, const SDL_Color& color = { 0,0,0,255 });
	Primitive(const Primitive& copy);
	~Primitive();

protected:

	fPoint		position = { 0,0 };
	iPoint		displacement = { 0,0 };
	float		x_angle =  (30.0f / 360.0f * PI * 2.0f);
	SDL_Color	color = { 0,0,0,255 };

public:

	//Functionality ---------
	//Draw
	virtual bool	Draw();
	//Set Methods
	void			SetPosition(const fPoint& pos);
	void			SetDisplacement(const iPoint& desp);
	void			SetXAngle(float angle);
	void			SetColor(const SDL_Color& rgba);
	//Get Methods
	fPoint			GetPosition()const;
	iPoint			GetDisplacement()const;
	float			GetXAngle()const;
	SDL_Color		GetColor()const;
};
/// ---------------------------------------------


///Class Elipse ---------------------------------
//Elipse isometric primitive
class Elipse : public Primitive
{
public:

	Elipse(const fPoint& position = { 0,0 }, uint rad = 0, const iPoint& displacement = { 0,0 });
	Elipse(const iPoint& position = { 0,0 }, uint rad = 0, const iPoint& displacement = { 0,0 });
	Elipse(const Elipse& copy);
	~Elipse();

private:

	uint rad = 0;

public:

	//Functionality ---------
	//Draw
	bool	Draw();
	//Shape functions
	bool	IsIn(const fPoint& loc) const;
	bool	IsIn(const iPoint& loc) const;
	bool	Intersects(const SDL_Rect* rect) const;
	bool	Intersects(const Elipse* target)const;
	bool	Intersects(const IsoRect* target)const;
	bool	Intersects(const iPoint* point)const;
	bool	Overlap(const Elipse* target)const;
	iPoint  NearestPoint(const Elipse* target)const;
	iPoint	NearestPoint(const IsoRect* target)const;

	//Set Methods
	void	SetRad(uint r);
	//Get Methods
	uint	GetRad()const;
};
/// ---------------------------------------------

#define PIXEL_TOLERANCE 1.5f
///Class Rectangle ------------------------------
//Rectangle isometric primitive
class IsoRect : public Primitive
{
protected:
	float width = 0.0f;
	float height = 0.0f;
	double diagonal_horizontal_angle = 0.0f;
	double diagonal_verical_angle = 0.0f;

public:
	IsoRect();
	IsoRect(const fPoint& position, float width, float height, double diagonal_horizontal_angle = 0.0f, double diagonal_verical_angle = 0.0f, const iPoint& displacement = { 0,0 });
	IsoRect(const iPoint& position, float width, float height, double diagonal_angle = 0.0f, double diagonal_verical_angle = 0.0f, const iPoint& displacement = { 0,0 });
	IsoRect(const IsoRect& copy);
	~IsoRect();

	//Functionality ---------
	//Draw
	bool	Draw() const;
	void	DrawDiagonals() const;
	//Set Methods
	void	SetWidth(uint w);
	void	SetHeight(uint h);
	//Get Methods
	uint	GetWidth()const;
	uint	GetHeight()const;

	bool Inside(const fPoint pos) const;
	bool Inside(const iPoint pos) const;
	bool Overlaps(SDL_Rect rect) const;
	bool Overlaps(IsoRect rect) const;
	bool Overlaps(iPoint center, int radius);
};
/// ---------------------------------------------

///Class PivotedRect ----------------------------
class PivotedRect : public Primitive
{
public:

	PivotedRect(const fPoint& origin = {0,0}, const iPoint& goal = {0,0}, uint width = 0, uint height = 0);
	PivotedRect(const PivotedRect& copy);
	~PivotedRect();

private:

	iPoint	goal = { 0,0 };
	uint	width = 0;
	uint	height = 0;
	uint	pivot_distance = 0;

	iPoint vertex[4];

public:

	//Functionality ---------
	//Draw -------------
	bool	Draw();

	//Set Methods ------
	void	SetGoal(const iPoint& new_goal);
	void	SetWidth(uint new_width);
	void	SetHeight(uint new_height);
	void	SetPivotDistance(uint new_pivot_distance);

	void	CalculateVertex();
	bool	IsIn(const fPoint* loc) const;
	bool	Intersects(const SDL_Rect* rect)const;

	uint	GetWidth() const;
	uint	GetHeight() const;
	uint	GetPivotDistance() const;

};
/// ---------------------------------------------

///Class Line -----------------------------------
//Line isometric primitive
class Line : public Primitive
{
public:

	Line(const fPoint& position, const iPoint& position_2, const SDL_Color& color, const iPoint& displacement = { 0,0 });
	Line(const IsoRect& copy);
	~Line();

private:

	iPoint position_2 = { 0,0 };

public:

	//Functionality ---------
	//Draw
	bool	Draw();
	//Set Methods
	void			SetP1(const fPoint& p1);
	void			SetP2(const iPoint& p2);
	//Get Methods
	const fPoint&	GetP1()const;
	const iPoint& 	GetP2()const;
};
/// ---------------------------------------------

#endif // _ISOPRIMITVES_