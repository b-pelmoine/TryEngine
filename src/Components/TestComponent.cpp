#include "Components/TestComponent.hpp"
#include "TryEngine.hpp"

const TEComponentType CTest::TypeID = "CTest";
std::pair<std::string, sf::Texture> CTest::g_texture;

CTest::CTest(std::weak_ptr<TEEntity> e): TEComponent(e), position(sf::Vector2i(0,0)), angle(0.0f)
{}

CTest::~CTest()
{}

Json::Value CTest::Serialize() const
{
    std::cout << "Serialized at (x=" << position.x << ", y=" << position.y << ")" << "Owner ID is: " << Owner().lock()->ID() << std::endl; 
    Json::Value obj(Json::objectValue);
    obj["x"] = position.x;
    obj["y"] = position.y;
    obj["rot"] = angle;
    obj["tex"] = g_texture.first;
    return obj;
}

void CTest::Load(Json::Value&& data)
{
    position = sf::Vector2f(data.get("x", 0).asFloat(), data.get("y", 0).asFloat());
    angle = data.get("rot", 0.0f).asFloat();
    if(!g_texture.second.loadFromFile(data.get("tex", "").asString()))
    {
        std::cout << "cannot find " << data.get("tex", "").asString() << std::endl;
    }
    g_texture.first = data.get("tex", "").asString();
}

void CTest::Initialize()
{
    sprite.setTexture(g_texture.second);
    sprite.setOrigin(sprite.getGlobalBounds().width/2, sprite.getGlobalBounds().height/2);
    sprite.setPosition(position);
    sprite.setRotation(angle);
}

void CTest::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}

void CTest::Tranform(const sf::Transform& transform)
{
    sprite.setPosition(transform.transformPoint(position));
    sprite.setRotation(angle);
}