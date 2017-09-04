#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Animation.h"
#include "Units.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "ProjectileManager.h"
#include "j1Score.h"
#include "j1Scene.h"

Unit::Unit(UNIT_TYPE u_type, iPoint pos, Side side) : Entity(E_UNIT, side), position(pos), unit_type(u_type), direction(D_EAST), action(A_IDLE), changed(false), target(nullptr), unit_circle(GetPosition(), 18), path_position(0)
{
	//Add paths
	if (side == S_ENEMY)
	{
		//if the bonuses start in true they are never applied
		bonus_attack = true;
		bonus_defense = true;
	}
	else
		App->pathfinding->AddPath(&path_vec);

	switch (u_type)
	{
	case U_GOD:
		SetLife(1000);
		attack = 100;
		SetArmor(50);
		speed = 15.0f;
		rate_of_fire = 15.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		//unit_circle = Elipse(GetPosition(), 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_HEAVYCAVALRYARCHER);
		else
			SetTextureID(T_HEAVYCAVALRYARCHER);
		break;

		//INFANTRY

	case U_MILITIA:
		SetLife(43);
		attack = 6;
		SetArmor(2);
		speed = 1.35f;
		rate_of_fire = 80.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_MILITIA);
		else
			SetTextureID(T_MILITIA);
		break;

	case U_MANATARMS:
		SetLife(58);
		attack = 7;
		SetArmor(4);
		speed = 1.27f;
		rate_of_fire = 120.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_MANATARMS);
		else
			SetTextureID(T_MANATARMS);
		break;

	case U_LONGSWORDMAN:
		SetLife(118);
		attack = 11;
		SetArmor(5);
		speed = 1.28f;
		rate_of_fire = 95.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_LONGSWORDMAN);
		else
			SetTextureID(T_LONGSWORDMAN);
		break;

	case U_TWOHANDEDSWORDMAN:
		SetLife(95);
		attack = 13;
		SetArmor(7);
		speed = 1.30f;
		rate_of_fire = 100.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_TWOHANDEDSWORDMAN);
		else
			SetTextureID(T_TWOHANDEDSWORDMAN);
		break;

	case U_CHAMPION:
		SetLife(152);
		attack = 14;
		SetArmor(8);
		speed = 1.30f;
		rate_of_fire = 100.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_CHAMPION);
		else
			SetTextureID(T_CHAMPION);
		break;

	case U_SPEARMAN:
		SetLife(55);
		attack = 6;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 95.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_SPEARMAN);
		else
			SetTextureID(T_SPEARMAN);
		break;

	case U_PIKEMAN:
		SetLife(65);
		attack = 5;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 95.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_PIKEMAN);
		else
			SetTextureID(T_PIKEMAN);
		break;

		//---
		//ARCHERS

	case U_ARCHER:
		SetLife(41);
		attack = 6;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 95.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_ARCHER);
		else
			SetTextureID(T_ARCHER);
		break;


	case U_ARBALEST:
		SetLife(50);
		attack = 7;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 80.0f;
		range = MID_COMBAT_RANGE;
		unit_class = C_ARCHER;
		//unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_ARBALEST);
		else
			SetTextureID(T_ARBALEST);
		break;

	case U_CAVALRYARCHER:
		SetLife(65);
		attack = 6;
		SetArmor(2);
		speed = 1.55f;
		rate_of_fire = 85.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		//unit_circle = Elipse(GetPosition(), 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_CAVALRYARCHER);
		else
			SetTextureID(T_CAVALRYARCHER);
		break;

	case U_HEAVYCAVALRYARCHER:
		SetLife(81);
		attack = 8;
		SetArmor(4);
		speed = 1.55f;
		rate_of_fire = 120.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		//unit_circle = Elipse(GetPosition(), 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_HEAVYCAVALRYARCHER);
		else
			SetTextureID(T_HEAVYCAVALRYARCHER);
		break;

		//---
		//CAVALRY

	case U_PALADIN:
		SetLife(167);
		attack = 16;
		SetArmor(6);
		speed = 1.58f;
		rate_of_fire = 110.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_CAVALRY;
		//unit_circle = Elipse(GetPosition(), 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_PALADIN);
		else
			SetTextureID(T_PALADIN);
		break;

	case U_KNIGHT:
		SetLife(137);
		attack = 12;
		SetArmor(5);
		speed = 1.60f;
		rate_of_fire = 105.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_CAVALRY;
		//unit_circle = Elipse(GetPosition(), 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_KNIGHT);
		else
			SetTextureID(T_KNIGHT);
		break;

	case U_CAVALIER:
		SetLife(125);
		attack = 13;
		SetArmor(3);
		speed = 1.55f;
		rate_of_fire = 100.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_CAVALRY;
		//unit_circle = Elipse(GetPosition(), 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_CAVALIER);
		else
			SetTextureID(T_CAVALIER);
		break;

		//---
		//SIEGE

	case U_SIEGERAM:
		SetLife(600);
		attack = 110;
		SetArmor(4);
		speed = 0.7f;
		rate_of_fire = 210.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_SIEGE;
		//unit_circle = Elipse(GetPosition(), 30);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_SIEGERAM);
		else
			SetTextureID(T_SIEGERAM);
		break;

	case U_MANGONEL:
		SetLife(500);
		attack = 67;
		SetArmor(-5);
		speed = 0.7f;
		rate_of_fire = 300.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_SIEGE;
		//unit_circle = Elipse(GetPosition(), 30);
		SetTextureID(T_MANGONEL);
		break;

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = C_NO_CLASS;
		break;
	}

	if (unit_type == U_GOD)
		animation = new AnimationManager(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, action, direction));
	else
		animation = new AnimationManager(App->anim->GetAnimationType(ANIM_UNIT, unit_type, action, direction));

	if (unit_class == C_SIEGE)
		idle_siege = new AnimationManager(App->anim->GetAnimationType(ANIM_UNIT, unit_type, A_IDLE, direction));

}

Unit::~Unit()
{
	if (animation != nullptr)
		delete animation;
}

void Unit::Update(float dt)
{	
	AI();

	if (changed == true)
	{
		ChangeAnimation();
		changed = false;
	}

	Draw();
}

bool Unit::Move()
{
	if (position_in_tile.x == 0 && position_in_tile.y == 0)
	{
		if (position == destination)
			return true;

		if (position == path_vec[path_position])
			GetNextPathPosition();
		
		if (target == nullptr)
			EnemyInSight();

		MoveToNextTile();
	}
	else
	{
		int dir_factor_x = 0;
		int dir_factor_y = 0;

		if (position_in_tile.x != 0)
		{
			if ((position_in_tile.x > 0 && position_in_tile.x - directional_speed.x < 0)
				|| (position_in_tile.x < 0 && position_in_tile.x - directional_speed.x > 0))
				position_in_tile.x = 0;
			else
				position_in_tile.x -= directional_speed.x;
		}

		if (position_in_tile.y != 0)
		{
			if ((position_in_tile.y > 0 && position_in_tile.y - directional_speed.y < 0)
				|| (position_in_tile.y < 0 && position_in_tile.y - directional_speed.y > 0))
				position_in_tile.y = 0;
			else
				position_in_tile.y -= directional_speed.y;
		}
	}
	return false;
}

void Unit::AI()
{
	//ai_update++;

	if (slowed == true && slow_timer.ReadSec() >= SLOW_TIME)
	{
		this->speed *= SLOW_PROPORTION;
		slowed = false;
	}

	//Investigations bonuses
	//CheckUnitsBuffs();
	//----------------------

	//TODO maybe update paths once in a wile to prevent old longer paths from happening

	switch (action)
	{
	case A_IDLE:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		if (target != nullptr && InRange(target->GetPosition()))
		{
			Fight();
			break;
		}

		if (target != nullptr)
		{
			if (target->GetEntityType() == E_UNIT)
			{
				if (((Unit*)target)->range == 1 && range == 1 && ((Unit*)target)->target == this)
				{
					if (((Unit*)target)->GetAction() == A_IDLE || ((Unit*)target)->GetAction() == A_FIGHT)
						App->pathfinding->CalculateCloseCombatFightPaths(this, (Unit*)target);
				}
				else
					GoToEnemy();
			}
			GoToEnemy();
			break;
		}

		if(target == nullptr)
			EnemyInSight();

		if (GetSide() == S_ENEMY)
		{
			//target = App->scene->townhall;
			//GoTo(); //goto Townhall pos
			break;
		}

		break;

	case A_MOVE:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		if (target != nullptr && EnemyDead())
		{
			GoIdle();
			break;
		}		

		if (Move())
		{
			if (target != nullptr && InRange(target->GetPosition()))
				Fight();
			else
				GoIdle(); 
		}
			

		break;

	/*case A_APPROACH:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		if (EnemyDead())
		{
			GoIdle();
			break;
		}

		if (AproachEnemy() == true)
			StartAttack();

		break;

	case A_CENTER:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}
		
		if (DestinationFull())
		{
			GoIdle();
			break;
		}

		//if (CenterUnit() == true)
			//StartAttack();

		break;*/

	case A_FIGHT:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		if (EnemyDead())
		{
			GoIdle();
			if (unit_class == C_SIEGE)
				siege_attacked = false;
			break;
		}

		if (!InRange(target->GetPosition()))
		{
			GoIdle();
			break;
		}

		if (animation->Finished())
			DoDamage();
		/*
		if (unit_class == C_SIEGE)
		{
			if (animation->GetCurrentFrame() >= GetFrameAttack() && siege_attacked == false)
				DoDamage();
			else if (siege_attacked == true)
				if (animation->Finished())
					siege_attacked = false;
		}
		else
			if (animation->Finished())
				DoDamage();*/
		break;

	case A_DIE:

		if (animation->Finished())
		{
			action = A_DISAPPEAR;
			changed = true;
		}

		break;

	case A_DISAPPEAR:

		if (animation->Finished())
			Die();
		break;
	}
}

void Unit::Draw()
{	
	SDL_Rect rect;
	iPoint pivot;

	if (unit_class == C_SIEGE && (action == A_MOVE || action == A_APPROACH || action == A_CENTER))
	{	
		idle_siege->Update(rect, pivot);

		SetPivot(pivot.x, pivot.y);
		SetRect(rect);

		if (direction == D_NORTH_EAST || direction == D_EAST || direction == D_SOUTH_EAST)
			App->render->PushInGameSprite(App->tex->GetTexture(GetTextureID()), GetPixelPosition(), &GetRect(), SDL_FLIP_HORIZONTAL, GetPivot());
		else
			App->render->PushInGameSprite(App->tex->GetTexture(GetTextureID()), GetPixelPosition(), &GetRect(), SDL_FLIP_NONE, GetPivot());
	}

	animation->Update(rect, pivot);
	
	SetPivot(pivot.x, pivot.y);
	SetRect(rect);

	App->render->PushInGameSprite(this);

}

const float Unit::GetX() const
{
	return position.x;
}

const float Unit::GetY() const
{
	return position.y;
}

const iPoint& Unit::GetPosition() const
{
	return position;
}

const iPoint& Unit::GetPixelPosition() const
{
	iPoint ret = App->map->MapToWorld(GetPosition());
	ret.y += App->map->data.tile_height / 2.0f;
	return iPoint(ret.x - position_in_tile.x, ret.y - position_in_tile.y);
}

const DIRECTION Unit::GetDir() const
{
	return direction;
}

const UNIT_TYPE Unit::GetUnitType() const
{
	return unit_type;
}

const UNIT_CLASS Unit::GetUnitClass() const
{
	return unit_class;
}

const ACTION Unit::GetAction() const
{
	return action;
}

const int Unit::GetUnitRadius() const
{
	return unit_circle.GetRad();
}

bool Unit::GetPath(const iPoint& destination)
{
	bool ret = false;

	App->pathfinding->MakeWalkable(position);

	if (App->pathfinding->CalculatePath(GetPosition(), destination, path_vec))
		ret = true;

	App->pathfinding->MakeNoWalkable(position);

	return ret;
}

const int Unit::GetAttack() const
{
	return attack;
}

const int Unit::GetRange() const
{
	return range;
}

const Elipse Unit::GetUnitCircle() const
{
	return unit_circle;
}

const bool Unit::IsMoving() const
{
	if (action == A_MOVE)
		return true;
	return false;
}

const iPoint & Unit::GetDestination() const
{
	return destination;
}

const float Unit::FramesToTileCenter() const
{
	return MAX(position_in_tile.x / directional_speed.x, position_in_tile.y / directional_speed.y);
}

const fPoint & Unit::PixelsToTileCenter() const
{
	return position_in_tile;
}

const float Unit::GetSpeed() const
{
	return speed;
}

bool Unit::InRange(const iPoint & tile) const
{
	fPoint vector(tile.x - position.x, tile.y - position.y);
	return abs(vector.x) <= range && abs(vector.y) <= range;
}

void Unit::GetEmptyAttackPositions(std::vector<iPoint>& vec, int range) const
{
	while (range > 0)
	{
		iPoint current_tile(position.x - range, position.y - range);

		//Left
		for (int i = -range; i < range; i++)
		{
			current_tile.x = position.x + i;
			if (App->pathfinding->IsWalkable(current_tile))
				vec.push_back(current_tile);
		}
		current_tile.x++;

		//Down
		for (int i = -range; i < range; i++)
		{
			current_tile.y = position.y + i;
			if (App->pathfinding->IsWalkable(current_tile))
				vec.push_back(current_tile);
		}
		current_tile.y++;

		//Right
		for (int i = -range; i < range; i++)
		{
			current_tile.x = position.x - i;
			if (App->pathfinding->IsWalkable(current_tile))
				vec.push_back(current_tile);
		}
		current_tile.x--;

		//Up
		for (int i = -range; i < range; i++)
		{
			current_tile.y = position.y - i;
			if (App->pathfinding->IsWalkable(current_tile))
				vec.push_back(current_tile);
		}
		current_tile.y--;

		range--;
	}
}

const iPoint & Unit::FindClosestEmptyAttackTile(const Entity* target, int tile_range) const
{
	iPoint ret(-1, -1);

	if (tile_range < 1)
	{
		LOG("Tile range inferior to 1");
		return ret;
	}

	std::vector<iPoint> empty_attack_tiles;

	target->GetEmptyAttackPositions(empty_attack_tiles, range);

	if (empty_attack_tiles.size() == 0)
		return ret;

	float shortest_distance = NUM_TILES;
	float distance_to_tile = 0.0f;

	for (std::vector<iPoint>::iterator it = empty_attack_tiles.begin(); it != empty_attack_tiles.end(); ++it)
	{
		distance_to_tile = it->DistanceTo(position);
		if (shortest_distance > distance_to_tile)
		{
			shortest_distance = distance_to_tile;
			ret = *it;
		}
	}
	return ret;
}

const Unit * Unit::GetCollision() const
{
	return collision;
}

const DIRECTION Unit::GetDirection(iPoint objective) const
{
	iPoint direction_vec = objective - position;

	if (direction_vec.x < 0 && direction_vec.y < 0)
		return D_NORTH;
	if (direction_vec.x == 0 && direction_vec.y < 0)
		return D_NORTH_EAST;
	if (direction_vec.x > 0 && direction_vec.y < 0)
		return D_EAST;
	if (direction_vec.x > 0 && direction_vec.y == 0)
		return D_SOUTH_EAST;
	if (direction_vec.x > 0 && direction_vec.y > 0)
		return D_SOUTH;
	if (direction_vec.x == 0 && direction_vec.y > 0)
		return D_SOUTH_WEST;
	if (direction_vec.x < 0 && direction_vec.y > 0)
		return D_WEST;
	if (direction_vec.x < 0 && direction_vec.y == 0)
		return D_NORTH_WEST;
}

void Unit::SetAction(const ACTION action)
{
	this->action = action;
}

void Unit::LookAt(const iPoint& pos)
{
	DIRECTION dir = GetDirection(pos);
	
	if (dir != direction)
	{
		this->direction = dir;
		changed = true;
	}

}

bool Unit::GoTo(const iPoint& destination)
{
	bool ret = false;
	if (GetPath(destination) != false)
	{
		this->destination = destination;
		ret = true;
	}
	else
		this->destination = path_vec[0];

	action = A_MOVE;
	changed = true;
	path_position = path_vec.size() - 1;
	return ret;
}

bool Unit::ChangeDirection(const iPoint& destination)
{
	bool ret = false;

	if (GetPath(destination) != true)
	{
		this->destination = destination;
		ret = true;
	}
	else
		this->destination = path_vec[0];

	path_position = path_vec.size() - 1;
	return false;
}

void Unit::PlayDeathSound() const
{
	int rand_num = rand() % 5;

	switch (rand_num)
	{
	case 0:
		App->audio->PlayFx(App->audio->fx_twohanded_die01);
		break;
	case 1:
		App->audio->PlayFx(App->audio->fx_twohanded_die02);
		break;
	case 2:
		App->audio->PlayFx(App->audio->fx_twohanded_die03);
		break;
	case 3:
		App->audio->PlayFx(App->audio->fx_twohanded_die04);
		break;
	case 4:
		App->audio->PlayFx(App->audio->fx_twohanded_die05);
		break;
	}	
}

void Unit::GetNextPathPosition()
{
	path_position--;
	LookAt(path_vec[path_position]);

	iPoint distance = App->map->MapToWorld(path_vec[path_position]) - App->map->MapToWorld(position);
	if (distance.x == 0)
	{
		directional_speed.x = 0;
		if(distance.y > 0)
			directional_speed.y = speed;
		else
			directional_speed.y = -speed;
	}
	else if (distance.y == 0)
	{
		directional_speed.x = speed;
		if (distance.x > 0)
			directional_speed.x = speed;
		else
			directional_speed.x = -speed;
	}
	else
	{
		float angle = atanf((abs((float)distance.y)) / abs((float)distance.x));
		directional_speed.x = speed * cosf(angle);
		directional_speed.y = speed * sinf(angle);

		if (distance.x < 0)
			directional_speed.x *= -1.0f;
		if (distance.y < 0)
			directional_speed.y *= -1.0f;
	}
}

void Unit::MoveToNextTile()
{
	int dx = 0;
	int dy = 0;

	switch (direction)
	{
	case D_NO_DIRECTION:
		break;
	case D_NORTH:
		dx--;
		dy--;
		break;
	case D_NORTH_EAST:
		dy--;
		break;
	case D_EAST:
		dx++;
		dy--;
		break;
	case D_SOUTH_EAST:
		dx++;
		break;
	case D_SOUTH:
		dx++;
		dy++;
		break;
	case D_SOUTH_WEST:
		dy++;
		break;
	case D_WEST:
		dx--;
		dy++;
		break;
	case D_NORTH_WEST:
		dx--;
		break;
	default:
		break;
	}

	if (App->pathfinding->IsWalkable(iPoint(position.x + dx, position.y + dy)))
	{
		App->pathfinding->MakeWalkable(position);

		iPoint pos_now = App->map->MapToWorld(position);

		position.x += dx;
		position.y += dy;

		App->pathfinding->MakeNoWalkable(position);

		iPoint pos_later = App->map->MapToWorld(position);

		position_in_tile.x = pos_later.x - pos_now.x;
		position_in_tile.y = pos_later.y - pos_now.y;
	}
	else
		GoTo(destination);
}

void Unit::PlayAttackSound() const
{
	if(unit_class == C_ARCHER)
		App->audio->PlayFx(App->audio->fx_arrow);

	else if(unit_type == U_SIEGERAM)
		App->audio->PlayFx(App->audio->fx_siegeram_hit);

	else
	{
		int rand_num = rand() % 3;

		switch (rand_num)
		{
		case 0:
			App->audio->PlayFx(App->audio->fx_attack01);
			break;
		case 1:
			App->audio->PlayFx(App->audio->fx_attack02);
			break;
		case 2:
			App->audio->PlayFx(App->audio->fx_attack03);
			break;
		}
	}
}

void Unit::UnitDies()
{
	action = A_DIE;

	for (std::vector<Entity*>::iterator it = App->scene->selection.begin(); it != App->scene->selection.end(); ++it)
		if ((*it) == this)
		{
			SetEntityStatus(ST_NON_SELECTED);
			App->scene->selection.erase(it);
			break;
		}
	
	/*if (GetSide() == S_ENEMY)
		App->score->EnemyKilled();*/

	if (unit_type == U_SIEGERAM)
		App->entity_manager->DropUnits(GetX(), GetY());

	changed = true;
	PlayDeathSound();
}

bool Unit::OutOfHP() const
{
	if (GetHp() <= 0)
		return true;
	return false;
}

void Unit::EnemyInSight()
{
	App->pathfinding->MakeWalkable(position);

	Entity* ret = App->entity_manager->LookForEnemies(VISION_RANGE, GetPixelPosition(), GetSide(), this, E_UNIT);
	if (ret == nullptr)
		ret = App->entity_manager->LookForEnemies(VISION_RANGE, GetPixelPosition(), GetSide(), this);

	App->pathfinding->MakeNoWalkable(position);

	if (ret != nullptr)
	{
		target = ret;
		if (action != A_IDLE)
			GoIdle();
	}
}

void Unit::GoToEnemy()
{
	destination = FindClosestEmptyAttackTile(target, range);

	if (destination == position)
	{
		GoIdle();
		return;
	}

	if (destination.y == -1)
	{
		target = nullptr;
		GoIdle();
	}
	else
		GoTo(destination);
}

void Unit::ChangeDirecctionToEnemy()
{
	iPoint destination_tile;
	destination_tile = FindClosestEmptyAttackTile(target, range);
	if (destination_tile.y == -1)
		target = nullptr;
	else
		ChangeDirection(destination_tile);
}

void Unit::GoIdle()
{
	action = A_IDLE;
	changed = true;
}

bool Unit::DestinationFull() const
{
	if (App->pathfinding->IsWalkable(destination))
		return false;
	return true;
}

bool Unit::EnemyDead()
{
	if (target->GetHp() <= 0.0f)
	{
		target = nullptr;
		return true;
	}
	return false;
}

void Unit::DoDamage()
{
	LookAt(target->GetPosition());

	if (unit_class == C_ARCHER)
	{
		if (unit_type == U_GOD)
			App->projectile_manager->CreateProjectile(GetPixelPosition(), target, attack, 5, 30, 0, P_ICE_ARROW);
		else
			App->projectile_manager->CreateProjectile(GetPixelPosition(), target, attack, 15, 20, 0, P_BASIC_ARROW);
	}

	else if(unit_type == U_MANGONEL)
		App->projectile_manager->CreateProjectile(GetPixelPosition(), target, attack, 30, 40, 50, P_CANNONBALL);

	else
		target->Damaged(attack);

	PlayAttackSound();

	if (unit_class == C_SIEGE)
		siege_attacked = true;

	if (destination != App->map->WorldToMap(GetX(), GetY()))
		LOG("NOT EQUAL!!!!!!!");
}

/*TODO
bool Unit::AproachEnemy()
{
	SetAttackPosition();

	SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);
	unit_circle.SetPosition({ GetX(), GetY() });

	if (GetPosition().DistanceTo(target->GetPosition()) <= APPROACH)
		return true;
	return false;
}*/

/*TODO
void Unit::SetAttackPosition()
{
	if (range != CLOSE_COMBAT_RANGE || GetUnitType() == U_SIEGERAM)
		StartAttack();
	else
	{
		action = A_APPROACH;
		float delta_x = target->GetX() - GetX();
		float delta_y = target->GetY() - GetY();
		float distance = sqrtf(delta_x * delta_x + delta_y * delta_y);

		move_vector.x = delta_x * (APPROACH / distance);
		move_vector.y = delta_y * (APPROACH / distance);

		float modul = sqrt(move_vector.x*move_vector.x + move_vector.y * move_vector.y);

		move_vector.x = move_vector.x / modul;
		move_vector.y = move_vector.y / modul;
	}
}*/

void Unit::Fight()
{
	if (target->GetEntityType() == E_UNIT)
	{
		if (((Unit*)target)->range == 1 && range == 1 && ((Unit*)target)->target == this)
		{
			if (((Unit*)target)->GetAction() == A_IDLE || ((Unit*)target)->GetAction() == A_FIGHT)
			{
				action = A_FIGHT;
				LookAt(target->GetPosition());
				changed = true;
			}
		}
		else
		{
			action = A_FIGHT;
			LookAt(target->GetPosition());
			changed = true;
		}
	}
	else
	{
		action = A_FIGHT;
		LookAt(target->GetPosition());
		changed = true;
	}
}

void Unit::MoveAway()
{
	iPoint new_pos = App->pathfinding->FindNearestEmpty(this);
	if (new_pos.y == -1)
		LOG("CAN NOT FIND EMPTY POS");
	else
		GoTo(new_pos);
}

/*
void Unit::CheckUnitsBuffs()
{
	//Bonus attack
	if (bonus_attack == false)
	{
		switch (unit_class)
		{
		case C_INFANTRY:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_INFANTRY_ATTACK)) == INV_LVL_UNLOCKED)
			{
				attack += 6;
				bonus_attack = true;
			}
			break;
		case C_ARCHER:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ARCHERS_ATTACK)) == INV_LVL_UNLOCKED)
			{
				attack += 5;
				bonus_attack = true;
			}
			break;
		case C_CAVALRY:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_CAVALRY_ATTACK)) == INV_LVL_UNLOCKED)
			{
				attack += 7;
				bonus_attack = true;
			}
			break;
		}
	}
	//-------------
	//Bonus defense
	if (bonus_defense == false)
	{
		switch (unit_class)
		{
		case C_INFANTRY:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_INFANTRY_DEFENSE)) == INV_LVL_UNLOCKED)
			{
				IncreaseArmor(4);
				bonus_defense = true;
			}
			break;
		case C_ARCHER:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ARCHERS_DEFENSE)) == INV_LVL_UNLOCKED)
			{
				IncreaseArmor(3);
				bonus_defense = true;
			}
			break;
		case C_CAVALRY:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_CAVALRY_DEFENSE)) == INV_LVL_UNLOCKED)
			{
				IncreaseArmor(4);
				bonus_defense = true;
			}
			break;
		}
	}
	//-------------
}
*/

//TODO or erase
/*
void Unit::GoToTileCenter()
{
	iPoint tile = GetTile();
	iPoint tile_center = App->map->MapToWorld(tile.x, tile.y);

	action = A_CENTER;
	float delta_x = tile_center.x - GetX();
	float delta_y = tile_center.y - GetY();
	float distance = sqrtf(delta_x * delta_x + delta_y * delta_y);

	float modul = sqrt(move_vector.x*move_vector.x + move_vector.y * move_vector.y);

	move_vector.x = move_vector.x / modul;
	move_vector.y = move_vector.y / modul;
}*/

void Unit::CenterUnit()
{
	position_in_tile.x = 0;
	position_in_tile.y = 0;
}

void Unit::ChangeAnimation()
{
	if (unit_type == U_GOD)
	{
		if (action == A_CENTER || action == A_APPROACH)
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, A_MOVE, direction));
		if (action == A_FIGHT)
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, action, direction), this->rate_of_fire);
		else
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, action, direction));
	}
	else
	{
		if (action == A_CENTER || action == A_APPROACH)
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, A_MOVE, direction));
		if (action == A_FIGHT)
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, action, direction), this->rate_of_fire);
		else
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, action, direction));

		if (unit_class == C_SIEGE)
			idle_siege->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, A_IDLE, direction));;
	}
}

int Unit::GetFrameAttack()
{
	switch (unit_type)
	{
	case U_MANGONEL:
		return 2;
		break;
	case U_SIEGERAM:
		return 9;
		break;
	default:
		return 0;
		break;
	}
}

void Unit::SlowUnit()
{
	if (slowed == false)
	{
		this->speed /= SLOW_PROPORTION;
		slowed = true;
		slow_timer.Start();
	}
}

void Unit::Save(pugi::xml_node &data)
{
	pugi::xml_node build = data.child("units");
	pugi::xml_node ActualUnit = build.append_child("unit");
	ActualUnit.append_attribute("unit_type") = GetUnitType();
	ActualUnit.append_attribute("posx") = GetX();
	ActualUnit.append_attribute("posy") = GetY();
	ActualUnit.append_attribute("hp") = GetHp();
	ActualUnit.append_attribute("side") = GetSide();
}
