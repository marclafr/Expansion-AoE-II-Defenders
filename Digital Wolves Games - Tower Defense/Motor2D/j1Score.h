//TODO activate
/*
#ifndef _SCORE
#define _SCORE

#include "j1Module.h"
#include "SDL_image\include\SDL_image.h"
#include "j1Timer.h"

#define WINNING_TIME 1500
#define ENEMY_KILL_SCORE 100

class j1Score : public j1Module
{
private:
	int enemies_killed = 0;
	int score = 0;
	bool enemy_killed = true;
	SDL_Texture* num_kills_texture = nullptr;
	SDL_Texture* score_texture = nullptr;
	SDL_Texture* time_texture = nullptr;
	SDL_Texture* wave_timer_texture = nullptr;
	int TimePassed = 0;
	j1Timer game_time;

public:
	bool Update(float dt);

	bool SecChange() const;
	int  GetTime() const;
	void GetTimer(int& mins, int& secs) const;
	void EnemyKilled();
	void AddScore(int score);
	void Reset();
	int  GetScore() const;
	void IncreaseScore();
	void DecreaseScore();
	int  GetEnemiesKilled() const;
	void SetScore(int num);
	void SetEnemiesKilleds(int num);
	void SetTime(int num);
	void SetNullWaveTimer();
};

#endif
*/
