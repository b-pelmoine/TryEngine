#ifndef TERO_PHYSICS_HPP
#define TERO_PHYSICS_HPP

#include "Box2D/Box2D.h"
#include "memory"

class TEPhysics
{
    public:
    constexpr static double scale = 30.0;
    
    TEPhysics();

    void Update();
    std::shared_ptr<b2World> World() const { return m_world; }

    private:
    std::shared_ptr<b2World> m_world;
    float32 timeStep = 1.0f / 60.f;
    int32 velocityIterations = 10;
    int32 positionIterations = 8;

    const static b2Vec2 default_gravity;
};

#endif