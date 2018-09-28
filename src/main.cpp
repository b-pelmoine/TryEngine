#include "TryEngine.hpp"

int main(int argc, char **argv)
{
    TE.Launch(argc, argv);
    return 0;
}

/*#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"

#include <iostream>

#include "CoreMinimal.hpp"

struct PositionComponent : public TEComponent {
    PositionComponent(std::weak_ptr<TEEntity> e): TEComponent(e) {}
    PositionComponent() {}
    static const TEComponentType TypeID;
    virtual TEComponentType Type() const noexcept override { return PositionComponent::TypeID; }
    virtual void Destroy() override
    {
        TEComponents<PositionComponent>::RemoveFrom(Owner(), false);
    }
    int x{};
    int y{};
    void print() const { std::cout << x << " : " << y << std::endl; }
};

const TEComponentType PositionComponent::TypeID = "position";

struct PositionComponent2 : public TEComponent {
    PositionComponent2(std::weak_ptr<TEEntity> e): TEComponent(e) {}
    PositionComponent2() {}
    static const TEComponentType TypeID;
    virtual TEComponentType Type() const noexcept override { return PositionComponent2::TypeID; }
    virtual void Destroy() override
    {
        TEComponents<PositionComponent2>::RemoveFrom(Owner(), false);
    }
    int x{};
    int y{};
    void print() const { std::cout << x << " : " << y << std::endl; }
};

const TEComponentType PositionComponent2::TypeID = "position2";

class Test : public TESystem {
    public:
    Test(std::weak_ptr<PositionComponent> pos) : 
    position(pos) {}
    void Update() override {
        if(auto pos = position.lock())
            pos->print();
    }
    std::weak_ptr<PositionComponent> Position() const { return position; } ;
    private:
    std::weak_ptr<PositionComponent> position;
};

int main()
{

    TEEntities entitiesFactory;
    auto me = entitiesFactory.Create();
    auto you = entitiesFactory.Create();


    auto comp = TEComponents<PositionComponent>::AddTo(me);
    auto comp2 = TEComponents<PositionComponent2>::AddTo(me);

    Test s(comp);

    s.Update();
    
    comp.lock()->x = 12;
    comp.lock()->y = 5;

    s.Update();
    std::vector<std::weak_ptr<TEEntity>> badGuys;
    for(auto i=0; i<10; ++i) { badGuys.push_back(entitiesFactory.Create()); }
    //entitiesFactory.Remove(you); // you're killed!!!
    you = entitiesFactory.Create(); // but you're a survivor
    
    entitiesFactory.Remove(me);
    if(auto mee = me.lock())
    {
        if(auto comp3 = mee->GetComponent(PositionComponent::TypeID))
        {
            if(auto posComp = dynamic_cast<PositionComponent*> (comp3->lock().get()))
            {
                posComp->y = 42;
            }
        }
    }

    //comp.lock()->print();
    s.Update();

    const double SCALE = 30.0;
    //window
    sf::RenderWindow window(sf::VideoMode(640, 640), "SFML works!");
    window.setFramerateLimit(60);

    //falling object
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(100, 100));

    //ground
    sf::RectangleShape Gshape;
    Gshape.setSize(sf::Vector2f(800,100));
    Gshape.setFillColor(sf::Color::Green);

    //create world
    b2Vec2 gravity(0.0, 10.f);
    b2World world(gravity);

    
  
    //falling object physics
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(100 / SCALE, 100 / SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* boxBody = world.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((100.f / 2) / SCALE, (100.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.shape = &Shape;
    boxBody->CreateFixture(&FixtureDef);


    //ground physics
    b2BodyDef gBodyDef;
    gBodyDef.position = b2Vec2(0 / SCALE, 600 / SCALE);
    gBodyDef.type = b2_staticBody;
    b2Body* gBody = world.CreateBody(&gBodyDef);

    b2PolygonShape gShape;
    gShape.SetAsBox((800 / 2) / SCALE, (100 / 2) / SCALE);
    b2FixtureDef gFixtureDef;
    gFixtureDef.density = 0.f;
    gFixtureDef.shape = &gShape;
    gBody->CreateFixture(&gFixtureDef);


    while (window.isOpen())
    {
        world.Step(1 / 60.f, 8, 3);

        window.clear();



        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        for (b2Body* BodyIterator = world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
        {
            if (BodyIterator->GetType() == b2_dynamicBody)
            {

                
                shape.setOrigin(50, 50);



                shape.setPosition(BodyIterator->GetPosition().x * 30, BodyIterator->GetPosition().y * 30);




               shape.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);

               window.draw(shape);
              
               

            }
            else if (BodyIterator->GetType() == b2_staticBody)
            {
                Gshape.setOrigin(400, 50);



                Gshape.setPosition(BodyIterator->GetPosition().x * 30, BodyIterator->GetPosition().y * 30);




                Gshape.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);

                window.draw(Gshape);

            }
        }


       

        window.display();


     
     
      
    }

    return 0;
}*/