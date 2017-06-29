
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Map.h"
#include "Camera.h"


Camera::Camera(Camera & copy) : moving(false), destination(iPoint(0,0)), speed(0), x_movement(0), y_movement(0), zoom(0.0f), transitioning(false), alpha(0), wait(0), alpha_delta(0)
{
	view_port.x = copy.view_port.x;
	view_port.y = copy.view_port.y;
	view_port.w = copy.view_port.w;
	view_port.h = copy.view_port.h;
}

Camera::Camera(SDL_Rect & rect) : moving(false), destination(iPoint(0, 0)), speed(0), x_movement(0), y_movement(0), zoom(0.0f), transitioning(false), alpha(0), wait(0), alpha_delta(0)
{
	view_port.x = rect.x;
	view_port.y = rect.y;
	view_port.w = rect.w;
	view_port.h = rect.h;
}

void Camera::SetPosition(iPoint pos)
{
	view_port.x = pos.x;
	view_port.y = pos.y;
}

void Camera::SetSize(int w, int h)
{
	view_port.w = w;
	view_port.h = h;
}

const iPoint Camera::GetPosition() const
{
	return iPoint(view_port.x, view_port.y);
}

const int Camera::GetWidth() const
{
	return view_port.w;
}

const int Camera::GetHeight() const
{
	return view_port.h;
}

const SDL_Rect Camera::GetVP() const
{
	return view_port;
}

const iPoint Camera::GetCenter() const
{
	return iPoint((view_port.x + view_port.w / 2), (view_port.y + view_port.h / 2));
}

const float Camera::GetAlpha() const
{
	return alpha;
}

void Camera::KeyboardMove(float dt)
{
	if (App->input->GetKey(App->input->move_up) == KEY_REPEAT)
		App->render->camera->MoveUp(floor(300.0f * dt)); // 450

	if (App->input->GetKey(App->input->move_down) == KEY_REPEAT)
		App->render->camera->MoveDown(floor(300.0f * dt));

	if (App->input->GetKey(App->input->move_left) == KEY_REPEAT)
		App->render->camera->MoveLeft(floor(300.0f * dt));

	if (App->input->GetKey(App->input->move_right) == KEY_REPEAT)
		App->render->camera->MoveRight(floor(300.0f * dt));
}

void Camera::MouseMove(int x, int y, float dt)
{
	if (x < MARGIN)
		MoveLeft(300.0f * dt); //450
	if (y < MARGIN)
		MoveUp(300.0f * dt);
	if (x > view_port.w - MARGIN)
		MoveRight(300.0f * dt);
	if (y > view_port.h - MARGIN)
		MoveDown(300.0f * dt);
}

bool Camera::InsideRenderTarget(int x, int y)
{
	x = (x - view_port.w / 2.0f) * (1.0f + (zoom / 100.0f)) + view_port.w / 2.0f;

	y = (y - view_port.h / 2.0f) * (1.0f + (zoom / 100.0f)) + view_port.h / 2.0f;

	int zoomed_tile_width = App->map->data.tile_width *(1.0f + zoom / 100.0f);
	int zoomed_margin_w = W_MARGIN * (1.0f + zoom / 100.0f);

	int zoomed_tile_height = App->map->data.tile_height *(1.0f + zoom / 100.0f);
	int zoomed_margin_h = H_MARGIN * (1.0f + zoom / 100.0f);

	if (x < -(zoomed_tile_width + zoomed_margin_w))
		return false;
	if (x > view_port.w + zoomed_margin_w)
		return false; 
	if (y < -(zoomed_tile_height + zoomed_margin_h))
		return false;
	if (y > (view_port.h + zoomed_margin_h)) // IF NOT FOR TOWER HEIGHT TOWERS WOULD NOT PRINT UNTIL THEIR LOWEST PART IS INSIDE THE CAMERA
		return false;
	return true;
}

void Camera::Move(iPoint destination, int speed)
{
	moving = true;

	this->destination = destination;
	this->speed = speed;

	y_movement = abs(destination.y - view_port.y);
	x_movement = abs(destination.x - view_port.x);
}

void Camera::MoveUp(float amount)
{
	if(view_port.y + floor(amount) < CAMERA_LIMIT_UP)
		view_port.y += floor(amount);
}

void Camera::MoveLeft(float amount)
{
	if(view_port.x + floor(amount) < CAMERA_LIMIT_LEFT)
		view_port.x += floor(amount);
}

void Camera::MoveDown(float amount)
{
	if(view_port.y - floor(amount) > CAMERA_LIMIT_DOWN)
		view_port.y -= floor(amount);
}

void Camera::MoveRight(float amount)
{
	if(view_port.x - floor(amount) > CAMERA_LIMIT_RIGHT)
		view_port.x -= floor(amount);
}

void Camera::SetZoom(float zoom_percentage)
{
	zoom = zoom_percentage;
}

void Camera::ZoomIn(float percentage)
{
	zoom += percentage;
}

void Camera::ZoomOut(float percentage)
{
	zoom -= percentage;
}

SDL_Rect Camera::GetZoomedRect(const SDL_Rect &starting_rect) const
{
	SDL_Rect rect = starting_rect;

	rect.w = rect.w * (1.0f + zoom / 100.0f);

	rect.h = rect.h * (1.0f + zoom / 100.0f);
	
	rect.x = (rect.x - view_port.w / 2.0f) * (1.0f + (zoom / 100.0f)) + view_port.w / 2.0f;

	rect.y = (rect.y - view_port.h / 2.0f) * (1.0f + (zoom / 100.0f)) + view_port.h / 2.0f;

	return rect;
}

void Camera::FadeToBlack(float secs_to_black, int wait_secs)
{
	alpha = 0;
	wait = wait_secs * FPS;
	alpha_delta = 255 / (secs_to_black * FPS);
	transitioning = true;
}

void Camera::FadeToLight(float secs_to_light)
{
	alpha = 255;
	alpha_delta = 255 / (secs_to_light * FPS);
	transitioning = true;
}

void Camera::UpdateCamera()
{
	if (moving)
	{
		int total_movement = y_movement + x_movement;

		if (total_movement == 0)
		{
			moving = false;
		}
		else
		{
			if (destination.x > view_port.x)
			{
				if ((speed * (float(x_movement) / float(total_movement))) > (destination.x - view_port.x))
				{
					view_port.x = destination.x;
					moving = false;
				}
				else
					view_port.x += speed * (float(x_movement) / float(total_movement));
			}
			else
			{
				if ((speed * (float(x_movement) / float(total_movement))) > abs(destination.x - view_port.x))
				{
					view_port.x = destination.x;
					moving = false;
				}
				else
					view_port.x -= speed * (float(x_movement) / float(total_movement));
			}

			if (destination.y > view_port.y)
			{
				if ((speed * (float(y_movement) / float(total_movement))) > (destination.y - view_port.y))
				{
					view_port.y = destination.y;
					moving = false;
				}
				else
					view_port.y += speed * (float(y_movement) / float(total_movement));
			}
			else
			{
				if ((speed * (float(y_movement) / float(total_movement))) > abs(destination.y - view_port.y))
				{
					view_port.y = destination.y;
					moving = false;
				}
				else
					view_port.y -= speed * (float(x_movement) / float(total_movement));
			}
		}
	}
}

void Camera::UpdateTransitions()
{
	if (transitioning == true)
	{
		if (alpha < 255)
		{
			alpha += alpha_delta;
			if (alpha >= 255 && wait == 0)
			{
				transitioning = false;
				alpha = 255;
			}
		}

		if (alpha == 255 && wait != 0)
		{
			wait--;
			if (wait == 0)
				transitioning = false;
		}

		if (alpha > 0)
		{
			alpha -= alpha_delta;
			if (alpha <= 0)
			{
				transitioning = false;
				alpha = 0;
			}
		}

		SDL_RenderDrawRect(App->render->renderer, nullptr);
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, alpha);
		SDL_RenderFillRect(App->render->renderer, nullptr);
	}
}
