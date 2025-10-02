#include "BomberStateBase.h"

BomberStateBase::BomberStateBase(std::weak_ptr<Actor> owner):
	CharacterStateBase(owner)
{
}

BomberStateBase::~BomberStateBase()
{
}
