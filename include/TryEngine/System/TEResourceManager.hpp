#ifndef TERO_RESOURCEMANAGER_HPP
#define TERO_RESOURCEMANAGER_HPP

#include <map>
#include <vector>
#include <memory>
#include <functional>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <iostream>

class TEResource
{
    public:
    enum class Type{
        TEXTURE,
        SHADER,
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

    virtual void Free() {};

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
    std::unique_ptr<sf::Texture> m_texture;
    public:
    TETexture(std::string location): TEResource(location), m_texture(std::make_unique<sf::Texture>()) {}
    void Load() override{
        if(m_state == State::LOADED) return;
        m_texture = std::make_unique<sf::Texture>();
        if(!m_texture->loadFromFile(m_diskLocation))
            m_state = State::NOT_FOUND;
        else m_state = State::LOADED;
    }
    void Free() override{
        m_texture.release();
        m_state = State::NOT_LOADED;
    }
    const sf::Texture& Get() const { return *m_texture; }
};

struct TEShader: public TEResource
{
    protected:
    std::unique_ptr<sf::Shader> m_shader;
    std::function<void(void)> load_fun;
    public:
    TEShader(std::string location): TEResource(location), load_fun(nullptr) {}
    void Load() override{
        if(m_state == State::LOADED || load_fun == nullptr) return;
        m_shader = std::make_unique<sf::Shader>();
        load_fun();
    }
    void Free() override{
        m_shader.release();
        m_state = State::NOT_LOADED;
    }
    void Configure(const std::string &filename, sf::Shader::Type type)
    {
        using T_ = sf::Shader::Type;
        switch(type)
        {
            case T_::Vertex:  load_fun = [this, filename]() {  
                if(!m_shader->loadFromFile(filename, sf::Shader::Vertex))
                    m_state = State::NOT_FOUND;
                else m_state = State::LOADED;
            };break;
            case T_::Geometry: load_fun = [this, filename]() {  
                if(!m_shader->loadFromFile(filename, sf::Shader::Geometry))
                    m_state = State::NOT_FOUND;
                else m_state = State::LOADED;
            };break;
            case T_::Fragment: load_fun = [this, filename]() {  
                if(!m_shader->loadFromFile(filename, sf::Shader::Fragment))
                    m_state = State::NOT_FOUND;
                else m_state = State::LOADED;
            };break;
        }
    }
    void Configure(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
    {
        load_fun = [this, vertexShaderFilename, fragmentShaderFilename]() {  
                if(!m_shader->loadFromFile(vertexShaderFilename, fragmentShaderFilename))
                    m_state = State::NOT_FOUND;
                else m_state = State::LOADED;
            };
    }
    void Configure(const std::string &vertexShaderFilename, const std::string &geometryShaderFilename, const std::string &fragmentShaderFilename)
    {
        load_fun = [this, vertexShaderFilename, geometryShaderFilename, fragmentShaderFilename]() {  
                if(!m_shader->loadFromFile(vertexShaderFilename, geometryShaderFilename, fragmentShaderFilename))
                    m_state = State::NOT_FOUND;
                else m_state = State::LOADED;
            };
    }
    sf::Shader& Get() const { return *m_shader; }
};

struct TESound: public TEResource
{
    protected:
    std::unique_ptr<sf::SoundBuffer> m_buffer;
    public:
    TESound(std::string location): TEResource(location) {}
    void Load() override{
        if(m_state == State::LOADED) return;
        m_buffer = std::make_unique<sf::SoundBuffer>();
        if(!m_buffer->loadFromFile(m_diskLocation))
            m_state = State::NOT_FOUND;
        else m_state = State::LOADED;
    }
    void Free() override{
        m_buffer.release();
        m_state = State::NOT_LOADED;
    }
    const sf::SoundBuffer& Get() const { return *m_buffer; }
};

struct TEMusic: public TEResource
{
    protected:
    std::unique_ptr<sf::Music> m_music;
    public:
    TEMusic(std::string location): TEResource(location) {}
    void Load() override{
        if(m_state == State::LOADED) return;
        m_music = std::make_unique<sf::Music>();
        if(!m_music->openFromFile(m_diskLocation))
            m_state = State::NOT_FOUND;
        else m_state = State::LOADED;
    }
    void Free() override{
        m_music.release();
        m_state = State::NOT_LOADED;
    }
    const sf::Music& Get() const { return *m_music; }
};

#define __GC(loadedResourceType, resourceMap) for(std::vector<std::string>::iterator it = loadedResourceType.begin(); it != loadedResourceType.end();)    \
                    { \
                        if(resourceMap[*it].unique()) \
                        { \
                            resourceMap[*it]->Free(); \
                            it = loadedResourceType.erase(it); \
                        } \
                        else \
                            ++it;\
                    }

class TEResourceManager
{
    public:
    TEResourceManager();

    void Register(std::string resourceID, TEResource::Type type, std::string location = "");

    void GetTexture (std::string resourceID, std::shared_ptr<TETexture>& res);
    void GetShader  (std::string resourceID, std::shared_ptr<TEShader>& res);
    void GetSound   (std::string resourceID, std::shared_ptr<TESound>& res);
    void GetMusic   (std::string resourceID, std::shared_ptr<TEMusic>& res);

    private:
    void GC();

    std::map<std::string, std::shared_ptr<TETexture>> m_textures;
    std::map<std::string, std::shared_ptr<TEShader>> m_shaders;
    std::map<std::string, std::shared_ptr<TESound>> m_sounds;
    std::map<std::string, std::shared_ptr<TEMusic>> m_musics;

    std::vector<std::string> m_loadedTextures;
    std::vector<std::string> m_loadedShaders;
    std::vector<std::string> m_loadedSounds;
    std::vector<std::string> m_loadedMusics;

    friend class TryEngine;
};

#endif