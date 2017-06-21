//TODO activate
/*
#ifndef __j1TUTORIAL_H__
#define __j1TUTORIAL_H__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;


enum TUTORIAL
{
	TUTORIAL_1,
	TUTORIAL_2,
	TUTORIAL_3,
	TUTORIAL_4,
	TUTORIAL_5,
	TUTORIAL_6,
	TUTORIAL_7,
	TUTORIAL_8
};

class j1Tutorial : public j1Module
{
public:

	j1Tutorial();

	// Destructor
	virtual ~j1Tutorial();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//void TutorialsProgression();

	//void TutorialReset();
	//void Tutorial1();
	//void Tutorial2();
	//void Tutorial3();
	//void Tutorial4();
	//void Tutorial5();
	//void Tutorial6();
	//void Tutorial7();

	bool tutorial = false;
	UIComponents* tutorial1;
	UILabel* text_tutorial1;
	UILabel* text1_tutorial1;
	UILabel* text2_tutorial1;
	UILabel* text3_tutorial1;

	UIComponents* tutorial2;
	UILabel* text_tutorial2;
	UILabel* text1_tutorial2;
	UILabel* text2_tutorial2;

	UIComponents* tutorial3;
	UILabel* text_tutorial3;
	UILabel* text1_tutorial3;
	UILabel* text2_tutorial3;
	UILabel* text3_tutorial3;
	UILabel* text4_tutorial3;

	UIComponents* tutorial4;
	UILabel* text_tutorial4;
	UILabel* text1_tutorial4;
	UILabel* text2_tutorial4;
	UILabel* text3_tutorial4;
	UILabel* text4_tutorial4;

	UIComponents* tutorial5;
	UILabel* text_tutorial5;
	UILabel* text1_tutorial5;
	UILabel* text2_tutorial5;
	UILabel* text3_tutorial5;

	UIComponents* tutorial6;
	UILabel* text_tutorial6;
	UILabel* text1_tutorial6;
	UILabel* text2_tutorial6;
	UILabel* text3_tutorial6;

	UIComponents* tutorial7;
	UILabel* text_tutorial7;
	UILabel* text1_tutorial7;

	bool TownHallSelected = false;
	bool PanelSelected = false;
	bool UniversitySelected = false;
	bool TowerSelected = false;
	bool InvestigationSelected = false;
	bool InvestigationDone = false;
	bool TowerUpgradeSelected = false;
	bool NextWaveButtonSelected = false;

	TUTORIAL tutorial_num = TUTORIAL_1;

private:
	bool tutorial1_part1 = false;
	bool tutorial1_part2 = false;
	bool tutorial1_part3 = false;

	bool tutorial2_part1 = false;

	bool tutorial3_part1 = false;
	bool tutorial3_part2 = false;
	bool tutorial3_part3 = false;
	bool tutorial3_part4 = false;

	bool tutorial4_part1 = false;
	bool tutorial4_part2 = false;
	bool tutorial4_part3 = false;

	bool tutorial5_part1 = false;
	bool tutorial5_part2 = false;
	bool tutorial5_part3 = false;

	bool tutorial6_part1 = false;
	bool tutorial6_part2 = false;
	bool tutorial6_part3 = false;

	bool tutorial1_deleted = false;
	bool tutorial2_deleted = false;
	bool tutorial3_deleted = false;
	bool tutorial4_deleted = false;
	bool tutorial5_deleted = false;
	bool tutorial6_deleted = false;
	bool tutorial7_deleted = false;
};

#endif // __j1TUTORIAL_H__
*/