#include "System/TEPhysics.hpp"

const b2Vec2 TEPhysics::default_gravity = b2Vec2(0.0, 10.0);

TEPhysics::TEPhysics()
{
    m_world = std::make_shared<b2World>(default_gravity);
}

void TEPhysics::Update()
{
    m_world->Step(timeStep, velocityIterations, positionIterations);
}