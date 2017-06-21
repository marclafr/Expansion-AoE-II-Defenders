#include "ProjectileManager.h"
#include "Projectile.h"
#include "p2Defs.h"
#include "j1Animation.h"

bool ProjectileManager::Start()
{
	fx_cannon_shoot = App->audio->LoadFx("audio/fx/Cannon01.wav");
	return true;
}

bool ProjectileManager::Update(float dt)
{
	int size = projectile_vector.size();
	for (int i = 0; i < size; i++)
	{
		projectile_vector[i]->Update();
	}
	return true;
}

bool ProjectileManager::PostUpdate()
{
	int size = projectile_vector.size();
	for (int i = 0; i < size; i++)
	{
		if (projectile_vector[i]->GetProjectilePos() == 1 && projectile_vector[i]->GetProjectileAnim()->Finished() == true && projectile_vector[i]->floor_effect == false)
			DeleteProjectile(projectile_vector[i]);
	}
	return true;
}

bool ProjectileManager::CleanUp()
{
	std::vector<Projectile*>::iterator item = projectile_vector.begin();
	for (int i = 0; i < projectile_vector.size(); i++)
		RELEASE(projectile_vector[i]);

	projectile_vector.clear();

	return true;
}

Projectile* ProjectileManager::CreateProjectile(iPoint initialpos, Entity * target, int damage, float TimeInSecs, int Startheight, int Curveheight, PROJECTILE_TYPE type)
{
	Projectile* new_projectile = new Projectile( initialpos, target, damage, TimeInSecs, Startheight, Curveheight, type);
	projectile_vector.push_back(new_projectile);
	return new_projectile;
}

void ProjectileManager::DeleteProjectile(Projectile* projectile)
{
	for (std::vector<Projectile*>::iterator i = projectile_vector.begin(); i != projectile_vector.end();)
	{
		if (*i == projectile)
		{
			projectile_vector.erase(i);
			delete projectile;
			return;
		}
		else
			i++;
	}
}
