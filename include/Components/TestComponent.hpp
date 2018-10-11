#ifndef TERO_COMP_TEST_HPP
#define TERO_COMP_TEST_HPP

#include "CoreMinimal.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>

struct CTest : public TEComponent, public sf::Drawable {
    CTest() { std::cout << "used" << std::endl;}
    CTest(std::weak_ptr<TEEntity> e);
    virtual ~CTest();
    static const TEComponentType TypeID;
    virtual TEComponentType Type() const noexcept override { return CTest::TypeID; }
    virtual Json::Value Serialize() const override;
    virtual void Load(Json::Value&& data) override;
    virtual void Initialize() override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Sprite sprite;
    
    public:
    sf::Vector2f position;
    float angle;
    virtual void Tranform(const sf::Transform& transform);
    static std::pair<std::string, sf::Texture> g_texture;
};

#endif