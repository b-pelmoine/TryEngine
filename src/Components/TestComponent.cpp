#include "Components/TestComponent.hpp"
#include "TryEngine.hpp"

const TEComponentType CTest::TypeID = "CTest";

CTest::CTest(std::weak_ptr<TEEntity> e, bool loading): TEComponent(e, loading)
{
    Initialize();
}

CTest::~CTest()
{}

Json::Value CTest::Serialize() const
{
    Json::Value obj(Json::objectValue);
    obj["x"] = sprite.getPosition().x;
    obj["y"] = sprite.getPosition().y;
    obj["rot"] = sprite.getRotation();
    obj["tex"] = tex->GetLocation();
    return obj;
}

void CTest::Load(Json::Value&& data)
{
    if(auto res = TE.Resources().lock())
    {
        auto texFile = data.get("tex", "").asString();
        res->Register(texFile, TEResource::Type::TEXTURE, texFile.c_str());
        res->GetTexture(texFile, tex);
        sprite.setTexture(tex->Get()); 
    }
    sprite.setPosition(sf::Vector2f(data.get("x", 0).asFloat(), data.get("y", 0).asFloat()));
    sprite.setRotation(data.get("rot", 0.0f).asFloat());
}

void CTest::Initialize()
{
    if( auto res = TE.Resources().lock())
    {
        if(!bLoadedFromFile)
        {
            res->Register("default_CTEST", TEResource::Type::TEXTURE, "Resources/gift.png");
            res->GetTexture("default_CTEST", tex);
            sprite.setTexture(tex->Get()); 
        }

        res->Register("test", TEResource::Type::SHADER);
        res->GetShader("test", shader);
        shader->Configure("Resources/shader/test.vert", "Resources/shader/test.frag");
        shader->Load();
        shader->Get().setUniform("texture", sf::Shader::CurrentTexture);
    }
    sprite.setOrigin(sprite.getGlobalBounds().width/2, sprite.getGlobalBounds().height/2);
}

void CTest::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.shader = &shader->Get();
    target.draw(sprite, states);
}