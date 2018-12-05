#ifndef TERO_RESOURCEMANAGER_HPP
#define TERO_RESOURCEMANAGER_HPP

#include <map>
#include <memory>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Texture.hpp>

class TEResource
{
    public:
    enum class Type{
        TEXTURE,
        SOUND,
        MUSIC,
        _Count
    };
    enum class State{
        NOT_LOADED,
        LOADED,
        NOT_FOUND
    };

    protected:
    std::string m_diskLocation;
    State m_state;

    public:
    TEResource(std::string location): m_diskLocation(location), m_state(State::NOT_LOADED) {}
    std::string GetLocation() const { return m_diskLocation; }

    virtual void Load() =0;
    State GetState() const { return m_state; }
    bool Loaded() const { return m_state == State::LOADED; }
    
};

struct TETexture: public TEResource
{
    protected:
    sf::Texture m_texture;
    public:
    TETexture(std::string location): TEResource(location) {}
    void Load() override{
        if(m_state == State::LOADED) return;
        if(!m_texture.loadFromFile(m_diskLocation))
            m_state = State::NOT_FOUND;
        else m_state = State::LOADED;
    }
    const sf::Texture& Get() const { return m_texture; }
};

struct TESound: public TEResource
{
    protected:
    sf::SoundBuffer m_buffer;
    public:
    TESound(std::string location): TEResource(location) {}
    void Load() override{
        if(m_state == State::LOADED) return;
        if(!m_buffer.loadFromFile(m_diskLocation))
            m_state = State::NOT_FOUND;
        else m_state = State::LOADED;
    }
    const sf::SoundBuffer& Get() const { return m_buffer; }
};

struct TEMusic: public TEResource
{
    protected:
    sf::Music m_music;
    public:
    TEMusic(std::string location): TEResource(location) {}
    void Load() override{
        if(m_state == State::LOADED) return;
        if(!m_music.openFromFile(m_diskLocation))
            m_state = State::NOT_FOUND;
        else m_state = State::LOADED;
    }
    const sf::Music& Get() const { return m_music; }
};

class TEResourceManager
{
    public:
    TEResourceManager();

    TEResource* Register(std::string resourceID, std::string location, TEResource::Type type);

    const TETexture&    GetTexture(std::string resourceID);
    const TESound&      GetSound(std::string resourceID);
    const TEMusic&      GetMusic(std::string resourceID);

    private:
    std::vector<std::map<std::string, std::unique_ptr<TEResource>>> m_resources;
};

#endif