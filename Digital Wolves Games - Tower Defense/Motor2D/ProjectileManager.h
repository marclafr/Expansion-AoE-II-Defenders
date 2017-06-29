#ifndef _PROJECTILEMANAGER
#define _PROJECTILEMANAGER


#include "j1Module.h"
#include "Projectile.h"
#include "j1Textures.h"
#include "p2Point.h"
#include <vector>
#include "SDL/include/SDL_rect.h"
#include "j1Audio.h"


class ProjectileManager : public j1Module
{
private:
	std::vector<Projectile*> projectile_vector;

public:

	ProjectileManager();
	~ProjectileManager();
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	Projectile* CreateProjectile(iPoint initialpos, Entity * target, int damage, float TimeInSecs, int Startheight, int Curveheight, PROJECTILE_TYPE type);
	void DeleteProjectile(Projectile* projectile);

	int fx_cannon_shoot;

};

#endif
