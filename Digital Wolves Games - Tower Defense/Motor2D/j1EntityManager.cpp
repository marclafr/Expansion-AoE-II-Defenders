#include "j1EntityManager.h"
#include "j1App.h"
j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("Units");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::Awake() { return true; }

bool j1EntityManager::CleanUp() { // not done

	if(sprites != nullptr)
	App->tex->UnLoad(sprites);

	std::list<Entity*>::iterator item = entity_list.begin();
	while (item != entity_list.end()) {
		RELEASE(item._Ptr->_Myval);
		item++;
	}
	entity_list.clear();

	//Borrar enemigos
	//for(uint i = 0;             ; ++i)
		//if(unit_list[i])
	return true;
}

void j1EntityManager::DeleteEntity(Entity * ptr)
{
	switch (ptr->GetEntityType())
	{
		case UNIT:
			DeleteUnit(ptr);
			break;

		case BUILDING:
			DeleteBuilding(ptr);
			break;
	}
}


bool j1EntityManager::Update() {

	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); ++it)
	{
		it._Ptr->_Myval->Update();
	}



	//a trav�s de la lista donde 
	//tenemos los enemigos ejecutamos los siguientes pasos

	//funcion de movimiento de cada enemigo

	//Draw de los enemigos

	//si han muerto y han acabado su animacion de muerte hacer delete
	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (std::list<Entity*>::iterator it = entity_list.begin(); it != entity_list.end(); ++it)
	{
		if (it._Ptr->_Myval->ToDelete() == true)
		{
			DeleteEntity(it._Ptr->_Myval);
			entity_list.erase(it);
		}
	}
	return true;
}

