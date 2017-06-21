/*
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "Camera.h"
//#include "j1UIManager.h"
#include "j1Score.h"
#include "j1WaveManager.h"
#include "j1Tutorial.h"

bool j1Score::SecChange() const
{
	if (game_time.Read() / 1000.0f != (game_time.Read() - 166) / 1000.0f)
		return true;
	return false;
}

int j1Score::GetTime() const
{
	return game_time.ReadSec() + TimePassed;
}

void j1Score::GetTimer(int & mins, int & secs) const
{
	secs = WINNING_TIME - game_time.ReadSec() - TimePassed;
	mins = secs / 60;
	secs -= mins * 60;
}

bool j1Score::Update(float dt)
{
	if (SecChange())
	{
		int minutes;
		int seconds;
		char time_left[256];

		SDL_DestroyTexture(time_texture);
		GetTimer(minutes, seconds);
		sprintf_s(time_left, 256, " Time Left: %d:%d", minutes, seconds);
		time_texture = App->font->Print(time_left);

		//Wave timer
		if (App->wave_manager->WaveEnded())
		{
			SDL_DestroyTexture(wave_timer_texture);
			if (!App->tutorial->tutorial)
			{
				sprintf_s(time_left, 256, "Next Wave in: %d", App->wave_manager->SecondsUntilNextWave());
				wave_timer_texture = App->font->Print(time_left);
			}
			
		}
		//--
	}

	if (enemy_killed)
	{
		char text_num_kills[256];
		char text_score[256];

		SDL_DestroyTexture(num_kills_texture);
		SDL_DestroyTexture(score_texture);

		sprintf_s(text_num_kills, 256, " Enemies Killed: %d", enemies_killed);
		sprintf_s(text_score, 256, " Score: %d", score);

		num_kills_texture = App->font->Print(text_num_kills);
		score_texture = App->font->Print(text_score);

		enemy_killed = false;
	}

	App->render->PushUISprite(num_kills_texture, -App->render->camera->GetPosition().x + App->uimanager->GetPosRectFromInfoUI().x + 10, -App->render->camera->GetPosition().y + App->uimanager->GetPosRectFromInfoUI().y + 5);
	App->render->PushUISprite(score_texture, -App->render->camera->GetPosition().x + App->uimanager->GetPosRectFromInfoUI().x + 10, -App->render->camera->GetPosition().y + App->uimanager->GetPosRectFromInfoUI().y + 25);
	App->render->PushUISprite(time_texture, -App->render->camera->GetPosition().x + App->uimanager->GetPosRectFromInfoUI().x + 10, -App->render->camera->GetPosition().y + App->uimanager->GetPosRectFromInfoUI().y + 45);
	App->render->PushUISprite(wave_timer_texture, -App->render->camera->GetPosition().x + App->uimanager->GetPosRectFromInfoUI().x + 10, -App->render->camera->GetPosition().y + App->uimanager->GetPosRectFromInfoUI().y + 65);

	return true;
}

void j1Score::EnemyKilled()
{
	score += ENEMY_KILL_SCORE;
	enemies_killed += 1;
	enemy_killed = true;
}

void j1Score::AddScore(int score)
{
	this->score += score;
}

void j1Score::Reset()
{
	game_time.Start();
	enemies_killed = 0;
	score = 0;
}

int j1Score::GetScore() const
{
	return score;
}

void j1Score::IncreaseScore()
{
	++score;
}

void j1Score::DecreaseScore()
{
	--score;
}

int j1Score::GetEnemiesKilled() const
{
	return enemies_killed;
}

void j1Score::SetScore(int num)
{
	score = num;
}

void j1Score::SetEnemiesKilleds(int num)
{
	enemies_killed = num;
	enemy_killed = true;
}

void j1Score::SetTime(int num)
{
	TimePassed = num;
}

void j1Score::SetNullWaveTimer()
{
	char wave_num[256];
	int actual_wave_num = App->wave_manager->GetWaveNum() + 1;
	SDL_DestroyTexture(wave_timer_texture);
	sprintf_s(wave_num, 256, "Wave %i / %d", actual_wave_num, App->wave_manager->GetMaxWaveNum());
	wave_timer_texture = App->font->Print(wave_num);
}
*/