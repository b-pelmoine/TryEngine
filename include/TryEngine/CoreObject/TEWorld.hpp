#ifndef TERO_WORLD_HPP
#define TERO_WORLD_HPP

#include "CoreMinimal.hpp"

class TEWorld
{
public:
    TEWorld();
    void Load(TEEntities&& entities);

    TEEntities& Entities() const noexcept;
private:
    TEEntities m_entities;
};

#endif