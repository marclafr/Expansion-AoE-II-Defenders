
//TODO activate
/*#include "j1WaveManager.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Entity.h"
#include "j1Map.h"
#include "Units.h"
#include "j1Pathfinding.h"
#include "p2Log.h"
#include "j1Score.h"
#include "j1Scene.h"

j1WaveManager::j1WaveManager() : j1Module()
{
	name.assign("wave_manager");
}

j1WaveManager::~j1WaveManager() {}

bool j1WaveManager::Start()
{
	Wave wave1;
	UnitGroup group1(U_MILITIA, 5, LEFT_UP);
	UnitGroup group2(U_MILITIA, 5, LEFT_DOWN);
	UnitGroup group3(U_MILITIA, 5, RIGHT_UP);
	UnitGroup group4(U_MILITIA, 5, RIGHT_DOWN);
	wave1.PushBack(group1, LEFT_UP);
	wave1.PushBack(group2, LEFT_DOWN);
	wave1.PushBack(group3, RIGHT_UP);
	wave1.PushBack(group4, RIGHT_DOWN);
	wave1.total_wave_units = 20;
	Wave wave2;

	UnitGroup group5(U_MANATARMS, 10, LEFT_UP);
	UnitGroup group6(U_MANATARMS, 10, LEFT_DOWN);
	UnitGroup group7(U_MANATARMS, 10, RIGHT_UP);
	UnitGroup group8(U_MANATARMS, 10, RIGHT_DOWN);

	wave2.PushBack(group5, LEFT_UP);
	wave2.PushBack(group6, LEFT_DOWN);
	wave2.PushBack(group7, RIGHT_UP);
	wave2.PushBack(group8, RIGHT_DOWN);
	wave2.total_wave_units = 40;

	Wave wave3;
	UnitGroup group9(U_LONGSWORDMAN, 8, LEFT_UP);
	UnitGroup group10(U_LONGSWORDMAN, 8, LEFT_DOWN);
	UnitGroup group11(U_LONGSWORDMAN, 8, RIGHT_UP);
	UnitGroup group12(U_LONGSWORDMAN, 8, RIGHT_DOWN);
	UnitGroup group13(U_ARCHER, 2, LEFT_UP);
	UnitGroup group14(U_SIEGERAM, 1, LEFT_DOWN);
	UnitGroup group15(U_ARCHER, 2, RIGHT_UP);
	UnitGroup group16(U_SIEGERAM, 1, RIGHT_DOWN);
	UnitGroup group17(U_LONGSWORDMAN, 8, LEFT_UP);
	UnitGroup group18(U_LONGSWORDMAN, 8, LEFT_DOWN);
	UnitGroup group19(U_LONGSWORDMAN, 8, RIGHT_UP);
	UnitGroup group20(U_LONGSWORDMAN, 8, RIGHT_DOWN);

	wave3.PushBack(group9, LEFT_UP);
	wave3.PushBack(group10, LEFT_DOWN);
	wave3.PushBack(group11, RIGHT_UP);
	wave3.PushBack(group12, RIGHT_DOWN);
	wave3.PushBack(group13, LEFT_UP);
	wave3.PushBack(group14, LEFT_DOWN);
	wave3.PushBack(group15, RIGHT_UP);
	wave3.PushBack(group16, RIGHT_DOWN);
	wave3.PushBack(group17, LEFT_UP);
	wave3.PushBack(group18, LEFT_DOWN);
	wave3.PushBack(group19, RIGHT_UP);
	wave3.PushBack(group20, RIGHT_DOWN);
	wave3.total_wave_units = 78;

	Wave wave4;
	UnitGroup group21(U_TWOHANDEDSWORDMAN, 10, LEFT_UP);
	UnitGroup group22(U_TWOHANDEDSWORDMAN, 10, LEFT_DOWN);
	UnitGroup group23(U_TWOHANDEDSWORDMAN, 10, RIGHT_UP);
	UnitGroup group24(U_TWOHANDEDSWORDMAN, 10, RIGHT_DOWN);
	UnitGroup group25(U_CAVALRYARCHER, 3, LEFT_UP);
	UnitGroup group26(U_CAVALRYARCHER, 3, LEFT_DOWN);
	UnitGroup group27(U_CAVALRYARCHER, 3, RIGHT_UP);
	UnitGroup group28(U_CAVALRYARCHER, 3, RIGHT_DOWN);
	UnitGroup group29(U_TWOHANDEDSWORDMAN, 10, LEFT_UP);
	UnitGroup group30(U_TWOHANDEDSWORDMAN, 10, LEFT_DOWN);
	UnitGroup group31(U_TWOHANDEDSWORDMAN, 10, RIGHT_UP);
	UnitGroup group32(U_TWOHANDEDSWORDMAN, 10, RIGHT_DOWN);
	wave4.PushBack(group21, LEFT_UP);
	wave4.PushBack(group22, LEFT_DOWN);
	wave4.PushBack(group23, RIGHT_UP);
	wave4.PushBack(group24, RIGHT_DOWN);
	wave4.PushBack(group25, LEFT_UP);
	wave4.PushBack(group26, LEFT_DOWN);
	wave4.PushBack(group27, RIGHT_UP);
	wave4.PushBack(group28, RIGHT_DOWN);
	wave4.PushBack(group29, LEFT_UP);
	wave4.PushBack(group30, LEFT_DOWN);
	wave4.PushBack(group31, RIGHT_UP);
	wave4.PushBack(group32, RIGHT_DOWN);
	wave4.total_wave_units = 92;

	Wave wave5;
	UnitGroup group33(U_CHAMPION, 11, LEFT_UP);
	UnitGroup group34(U_CHAMPION, 11, LEFT_DOWN);
	UnitGroup group35(U_CHAMPION, 11, RIGHT_UP);
	UnitGroup group36(U_CHAMPION, 11, RIGHT_DOWN);
	UnitGroup group37(U_MANGONEL, 3, LEFT_UP);
	UnitGroup group38(U_SIEGERAM, 3, LEFT_DOWN);
	UnitGroup group39(U_SIEGERAM, 3, RIGHT_UP);
	UnitGroup group40(U_MANGONEL, 3, RIGHT_DOWN);
	UnitGroup group41(U_CHAMPION, 11, LEFT_UP);
	UnitGroup group42(U_CHAMPION, 11, LEFT_DOWN);
	UnitGroup group43(U_CHAMPION, 11, RIGHT_UP);
	UnitGroup group44(U_CHAMPION, 11, RIGHT_DOWN);
	wave5.PushBack(group33, LEFT_UP);
	wave5.PushBack(group34, LEFT_DOWN);
	wave5.PushBack(group35, RIGHT_UP);
	wave5.PushBack(group36, RIGHT_DOWN);
	wave5.PushBack(group37, LEFT_UP);
	wave5.PushBack(group38, LEFT_DOWN);
	wave5.PushBack(group39, RIGHT_UP);
	wave5.PushBack(group40, RIGHT_DOWN);
	wave5.PushBack(group41, LEFT_UP);
	wave5.PushBack(group42, LEFT_DOWN);
	wave5.PushBack(group43, RIGHT_UP);
	wave5.PushBack(group44, RIGHT_DOWN);
	wave5.total_wave_units = 124;



	Wave wave6;
	UnitGroup group45(U_PALADIN, 10, LEFT_UP);
	UnitGroup group46(U_SIEGERAM, 3, LEFT_DOWN);
	UnitGroup group47(U_SIEGERAM, 3, RIGHT_UP);
	UnitGroup group48(U_PALADIN, 10, RIGHT_DOWN);
	UnitGroup group49(U_KNIGHT, 12, LEFT_UP);
	UnitGroup group50(U_MANGONEL, 2, LEFT_DOWN);
	UnitGroup group51(U_MANGONEL, 2, RIGHT_UP);
	UnitGroup group52(U_KNIGHT, 12, RIGHT_DOWN);
	UnitGroup group53(U_TWOHANDEDSWORDMAN, 12, LEFT_UP);
	UnitGroup group54(U_TWOHANDEDSWORDMAN, 12, LEFT_DOWN);
	UnitGroup group55(U_TWOHANDEDSWORDMAN, 12, RIGHT_UP);
	UnitGroup group56(U_TWOHANDEDSWORDMAN, 12, RIGHT_DOWN);
	UnitGroup group57(U_CAVALRYARCHER, 2, LEFT_UP);
	UnitGroup group58(U_CAVALRYARCHER, 2, LEFT_DOWN);
	UnitGroup group59(U_CAVALRYARCHER, 2, RIGHT_UP);
	UnitGroup group60(U_CAVALRYARCHER, 2, RIGHT_DOWN);


	wave6.PushBack(group45, LEFT_UP);
	wave6.PushBack(group46, LEFT_DOWN);
	wave6.PushBack(group47, RIGHT_UP);
	wave6.PushBack(group48, RIGHT_DOWN);
	wave6.PushBack(group49, LEFT_UP);
	wave6.PushBack(group50, LEFT_DOWN);
	wave6.PushBack(group51, RIGHT_UP);
	wave6.PushBack(group52, RIGHT_DOWN);
	wave6.PushBack(group53, LEFT_UP);
	wave6.PushBack(group54, LEFT_DOWN);
	wave6.PushBack(group55, RIGHT_UP);
	wave6.PushBack(group56, RIGHT_DOWN);
	wave6.PushBack(group57, LEFT_UP);
	wave6.PushBack(group58, LEFT_DOWN);
	wave6.PushBack(group59, RIGHT_UP);
	wave6.PushBack(group60, RIGHT_DOWN);
	wave6.total_wave_units = 134;

	Wave wave7;
	UnitGroup group61(U_CHAMPION, 10, LEFT_UP);
	UnitGroup group62(U_CHAMPION, 10, LEFT_DOWN);
	UnitGroup group63(U_CHAMPION, 10, RIGHT_UP);
	UnitGroup group64(U_CHAMPION, 10, RIGHT_DOWN);
	UnitGroup group65(U_HEAVYCAVALRYARCHER, 3, LEFT_UP);
	UnitGroup group66(U_HEAVYCAVALRYARCHER, 3, LEFT_DOWN);
	UnitGroup group67(U_HEAVYCAVALRYARCHER, 3, RIGHT_UP);
	UnitGroup group68(U_HEAVYCAVALRYARCHER, 3, RIGHT_DOWN);
	UnitGroup group69(U_PALADIN, 10, LEFT_UP);
	UnitGroup group70(U_PALADIN, 10, LEFT_DOWN);
	UnitGroup group71(U_PALADIN, 10, RIGHT_UP);
	UnitGroup group72(U_PALADIN, 10, RIGHT_DOWN);
	wave7.PushBack(group61, LEFT_UP);
	wave7.PushBack(group62, LEFT_DOWN);
	wave7.PushBack(group63, RIGHT_UP);
	wave7.PushBack(group64, RIGHT_DOWN);
	wave7.PushBack(group65, LEFT_UP);
	wave7.PushBack(group66, LEFT_DOWN);
	wave7.PushBack(group67, RIGHT_UP);
	wave7.PushBack(group68, RIGHT_DOWN);
	wave7.PushBack(group69, LEFT_UP);
	wave7.PushBack(group70, LEFT_DOWN);
	wave7.PushBack(group71, RIGHT_UP);
	wave7.PushBack(group72, RIGHT_DOWN);
	wave7.total_wave_units = 92;

	Wave wave8;
	UnitGroup group73(U_HEAVYCAVALRYARCHER, 2, LEFT_UP);
	UnitGroup group74(U_HEAVYCAVALRYARCHER, 2, LEFT_DOWN);
	UnitGroup group75(U_HEAVYCAVALRYARCHER, 2, RIGHT_UP);
	UnitGroup group76(U_HEAVYCAVALRYARCHER, 2, RIGHT_DOWN);
	UnitGroup group77(U_KNIGHT, 12, LEFT_UP);
	UnitGroup group78(U_KNIGHT, 12, LEFT_DOWN);
	UnitGroup group79(U_KNIGHT, 12, RIGHT_UP);
	UnitGroup group80(U_KNIGHT, 12, RIGHT_DOWN);
	UnitGroup group81(U_CAVALRYARCHER, 2, LEFT_UP);
	UnitGroup group82(U_CAVALRYARCHER, 2, LEFT_DOWN);
	UnitGroup group83(U_CAVALRYARCHER, 2, RIGHT_UP);
	UnitGroup group84(U_CAVALRYARCHER, 2, RIGHT_DOWN);
	UnitGroup group85(U_CHAMPION, 12, LEFT_UP);
	UnitGroup group86(U_SIEGERAM, 3, LEFT_DOWN);
	UnitGroup group87(U_CHAMPION, 12, RIGHT_UP);
	UnitGroup group88(U_SIEGERAM, 3, RIGHT_DOWN);
	UnitGroup group89(U_MANGONEL, 3, LEFT_UP);
	UnitGroup group90(U_SIEGERAM, 3, LEFT_DOWN);
	UnitGroup group91(U_SIEGERAM, 3, RIGHT_UP);
	UnitGroup group92(U_MANGONEL, 3, RIGHT_DOWN);

	wave8.PushBack(group73, LEFT_UP);
	wave8.PushBack(group74, LEFT_DOWN);
	wave8.PushBack(group75, RIGHT_UP);
	wave8.PushBack(group76, RIGHT_DOWN);
	wave8.PushBack(group77, LEFT_UP);
	wave8.PushBack(group78, LEFT_DOWN);
	wave8.PushBack(group79, RIGHT_UP);
	wave8.PushBack(group80, RIGHT_DOWN);
	wave8.PushBack(group81, LEFT_UP);
	wave8.PushBack(group82, LEFT_DOWN);
	wave8.PushBack(group83, RIGHT_UP);
	wave8.PushBack(group84, RIGHT_DOWN);
	wave8.PushBack(group85, LEFT_UP);
	wave8.PushBack(group86, LEFT_DOWN);
	wave8.PushBack(group87, RIGHT_UP);
	wave8.PushBack(group88, RIGHT_DOWN);
	wave8.PushBack(group89, LEFT_UP);
	wave8.PushBack(group90, LEFT_DOWN);
	wave8.PushBack(group91, RIGHT_UP);
	wave8.PushBack(group92, RIGHT_DOWN);
	wave8.total_wave_units = 154;

	Wave wave9;
	UnitGroup group93(U_MANGONEL, 2, LEFT_UP);
	UnitGroup group94(U_SIEGERAM, 5, LEFT_DOWN);
	UnitGroup group95(U_MANGONEL, 2, RIGHT_UP);
	UnitGroup group96(U_SIEGERAM, 5, RIGHT_DOWN);
	UnitGroup group97(U_CAVALRYARCHER, 10, LEFT_UP);
	UnitGroup group98(U_CAVALRYARCHER, 10, LEFT_DOWN);
	UnitGroup group99(U_CAVALRYARCHER, 10, RIGHT_UP);
	UnitGroup group100(U_CAVALRYARCHER, 10, RIGHT_DOWN);
	UnitGroup group101(U_SIEGERAM, 5, LEFT_UP);
	UnitGroup group102(U_MANGONEL, 2, LEFT_DOWN);
	UnitGroup group103(U_SIEGERAM, 5, RIGHT_UP);
	UnitGroup group104(U_MANGONEL, 2, RIGHT_DOWN);
	UnitGroup group105(U_HEAVYCAVALRYARCHER, 2, LEFT_UP);
	UnitGroup group106(U_HEAVYCAVALRYARCHER, 2, LEFT_DOWN);
	UnitGroup group107(U_HEAVYCAVALRYARCHER, 2, RIGHT_UP);
	UnitGroup group108(U_HEAVYCAVALRYARCHER, 2, RIGHT_DOWN);
	UnitGroup group109(U_SIEGERAM, 5, LEFT_UP);
	UnitGroup group110(U_MANGONEL, 2, LEFT_DOWN);
	UnitGroup group111(U_MANGONEL, 2, RIGHT_UP);
	UnitGroup group112(U_SIEGERAM, 5, RIGHT_DOWN);

	wave9.PushBack(group93, LEFT_UP);
	wave9.PushBack(group94, LEFT_DOWN);
	wave9.PushBack(group95, RIGHT_UP);
	wave9.PushBack(group96, RIGHT_DOWN);
	wave9.PushBack(group97, LEFT_UP);
	wave9.PushBack(group98, LEFT_DOWN);
	wave9.PushBack(group99, RIGHT_UP);
	wave9.PushBack(group100, RIGHT_DOWN);
	wave9.PushBack(group101, LEFT_UP);
	wave9.PushBack(group102, LEFT_DOWN);
	wave9.PushBack(group103, RIGHT_UP);
	wave9.PushBack(group104, RIGHT_DOWN);
	wave9.PushBack(group105, LEFT_UP);
	wave9.PushBack(group106, LEFT_DOWN);
	wave9.PushBack(group107, RIGHT_UP);
	wave9.PushBack(group108, RIGHT_DOWN);
	wave9.PushBack(group109, LEFT_UP);
	wave9.PushBack(group110, LEFT_DOWN);
	wave9.PushBack(group111, RIGHT_UP);
	wave9.PushBack(group112, RIGHT_DOWN);
	wave9.total_wave_units = 222;
	
	Wave wave10;
	UnitGroup group113(U_PALADIN, 15, LEFT_UP);
	UnitGroup group114(U_CHAMPION, 15, LEFT_DOWN);
	UnitGroup group115(U_CHAMPION, 15, RIGHT_UP);
	UnitGroup group116(U_PALADIN, 15, RIGHT_DOWN);
	UnitGroup group117(U_SIEGERAM, 5, LEFT_UP);
	UnitGroup group118(U_MANGONEL, 3, LEFT_DOWN);
	UnitGroup group119(U_MANGONEL, 5, RIGHT_UP);
	UnitGroup group120(U_SIEGERAM, 3, RIGHT_DOWN);
	UnitGroup group121(U_KNIGHT, 15, LEFT_UP);
	UnitGroup group122(U_CHAMPION, 15, LEFT_DOWN);
	UnitGroup group123(U_CHAMPION, 15, RIGHT_UP);
	UnitGroup group124(U_KNIGHT, 15, RIGHT_DOWN);

	wave10.PushBack(group113, LEFT_UP);
	wave10.PushBack(group114, LEFT_DOWN);
	wave10.PushBack(group115, RIGHT_UP);
	wave10.PushBack(group116, RIGHT_DOWN);
	wave10.PushBack(group117, LEFT_UP);
	wave10.PushBack(group118, LEFT_DOWN);
	wave10.PushBack(group119, RIGHT_UP);
	wave10.PushBack(group120, RIGHT_DOWN);
	wave10.PushBack(group121, LEFT_UP);
	wave10.PushBack(group122, LEFT_DOWN);
	wave10.PushBack(group123, RIGHT_UP);
	wave10.PushBack(group124, RIGHT_DOWN);
	wave10.total_wave_units = 168;

	Wave wave11;
	UnitGroup group125(U_PALADIN, 15, LEFT_UP);
	UnitGroup group126(U_CHAMPION, 15, LEFT_DOWN);
	UnitGroup group127(U_CHAMPION, 15, RIGHT_UP);
	UnitGroup group128(U_PALADIN, 15, RIGHT_DOWN);
	UnitGroup group129(U_SIEGERAM, 5, LEFT_UP);
	UnitGroup group130(U_MANGONEL, 4, LEFT_DOWN);
	UnitGroup group131(U_MANGONEL, 5, RIGHT_UP);
	UnitGroup group132(U_SIEGERAM, 4, RIGHT_DOWN);
	UnitGroup group133(U_KNIGHT, 15, LEFT_UP);
	UnitGroup group134(U_CHAMPION, 15, LEFT_DOWN);
	UnitGroup group135(U_CHAMPION, 15, RIGHT_UP);
	UnitGroup group136(U_KNIGHT, 15, RIGHT_DOWN);

	wave11.PushBack(group125, LEFT_UP);
	wave11.PushBack(group126, LEFT_DOWN);
	wave11.PushBack(group127, RIGHT_UP);
	wave11.PushBack(group128, RIGHT_DOWN);
	wave11.PushBack(group129, LEFT_UP);
	wave11.PushBack(group130, LEFT_DOWN);
	wave11.PushBack(group131, RIGHT_UP);
	wave11.PushBack(group132, RIGHT_DOWN);
	wave11.PushBack(group133, LEFT_UP);
	wave11.PushBack(group134, LEFT_DOWN);
	wave11.PushBack(group135, RIGHT_UP);
	wave11.PushBack(group136, RIGHT_DOWN);
	wave11.total_wave_units = 174;

	Wave wave12;
	UnitGroup group137(U_PALADIN, 15, LEFT_UP);
	UnitGroup group138(U_CHAMPION, 15, LEFT_DOWN);
	UnitGroup group139(U_CHAMPION, 15, RIGHT_UP);
	UnitGroup group140(U_PALADIN, 15, RIGHT_DOWN);
	UnitGroup group141(U_SIEGERAM, 5, LEFT_UP);
	UnitGroup group142(U_MANGONEL, 5, LEFT_DOWN);
	UnitGroup group143(U_MANGONEL, 5, RIGHT_UP);
	UnitGroup group144(U_SIEGERAM, 5, RIGHT_DOWN);
	UnitGroup group145(U_KNIGHT, 15, LEFT_UP);
	UnitGroup group146(U_CHAMPION, 15, LEFT_DOWN);
	UnitGroup group147(U_CHAMPION, 15, RIGHT_UP);
	UnitGroup group148(U_KNIGHT, 15, RIGHT_DOWN);

	wave12.PushBack(group137, LEFT_UP);
	wave12.PushBack(group138, LEFT_DOWN);
	wave12.PushBack(group139, RIGHT_UP);
	wave12.PushBack(group140, RIGHT_DOWN);
	wave12.PushBack(group141, LEFT_UP);
	wave12.PushBack(group142, LEFT_DOWN);
	wave12.PushBack(group143, RIGHT_UP);
	wave12.PushBack(group144, RIGHT_DOWN);
	wave12.PushBack(group145, LEFT_UP);
	wave12.PushBack(group146, LEFT_DOWN);
	wave12.PushBack(group147, RIGHT_UP);
	wave12.PushBack(group148, RIGHT_DOWN);
	wave12.total_wave_units = 180;


	waves.push_back(wave1);
	waves.push_back(wave2);
	waves.push_back(wave3);
	waves.push_back(wave4);
	waves.push_back(wave5);
	waves.push_back(wave6);
	waves.push_back(wave7);
	waves.push_back(wave8);
	waves.push_back(wave9);
	waves.push_back(wave10);
	waves.push_back(wave11);
	waves.push_back(wave12);

	timer.Start();
	spawning = true;
	group_num_lu = 0;
	group_num_ld = 0;
	group_num_ru = 0;
	group_num_rd = 0;
	unit_num_lu = 0;
	unit_num_ld = 0;
	unit_num_ru = 0;
	unit_num_rd = 0;
	kills_for_next_wave = 0;
	all_waves_ended = false;
	can_bring_next_wave = true;
	wave_ended = true;
	return true;
}

bool j1WaveManager::Update(float dt)
{
	if (all_waves_ended == false)
	{
		if (timer.ReadSec() >= TIME_BETWEEN_WAVES && wave_ended == true && can_bring_next_wave == true)
		{
			timer.Start();
			wave_num++;

			if (waves[wave_num].units_vec_left_up.size() <= 0)
				left_up_finished = true;

			if (waves[wave_num].units_vec_left_down.size() <= 0)
				left_down_finished = true;

			if (waves[wave_num].units_vec_right_up.size() <= 0)
				right_up_finished = true;

			if (waves[wave_num].units_vec_right_down.size() <= 0)
				right_down_finished = true;

			group_num_lu = 0;
			group_num_ld = 0;
			group_num_ru = 0;
			group_num_rd = 0;
			unit_num_lu = 0;
			unit_num_ld = 0;
			unit_num_ru = 0;
			unit_num_rd = 0;
			spawning = true;
			wave_ended = false;
			App->score->SetNullWaveTimer();
			can_bring_next_wave = false;
			kills_for_next_wave += waves[wave_num].total_wave_units;
			App->scene->bring_next_wave = true;
		}

		if (spawning)
		{
			if (delay_timer.ReadSec() >= TIME_BETWEEN_UNITS && wave_ended == false)
			{
				delay_timer.Start();
				//LEFT_UP
				if (wave_num < waves.size() && left_up_finished == false && waves[wave_num].units_vec_left_up.size() > 0)
				{
					if (unit_num_lu < waves[wave_num].units_vec_left_up[group_num_lu].amount)
					{
						waves[wave_num].units_vec_left_up[group_num_lu].Create();
						unit_num_lu++;
					}
					else
					{
						if (group_num_lu < waves[wave_num].units_vec_left_up.size() - 1)
						{
							group_num_lu++;
							unit_num_lu = 0;
						}
						else
							left_up_finished = true;
					}
				}
				//--
				//LEFT_DOWN
				if (wave_num < waves.size() && left_down_finished == false && waves[wave_num].units_vec_left_down.size() > 0)
				{
					if (unit_num_ld < waves[wave_num].units_vec_left_down[group_num_ld].amount)
					{
						waves[wave_num].units_vec_left_down[group_num_ld].Create();
						unit_num_ld++;
					}
					else
					{
						if (group_num_ld < waves[wave_num].units_vec_left_down.size() - 1)
						{
							group_num_ld++;
							unit_num_ld = 0;
						}
						else
							left_down_finished = true;
					}
				}
				//--
				//RIGHT_UP
				if (wave_num < waves.size() && right_up_finished == false && waves[wave_num].units_vec_right_up.size() > 0)
				{
					if (unit_num_ru < waves[wave_num].units_vec_right_up[group_num_ru].amount)
					{
						waves[wave_num].units_vec_right_up[group_num_ru].Create();
						unit_num_ru++;
					}
					else
					{
						if (group_num_ru < waves[wave_num].units_vec_right_up.size() - 1)
						{
							group_num_ru++;
							unit_num_ru = 0;
						}
						else
							right_up_finished = true;
					}
				}
				//--
				//RIGHT_DOWN
				if (wave_num < waves.size() && right_down_finished == false && waves[wave_num].units_vec_right_down.size() > 0)
				{
					if (unit_num_rd < waves[wave_num].units_vec_right_down[group_num_rd].amount)
					{
						waves[wave_num].units_vec_right_down[group_num_rd].Create();
						unit_num_rd++;
					}
					else
					{
						if (group_num_rd < waves[wave_num].units_vec_right_down.size() - 1)
						{
							group_num_rd++;
							unit_num_rd = 0;
						}
						else
							right_down_finished = true;
					}
				}
				//IF ALL PATHS ENDED
				if (left_up_finished == true && left_down_finished == true
					&& right_up_finished == true && right_down_finished == true)
				{
					if (wave_num > waves.size())
						all_waves_ended = true;
					else
					{
						left_up_finished = false;
						left_down_finished = false;
						right_up_finished = false;
						right_down_finished = false;
						spawning = false;
						wave_ended = true;
					}
				}
				//--
			}
		}
		else
		{
			if (App->score->GetEnemiesKilled() >= kills_for_next_wave && can_bring_next_wave == false)
			{
				App->SaveGame("save_game.xml");
				timer.Start();
				can_bring_next_wave = true;
			}
		}
	}
	return true;
}

bool j1WaveManager::CleanUp()
{
	wave_num = -1;
	waves.clear();
	return true;
}

int j1WaveManager::GetWaveNum()
{
	return wave_num;
}

int j1WaveManager::GetMaxWaveNum()
{
	return waves.size();
}

void j1WaveManager::SetWaveNum(int wave)
{
	wave_num = wave;
	timer.Start();
}

Wave::~Wave()
{
	units_vec_left_up.clear();
	units_vec_left_down.clear();
	units_vec_right_up.clear();
	units_vec_right_down.clear();
}

void Wave::PushBack(UnitGroup unit_group, STARTING_ENEMY_POS start_pos)
{
	switch (start_pos)
	{
	case LEFT_UP:
		units_vec_left_up.push_back(unit_group);
		break;
	case LEFT_DOWN:
		units_vec_left_down.push_back(unit_group);
		break;
	case RIGHT_UP:
		units_vec_right_up.push_back(unit_group);
		break;
	case RIGHT_DOWN:
		units_vec_right_down.push_back(unit_group);
		break;
	}
}

fPoint UnitGroup::GetStartingPos(STARTING_ENEMY_POS pos) const
{
	iPoint spawn;
	switch (pos)
	{
	case LEFT_UP:
		spawn = App->map->MapToWorld(47,73);
		break;
	case LEFT_DOWN:
		spawn = App->map->MapToWorld(49, 73);
		break;
	case RIGHT_UP:
		spawn = App->map->MapToWorld(64, 73);
		break;
	case RIGHT_DOWN:
		spawn = App->map->MapToWorld(66, 73);
		break;
	default:
		break;
	}
	return fPoint(spawn.x, spawn.y);
}

UnitGroup::UnitGroup(UNIT_TYPE type, int amount, STARTING_ENEMY_POS start_pos) : type(type), amount(amount), start_pos(start_pos)
{}

void UnitGroup::Create() const
{
	App->entity_manager->CreateUnit(type, GetStartingPos(start_pos), S_ENEMY);
}

bool j1WaveManager::BringNextWave()
{
	if (can_bring_next_wave == true)
	{
		timer.SetTicks(timer.Read() + (TIME_BETWEEN_WAVES * 5000));
		return true;
	}
	return false;
}

bool j1WaveManager::WaveEnded()
{
	return (wave_ended && can_bring_next_wave);
}

void j1WaveManager::ResetWave()
{
	group_num_lu = 0;
	group_num_ld = 0;
	group_num_ru = 0;
	group_num_rd = 0;
	unit_num_lu = 0;
	unit_num_ld = 0;
	unit_num_ru = 0;
	unit_num_rd = 0;
	kills_for_next_wave = 0;
	all_waves_ended = false;
	can_bring_next_wave = true;
	wave_ended = true;
	timer.Start();
}

int j1WaveManager::SecondsUntilNextWave()
{
	return (TIME_BETWEEN_WAVES - (int)timer.ReadSec());
}
*/