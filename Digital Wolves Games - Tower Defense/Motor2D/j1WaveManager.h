//TODO Activate
/*
#ifndef __j1WAVE_MANAGER_H__
#define __j1WAVE_MANAGER_H__


#include <vector>
#include "p2Point.h"
#include "j1Module.h"
#include "j1Timer.h"


#define LEFT_UP_TILE_X 47
#define LEFT_UP_TILE_y 73

#define TIME_BETWEEN_WAVES 30 //seconds
#define TIME_BETWEEN_UNITS 1 //seconds 522 317

enum UNIT_TYPE;

enum STARTING_ENEMY_POS
{
	LEFT_UP,
	LEFT_DOWN,
	RIGHT_UP,
	RIGHT_DOWN
};

struct UnitGroup
{
	UNIT_TYPE type;
	int amount;
	STARTING_ENEMY_POS start_pos;

	UnitGroup(UNIT_TYPE type, int amount, STARTING_ENEMY_POS start_pos);
	void Create() const;
	fPoint GetStartingPos(STARTING_ENEMY_POS pos) const;
};

struct Wave
{
	std::vector<UnitGroup> units_vec_left_up;
	std::vector<UnitGroup> units_vec_left_down;
	std::vector<UnitGroup> units_vec_right_up;
	std::vector<UnitGroup> units_vec_right_down;
	uint total_wave_units = 0;

	~Wave();

	void PushBack(UnitGroup unit_group, STARTING_ENEMY_POS start_pos);

};

class j1WaveManager : public j1Module
{
public:
	j1WaveManager();
	~j1WaveManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();
	int GetWaveNum();
	int GetMaxWaveNum();
	void SetWaveNum(int wave);
	bool BringNextWave();
	bool WaveEnded();
	void ResetWave();
	int SecondsUntilNextWave();

private:
	j1Timer timer;
	j1Timer delay_timer;
	int wave_num = -1;
	//l = left; u = up; r = right; d = down;
	int group_num_lu = 0;
	int group_num_ld = 0;
	int group_num_ru = 0;
	int group_num_rd = 0;
	int unit_num_lu = 0;
	int unit_num_ld = 0;
	int unit_num_ru = 0;
	int unit_num_rd = 0;
	bool spawning = false;

	//Creation place
	int creation_pos_x1 = -2112;
	int creation_pos_y1 = 1032;

	int creation_pos_x2 = -2112;
	int creation_pos_y2 = 1032;
	//Separation between units
	int space_between_x = 24;
	int space_between_y = -14;

	std::vector<Wave> waves;

	bool wave_ended = true;
	bool left_up_finished = false;
	bool left_down_finished = false;
	bool right_up_finished = false;
	bool right_down_finished = false;

	uint kills_for_next_wave = 0;
	bool all_waves_ended = false;
	bool can_bring_next_wave = true;


};

#endif // __j1WAVE_MANAGER_H__
*/