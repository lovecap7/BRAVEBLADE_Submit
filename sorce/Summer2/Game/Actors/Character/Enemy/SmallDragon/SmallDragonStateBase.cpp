#include "SmallDragonStateBase.h"
#include "SmallDragon.h"

SmallDragonStateBase::SmallDragonStateBase(std::weak_ptr<Actor> owner):
	CharacterStateBase(owner)
{
}

SmallDragonStateBase::~SmallDragonStateBase()
{
}