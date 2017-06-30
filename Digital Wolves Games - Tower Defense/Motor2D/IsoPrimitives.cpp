#include <math.h>

#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "IsoPrimitives.h"

///Class Primitive ------------------------------
//Constructors ==============
Primitive::Primitive()
{}

Primitive::Primitive(const fPoint& position, const iPoint& displacement, const SDL_Color& color) :position(position), displacement(displacement), color(color)
{}

Primitive::Primitive(const Primitive& copy) : position(copy.position), displacement(copy.displacement), color(copy.color), x_angle(copy.x_angle)
{}

//Destructors ===============
Primitive::~Primitive()
{

}

//Functionality =============
bool Primitive::Draw()
{
	return true;
}

void Primitive::SetPosition(const fPoint & pos)
{
	position = pos;
}

void Primitive::SetDisplacement(const iPoint & desp)
{
	displacement = desp;
}

void Primitive::SetXAngle(float angle)
{
	x_angle = angle;
}

void Primitive::SetColor(const SDL_Color & rgba)
{
	color = rgba;
}

fPoint Primitive::GetPosition() const
{
	return position;
}

iPoint Primitive::GetDisplacement() const
{
	return displacement;
}

float Primitive::GetXAngle() const
{
	return x_angle;
}
SDL_Color Primitive::GetColor() const
{
	return color;
}
/// ---------------------------------------------


///Class Elipse ---------------------------------
//Constructors ==============
Elipse::Elipse(const fPoint& position, uint rad, const iPoint& displacement) :Primitive(position, displacement), rad(rad)
{}

Elipse::Elipse(const iPoint & position, uint rad, const iPoint & displacement) : Primitive(fPoint(position.x, position.y), displacement), rad(rad)
{}

Elipse::Elipse(const Elipse & copy) : Primitive(copy), rad(copy.rad)
{}

//Destructors ===============
Elipse::~Elipse()
{}

//Functionality =============
bool Elipse::Draw()
{
	float angle = x_angle * 180 / PI;
	return App->render->DrawElipse(position.x + displacement.x, position.y + displacement.y, rad, color.r, color.g, color.b, angle, color.a, true);
}

bool Elipse::IsIn(const fPoint& loc) const
{
	float sin_rad = rad * sin(x_angle);
	return ((((loc.x - position.x) * (loc.x - position.x)) / (rad  *rad) + ((loc.y - position.y) * (loc.y - position.y)) / (sin_rad * sin_rad)) <= 1.0f);
}

bool Elipse::IsIn(const iPoint& loc) const
{
	float sin_rad = rad * sin(x_angle);
	return (((((float)loc.x - position.x) * ((float)loc.x - position.x)) / (rad  *rad) + (((float)loc.y - position.y) * ((float)loc.y - position.y)) / (sin_rad * sin_rad)) <= 1.0f);
}

bool Elipse::Intersects(const SDL_Rect * rect) const
{
	if (rect == nullptr)return false;
	float dx = abs(position.x - (rect->x + rect->w * 0.5));
	float dy = abs(position.y - (rect->y + rect->h * 0.5));

	//Case the rect is out the circle limits in axis
	if (dx > rect->w * 0.5 + rad) return false;
	if (dy > rect->h * (0.5 + rad * sin(x_angle))) return false;

	//Case the rect is in the circle limits in axis
	if (dx <= rect->w * 0.5) return true;
	if (dx <= rect->w * 0.5) return true;

	//Case the rect intersects or not in the circle mid curves
	float a = (dx - rect->w * 0.5);
	float b = (dy - rect->h * 0.5);
	float corner_distance = a * a + b * b;

	return corner_distance <= rad ^ 2;
}

bool Elipse::Intersects(const Elipse * target) const
{
	if (target == nullptr)
		return false;
	iPoint vec(target->position.x - position.x, target->position.y - position.y);
	fPoint cpy(vec.x, vec.y);
	cpy.Norm();
	vec.x -= ((rad * cpy.x) + target->rad * cpy.x);
	vec.y -= (((target->rad + rad) * sin(x_angle) * cpy.y));
	float vec_len = sqrt(vec.x * vec.x + vec.y * vec.y);
	return (rad >= vec_len);
}
bool Elipse::Overlap(const Elipse* target) const
{
	if (target == nullptr)return false;

	int radius = rad + target->GetRad() + 1;
	int deltaX = position.x - target->GetPosition().x;
	int deltaY = position.y - target->GetPosition().y;

	return (abs(deltaX) < radius && abs(deltaY) < radius*sin(x_angle));
}

bool Elipse::Intersects(const IsoRect * target) const
{
	if (target == nullptr)return false;
	iPoint vec = (iPoint(target->GetPosition().x, target->GetPosition().y) + target->GetDisplacement()) - iPoint(position.x,position.y);
	fPoint norm(vec.x, vec.y);
	norm.Norm();
	vec.x -= ceil((target->GetWidth() * 0.5) * norm.x);
	float diagonal = sqrt(target->GetHeight() / 2.0f * target->GetHeight() / 2.0f + target->GetWidth() / 2.0f);
	vec.y -= ceil(((diagonal) * 0.5) * norm.y);
	float len = sqrt(vec.x * vec.x + vec.y * vec.y);
	return (len <= rad);
}

bool Elipse::Intersects(const iPoint* point) const
{	
	float r_x = rad * cos(x_angle);
	float r_y = rad * sin(x_angle);
	float delta_x = (point->x - position.x) * (point->x - position.x) / (r_x * r_x);
	float delta_y = (point->y - position.y) * (point->y - position.y) / (r_y * r_y);

	return delta_x + delta_y <= 1.0f;
}

iPoint Elipse::NearestPoint(const Elipse* target) const
{
	iPoint vec(target->position.x - position.x, target->position.y - position.y);
	fPoint cpy(vec.x, vec.y);
	cpy.Norm();
	vec.x -= ((rad * cpy.x) + target->rad * cpy.x);
	vec.y -= (((target->rad + rad) * sin(x_angle) * cpy.y));

	return iPoint(position.x + vec.x, position.y + vec.y);
}

iPoint Elipse::NearestPoint(const IsoRect* target) const
{
	iPoint vec = (iPoint(target->GetPosition().x, target->GetPosition().y) + target->GetDisplacement()) - iPoint(position.x, position.y);
	fPoint norm(vec.x, vec.y);
	norm.Norm();
	vec.x -= ceil((target->GetWidth() * 0.5) * norm.x);
	vec.y -= ceil((target->GetHeight() * 0.5) * norm.y);
	return iPoint(position.x + vec.x, position.y + vec.y);
}

void Elipse::SetRad(uint r)
{
	rad = r;
}

uint Elipse::GetRad() const
{
	return rad;
}
/// ---------------------------------------------

IsoRect::IsoRect() :Primitive({ 0,0 }, { 0,0 }), width(0), height(0)
{}

///Class Rectangle ------------------------------
//Constructors ==============
IsoRect::IsoRect(const fPoint& position, float width, float height, double diagonal_horizontal_angle, double diagonal_verical_angle, const iPoint& displacement) :Primitive(position, displacement), width(width), height(height), diagonal_horizontal_angle(diagonal_horizontal_angle), diagonal_verical_angle(diagonal_verical_angle)
{}

IsoRect::IsoRect(const iPoint& position, float width, float height, double diagonal_horizontal_angle, double diagonal_verical_angle, const iPoint& displacement) : Primitive(fPoint(position.x,position.y), displacement), width(width), height(height), diagonal_horizontal_angle(diagonal_horizontal_angle), diagonal_verical_angle(diagonal_verical_angle)
{}

IsoRect::IsoRect(const IsoRect& copy) : Primitive(copy), width(copy.width), height(copy.height), diagonal_horizontal_angle(copy.diagonal_horizontal_angle), diagonal_verical_angle(copy.diagonal_verical_angle)
{}

//Destructors ===============
IsoRect::~IsoRect()
{}

//Functionality =============
bool IsoRect::Draw() const
{
	iPoint bottom_vertex (position.x + 0.5f * height * sinf(diagonal_verical_angle), position.y + 0.5f * height * cosf(diagonal_verical_angle));
	iPoint top_vertex (position.x - 0.5f * height * sinf(diagonal_verical_angle), position.y - 0.5f * height * cosf(diagonal_verical_angle));
	iPoint right_vertex (position.x + 0.5f * width * cosf(diagonal_horizontal_angle), position.y + 0.5f * width * sinf(diagonal_horizontal_angle));
	iPoint left_vertex (position.x - 0.5f * width * cosf(diagonal_horizontal_angle), position.y - 0.5f * width * sinf(diagonal_horizontal_angle));

	App->render->DrawLine(top_vertex.x, top_vertex.y, right_vertex.x, right_vertex.y, color.r, color.g, color.b, color.a, true);
	App->render->DrawLine(top_vertex.x, top_vertex.y, left_vertex.x, left_vertex.y, color.r, color.g, color.b, color.a, true);
	App->render->DrawLine(bottom_vertex.x, bottom_vertex.y, right_vertex.x, right_vertex.y, color.r, color.g, color.b, color.a, true);
	App->render->DrawLine(bottom_vertex.x, bottom_vertex.y, left_vertex.x, left_vertex.y, color.r, color.g, color.b, color.a, true);
	
	//Center
	SDL_Rect rect{ position.x - 3, position.y - 3, 6,6 };
	App->render->DrawQuad(rect, 255, 255, 0, 255, true, true);

	return true;
}

void IsoRect::DrawDiagonals() const
{
	iPoint bottom_vertex(position.x + 0.5f * height * sinf(diagonal_verical_angle), position.y + 0.5f * height * cosf(diagonal_verical_angle));
	iPoint top_vertex(position.x - 0.5f * height * sinf(diagonal_verical_angle), position.y - 0.5f * height * cosf(diagonal_verical_angle));
	iPoint right_vertex(position.x + 0.5f * width * cosf(diagonal_horizontal_angle), position.y + 0.5f * width * sinf(diagonal_horizontal_angle));
	iPoint left_vertex(position.x - 0.5f * width * cosf(diagonal_horizontal_angle), position.y - 0.5f * width * sinf(diagonal_horizontal_angle));

	//Diagonals
	App->render->DrawLine(top_vertex.x, top_vertex.y, bottom_vertex.x, bottom_vertex.y, 0, 255, 0, 255, true);
	App->render->DrawLine(right_vertex.x, right_vertex.y, left_vertex.x, left_vertex.y, 0, 255, 0, 255, true);
}

void IsoRect::SetWidth(uint w)
{
	width = w;
}

void IsoRect::SetHeight(uint h)
{
	height = h;
}

uint IsoRect::GetWidth() const
{
	return width;
}

uint IsoRect::GetHeight() const
{
	return height;
}

bool IsoRect::Inside(const fPoint pos) const
{
	float delta_x = abs(pos.x - position.x);
	if (delta_x >= width / 2.0f)
		return false;

	float delta_y = abs(pos.y - position.y);

	if(height / 2.0f - height / width * delta_x - delta_y >= -PIXEL_TOLERANCE)
		return true;
	return false;
}

bool IsoRect::Inside(const iPoint pos) const
{
	float delta_x = abs(pos.x - position.x);
	if (delta_x >= width / 2.0f)
		return false;

	float delta_y = abs(pos.y - position.y);

	if (height / 2.0f - height / width * delta_x - delta_y >= -PIXEL_TOLERANCE)
		return true;
	return false;
}

bool IsoRect::Overlaps(SDL_Rect rect) const
{
	bool ret = false;
	//check if SDL_Rect vertices are inside IsoRect
	if (Inside(fPoint(rect.x, rect.y)))
		ret = true;
	if(Inside(fPoint(rect.x + rect.w, rect.y)))
		ret = true;
	if(Inside(fPoint(rect.x, rect.y + rect.h)))
		ret = true;
	if(Inside(fPoint(rect.x + rect.w, rect.y + rect.h)))
		ret = true;
	if (Inside(fPoint(rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f)))
		ret = true;

	//check if IsoRect vertices are inside SDL_Rect 
	fPoint up(position.x, position.y - height / 2);	
	//top-left to down right
	if (rect.x < up.x
		&& rect.x + rect.w > up.x
		&& rect.y < up.y
		&& rect.y + rect.h > up.y)
		ret = true;

	//down-right to top-left
	if (rect.x > up.x
		&& rect.x + rect.w < up.x
		&& rect.y > up.y
		&& rect.y + rect.h < up.y)
		ret = true;

	//down-left to top-right
	if (rect.x < up.x
		&& rect.x + rect.w > up.x
		&& rect.y > up.y
		&& rect.y + rect.h < up.y)
		ret = true;

	//top-right to down-left
	if (rect.x > up.x
		&& rect.x + rect.w < up.x
		&& rect.y < up.y
		&& rect.y + rect.h > up.y)
		ret = true;

	fPoint left(position.x - width / 2, position.y);
	//top-left to down right
	if (rect.x < left.x
		&& rect.x + rect.w > left.x
		&& rect.y < left.y
		&& rect.y + rect.h > left.y)
		ret = true;

	//down-right to top-left
	if (rect.x > left.x
		&& rect.x + rect.w < left.x
		&& rect.y > left.y
		&& rect.y + rect.h < left.y)
		ret = true;

	//down-left to top-right
	if (rect.x < left.x
		&& rect.x + rect.w > left.x
		&& rect.y > left.y
		&& rect.y + rect.h < left.y)
		ret = true;

	//top-right to down-left
	if (rect.x > left.x
		&& rect.x + rect.w < left.x
		&& rect.y < left.y
		&& rect.y + rect.h > left.y)
		ret = true;

	fPoint right(position.x + width / 2, position.y);
	//top-left to down right
	if (rect.x < right.x
		&& rect.x + rect.w > right.x
		&& rect.y < right.y
		&& rect.y + rect.h > right.y)
		ret = true;

	//down-right to top-left
	if (rect.x > right.x
		&& rect.x + rect.w < right.x
		&& rect.y > right.y
		&& rect.y + rect.h < right.y)
		ret = true;

	//down-left to top-right
	if (rect.x < right.x
		&& rect.x + rect.w > right.x
		&& rect.y > right.y
		&& rect.y + rect.h < right.y)
		ret = true;

	//top-right to down-left
	if (rect.x > right.x
		&& rect.x + rect.w < right.x
		&& rect.y < right.y
		&& rect.y + rect.h > right.y)
		ret = true;

	fPoint down(position.x, position.y + height / 2);
	//top-left to down right
	if (rect.x < down.x
		&& rect.x + rect.w > down.x
		&& rect.y < down.y
		&& rect.y + rect.h > down.y)
		ret = true;

	//down-right to top-left
	if (rect.x > down.x
		&& rect.x + rect.w < down.x
		&& rect.y > down.y
		&& rect.y + rect.h < down.y)
		ret = true;

	//down-left to top-right
	if (rect.x < down.x
		&& rect.x + rect.w > down.x
		&& rect.y > down.y
		&& rect.y + rect.h < down.y)
		ret = true;

	//Center
	//top-right to down-left
	if (rect.x > down.x
		&& rect.x + rect.w < down.x
		&& rect.y < down.y
		&& rect.y + rect.h > down.y)
		ret = true;

	//top-left to down right
	if (rect.x < position.x
		&& rect.x + rect.w > position.x
		&& rect.y < position.y
		&& rect.y + rect.h > position.y)
		ret = true;

	//down-right to top-left
	if (rect.x > position.x
		&& rect.x + rect.w < position.x
		&& rect.y > position.y
		&& rect.y + rect.h < position.y)
		ret = true;

	//down-left to top-right
	if (rect.x < position.x
		&& rect.x + rect.w > position.x
		&& rect.y > position.y
		&& rect.y + rect.h < position.y)
		ret = true;

	//top-right to down-left
	if (rect.x > position.x
		&& rect.x + rect.w < position.x
		&& rect.y < position.y
		&& rect.y + rect.h > position.y)
		ret = true;

	return ret;
}

bool IsoRect::Overlaps(IsoRect rect) const
{
	fPoint first(rect.GetPosition().x, rect.GetPosition().y + rect.GetHeight()/ 2.0f);
	fPoint second(rect.GetPosition().x, rect.GetPosition().y - rect.GetHeight() / 2.0f);
	fPoint third(rect.GetPosition().x + rect.GetWidth() / 2.0f, rect.GetPosition().y);
	fPoint forth(rect.GetPosition().x - rect.GetWidth() / 2.0f, rect.GetPosition().y);
	
	if (Inside(first) || Inside(second) || Inside(third) || Inside(forth))
		return true;

	fPoint fith(GetPosition().x, GetPosition().y + GetHeight() / 2.0f);
	fPoint sixth(GetPosition().x, GetPosition().y - GetHeight() / 2.0f);
	fPoint seventh(GetPosition().x + GetWidth() / 2.0f, GetPosition().y);
	fPoint eigth(GetPosition().x - GetWidth() / 2.0f, GetPosition().y);

	if (rect.Inside(fith) || rect.Inside(sixth) || rect.Inside(seventh) || rect.Inside(eigth))
		return true;

	return false;
}

bool IsoRect::Overlaps(iPoint center, int radius)
{
	if (Inside(center))
		return true;

	float delta_x = abs(center.x - position.x);
	float delta_y = abs(center.y - position.y);

	float distance = abs(-(height / width) * delta_x - delta_y + height / 2.0f) / sqrt((height / width) * (height / width) + 1);

	return distance < radius;
}
/// ---------------------------------------------

///Class PivotedRect ----------------------------
//Constructors ==============
PivotedRect::PivotedRect(const fPoint & origin, const iPoint & goal, uint width, uint height) :Primitive(origin), goal(goal), width(width), height(height)
{

}

PivotedRect::PivotedRect(const PivotedRect & copy) : Primitive(copy), goal(copy.goal), width(copy.width), height(copy.height), pivot_distance(copy.pivot_distance)
{

}

//Destructors ===============
PivotedRect::~PivotedRect()
{

}

//Functionality =============
bool PivotedRect::Draw()
{
	App->render->DrawLine(vertex[0].x, vertex[0].y, vertex[1].x, vertex[1].y, color.r, color.g, color.b, color.a);
	App->render->DrawLine(vertex[2].x, vertex[2].y, vertex[0].x, vertex[0].y, color.r, color.g, color.b, color.a);
	App->render->DrawLine(vertex[2].x, vertex[2].y, vertex[3].x, vertex[3].y, color.r, color.g, color.b, color.a);
	App->render->DrawLine(vertex[3].x, vertex[3].y, vertex[1].x, vertex[1].y, color.r, color.g, color.b, color.a);

	return true;
}

void PivotedRect::SetGoal(const iPoint & new_goal)
{
	goal = new_goal;
}

void PivotedRect::SetWidth(uint new_width)
{
	width = new_width;
}

void PivotedRect::SetHeight(uint new_height)
{
	height = new_height;
}

void PivotedRect::SetPivotDistance(uint new_pivot_distance)
{
	pivot_distance = new_pivot_distance;
}

void PivotedRect::CalculateVertex()
{
	/*
	A-----G-----B
	-			-
	-			-
	-			-
	-			-
	-			-
	C-----M-----D
	-         -
	-   d   -
	-     -
	-   -
	- -
	P
	P is the pivot
	d is the pivot distance
	G is the goal
	*/

	//Calculate P -> G vector
	fPoint dir_vector(goal.x - this->position.x, goal.y - this->position.y);
	dir_vector.Norm();



	fPoint side_vec;
	side_vec.x = width * -dir_vector.y;
	side_vec.y = width * dir_vector.x * sin(x_angle);

	iPoint M_point = iPoint(position.x, position.y);
	M_point.x += pivot_distance * dir_vector.x;
	M_point.y += pivot_distance * dir_vector.y * sin(x_angle);

	iPoint G_Point = M_point;
	G_Point.x += height * dir_vector.x;
	G_Point.y += height * dir_vector.y;

	//Calculate A vertex
	vertex[0].x = G_Point.x - side_vec.x;
	vertex[0].y = G_Point.y - side_vec.y;

	//Calculate B vertex
	vertex[1].x = G_Point.x + side_vec.x;
	vertex[1].y = G_Point.y + side_vec.y;

	//Calculate C vertex
	vertex[2].x = M_point.x - side_vec.x;
	vertex[2].y = M_point.y - side_vec.y;

	//Calculate D vertex
	vertex[3].x = M_point.x + side_vec.x;
	vertex[3].y = M_point.y + side_vec.y;


}

bool PivotedRect::IsIn(const fPoint* loc) const
{
	float rect_area = width * height;

	// AB
	float area_tri_AB = (vertex[0].x*(vertex[1].y - loc->y) + vertex[1].x * (loc->y - vertex[0].y) + loc->x * (vertex[0].y - vertex[1].y)) * 0.5* sin(x_angle);
	// DB
	float area_tri_DB = (vertex[3].x*(vertex[1].y - loc->y) + vertex[1].x * (loc->y - vertex[3].y) + loc->x * (vertex[3].y - vertex[1].y)) * 0.5* sin(x_angle);
	// AC
	float area_tri_AC = (vertex[0].x*(vertex[2].y - loc->y) + vertex[2].x * (loc->y - vertex[0].y) + loc->x * (vertex[0].y - vertex[2].y)) * 0.5* sin(x_angle);
	// DC
	float area_tri_DC = (vertex[3].x*(vertex[2].y - loc->y) + vertex[2].x * (loc->y - vertex[3].y) + loc->x * (vertex[3].y - vertex[2].y)) * 0.5* sin(x_angle);

	return (!((abs(area_tri_AB) + abs(area_tri_AC) + abs(area_tri_DB) + abs(area_tri_DC)) > rect_area));

}
bool PivotedRect::Intersects(const SDL_Rect* rect) const
{
	if (rect == nullptr)return false;
	iPoint n_vertex[4];
	for (uint k = 0; k < 4; k++)n_vertex[k] = vertex[k];

	if (SDL_IntersectRectAndLine(rect, &n_vertex[0].x, &n_vertex[0].y, &n_vertex[1].x, &n_vertex[1].y))return true;
	if (SDL_IntersectRectAndLine(rect, &n_vertex[0].x, &n_vertex[0].y, &n_vertex[2].x, &n_vertex[2].y))return true;
	if (SDL_IntersectRectAndLine(rect, &n_vertex[2].x, &n_vertex[2].y, &n_vertex[3].x, &n_vertex[3].y))return true;
	if (SDL_IntersectRectAndLine(rect, &n_vertex[3].x, &n_vertex[3].y, &n_vertex[1].x, &n_vertex[1].y))return true;
	return false;
}
uint PivotedRect::GetWidth()const
{
	return width;
}
uint PivotedRect::GetHeight()const
{
	return height;
}
uint PivotedRect::GetPivotDistance() const
{
	return pivot_distance;
}
/// ---------------------------------------------
///Class Line -----------------------------------
//Constructors ==============
Line::Line(const fPoint & position, const iPoint & position_2, const SDL_Color& color, const iPoint& desplacement) :Primitive(position, desplacement, color), position_2(position_2)
{

}

Line::Line(const IsoRect & copy) : Primitive(position, displacement, color), position_2(position_2)
{
}

//Destructors ===============
Line::~Line()
{
}

//Functionality =============
bool Line::Draw()
{
	return App->render->DrawLine(position.x + displacement.x, position.y + displacement.y, position_2.x, position_2.y, color.r, color.g, color.b, color.a);
}

void Line::SetP1(const fPoint & p1)
{
	position = p1;
}

void Line::SetP2(const iPoint & p2)
{
	position_2 = p2;
}

const fPoint& Line::GetP1() const
{
	return position;
}

const iPoint& Line::GetP2() const
{
	return position_2;
}
