#ifndef _ENTITY
#define _ENTITY

#include "SDL\include\SDL.h"
#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
enum TextureID;

enum ENTITY_TYPE
{
	E_NO_ENTITY = 0,
	E_BUILDING,
	E_UNIT,
	E_RESOURCE,
};

enum Side
{
	S_ALLY,
	S_ENEMY,
	S_NEUTRAL,
	S_NO_SIDE
};

enum ENTITY_STATUS
{
	ST_SELECTED = 0,
	ST_NON_SELECTED
};

class Entity
{
protected:
	//todo see if all are necesary
	SDL_Rect rect;
	iPoint pivot;
	bool to_delete = false;
	int max_hp = 0;
	int current_hp = 0;
	int armor = 0;
	int attack = 0;
	enum ENTITY_TYPE entity_type;
	enum ENTITY_STATUS entity_status = ST_NON_SELECTED;
	enum TextureID texture_id;
	enum Side side;
	float Arrow_pos = 0; //It goes from 0 to 1
	float ai_dt = 0;

public:
	Entity(ENTITY_TYPE entity_type, Side side);
	~Entity();

	//Main
	virtual void Update(float dt) = 0;
	virtual void AI() = 0;
	virtual void Draw() = 0;

	//Usefull
	void Kill();
	virtual void Die();//to_delete = true
	bool ToDelete() const;
	void IncreaseArmor(int extra_defense);
	void Attack(Entity*);
	void Damaged(int dmg);
	void UpgradeUnit(int plushealth);

	//Geters
	virtual const iPoint& GetPixelPosition() const = 0;
	bool Inside(SDL_Rect rect) const;	
	ENTITY_TYPE GetEntityType() const;
	ENTITY_STATUS GetEntityStatus();
	const int GetHp() const;
	const int GetArmor() const;
	virtual const iPoint& GetPosition() const = 0;
	virtual const int GetAttack() const;
	const iPoint GetPivot() const;
	const SDL_Rect GetRect() const;
	const TextureID GetTextureID() const;
	const Side GetSide() const;
	const float GetAIDT() const;
	const SDL_Rect GetInWorldTextureRect() const;

	//Seters
protected:
	void SetArmor(int new_armor);
	void SetHp(int new_hp);
	void SetMaxHp(const int new_max_hp);
	void SetLife(const int hp);
	void SetAttack(int new_attack);
	void SetPivot(int x, int y);
	void SetRect(SDL_Rect rect);
	void SetTextureID(TextureID id);
	void SetSide(Side side);
	void SetToDelete();

public:
	void SetEntityStatus(ENTITY_STATUS status);

	//Arrow, SHOULD UPDATE ITSELVE
	float GetArrowPos() const;
	void ResetArrowPos();
	void UpdateArrow(int start_height, iPoint target_pos, int curve_height, float time_secs);

	//Extras
	//void DrawPointMinimap();
	//virtual void Save(pugi::xml_node &node) const = 0;


};
#endif
