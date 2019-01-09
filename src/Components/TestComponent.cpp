#include "Components/TestComponent.hpp"
#include "TryEngine.hpp"

const TEComponentType CTest::TypeID = "CTest";

CTest::CTest(std::weak_ptr<TEEntity> e): TEComponent(e), texFile(""), position(sf::Vector2i(0,0)), angle(0.0f)
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
    obj["tex"] = texFile;
    return obj;
}

void CTest::Load(Json::Value&& data)
{
    position = sf::Vector2f(data.get("x", 0).asFloat(), data.get("y", 0).asFloat());
    angle = data.get("rot", 0.0f).asFloat();
    if(auto res = TE.Resources().lock())
    {
        texFile = data.get("tex", "").asString();
        res->Register(texFile, TEResource::Type::TEXTURE, texFile.c_str());
    }
}

void CTest::Initialize()
{
    if( auto res = TE.Resources().lock())
    {
        if(texFile.empty()) 
        {
            if(auto res = TE.Resources().lock())
            {
                texFile = "Resources/gift.png";
                res->Register(texFile, TEResource::Type::TEXTURE, texFile.c_str());
            }
        }
        res->GetTexture(texFile, tex);
        sprite.setTexture(tex->Get()); 
    }
    sprite.setOrigin(sprite.getGlobalBounds().width/2, sprite.getGlobalBounds().height/2);
    sprite.setPosition(position);
    sprite.setRotation(angle);
}

void CTest::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //states.shader = &m_shader;
    target.draw(sprite, states);
}

void CTest::Tranform(const sf::Transform& transform)
{
    sprite.setPosition(transform.transformPoint(position));
    sprite.setRotation(angle);
}