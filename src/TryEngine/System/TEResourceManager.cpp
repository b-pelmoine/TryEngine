#include "System/TEResourceManager.hpp"

TEResourceManager::TEResourceManager()
{}

void TEResourceManager::Register(std::string resourceID, TEResource::Type type, std::string location)
{
    using T_ = TEResource::Type;
    switch(type)
    {
        case T_::TEXTURE :  
        {
            auto it = m_textures.find(resourceID);
            if(it != m_textures.end()) return;
            m_textures[resourceID] = std::make_shared<TETexture>(location); 
        }break;
        case T_::SHADER :   
        {
            auto it = m_shaders.find(resourceID);
            if(it != m_shaders.end()) return;
            m_shaders[resourceID] = std::make_shared<TEShader>(location); 
        }break;
        case T_::SOUND :    
        {
            auto it = m_sounds.find(resourceID);
            if(it != m_sounds.end()) return;
            m_sounds[resourceID] = std::make_shared<TESound>(location); 
        }break;
        case T_::MUSIC :    
        {
            auto it = m_musics.find(resourceID);
            if(it != m_musics.end()) return;
            m_musics[resourceID] = std::make_shared<TEMusic>(location); 
        }break;
        default : break;
    }
}

void TEResourceManager::GetTexture  (std::string resourceID, std::shared_ptr<TETexture>& res)
{
    res = m_textures[resourceID];
    if(!res->Loaded()) 
    {
        res->Load();
        m_loadedTextures.push_back(resourceID);
    }
}

void TEResourceManager::GetShader  (std::string resourceID, std::shared_ptr<TEShader>& res)
{
    res = m_shaders[resourceID];
    if(!res->Loaded()) res->Load();
}

void TEResourceManager::GetSound   (std::string resourceID, std::shared_ptr<TESound>& res)
{
    res = m_sounds[resourceID];
    if(!res->Loaded()) res->Load();
}

void TEResourceManager::GetMusic   (std::string resourceID, std::shared_ptr<TEMusic>& res)
{
    res = m_musics[resourceID];
    if(!res->Loaded()) res->Load();
}

void TEResourceManager::GC()
{
    __GC(m_loadedTextures,  m_textures)
    __GC(m_loadedShaders,   m_shaders)
    __GC(m_loadedSounds,    m_sounds)
    __GC(m_loadedMusics,    m_musics)
}