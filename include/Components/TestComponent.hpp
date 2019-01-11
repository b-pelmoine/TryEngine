#ifndef TERO_COMP_TEST_HPP
#define TERO_COMP_TEST_HPP

#include "CoreMinimal.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include "System/TEResourceManager.hpp"

struct CTest : public TEComponent, public sf::Drawable {
    CTest() {}
    CTest(std::weak_ptr<TEEntity> e, bool);
    virtual ~CTest();
    static const TEComponentType TypeID;
    virtual TEComponentType Type() const noexcept override { return CTest::TypeID; }
    virtual Json::Value Serialize() const override;
    virtual void Load(Json::Value&& data) override;
    virtual void Initialize() override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
 
    std::shared_ptr<TETexture> tex;
    std::shared_ptr<TEShader> shader;
    
    public:
    sf::Sprite sprite;
};

#endif