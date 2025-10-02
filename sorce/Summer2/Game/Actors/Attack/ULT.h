#pragma once
#include "Slash.h"
class ULT :
    public Slash
{
public:
    ULT(std::weak_ptr<Actor> owner);
    ~ULT();
    //“–‚½‚Á‚½Žž
    void OnCollide(const std::shared_ptr<Collidable> other)override;
};

