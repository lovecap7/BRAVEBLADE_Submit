#include "PurpleDinosaurStateBase.h"
#include "PurpleDinosaur.h"
#include "../../../../../General/Input.h"

PurpleDinosaurStateBase::PurpleDinosaurStateBase(std::weak_ptr<Actor> owner):
	CharacterStateBase(owner)
{
}

PurpleDinosaurStateBase::~PurpleDinosaurStateBase()
{
}

