#include "System/TEResourceManager.hpp"
#include <iostream>
TEResourceManager::TEResourceManager()
{
    for(size_t i=0; i < static_cast<size_t>(TEResource::Type::_Count); ++i)
    {
        m_resources.push_back(
            std::map<std::string, std::unique_ptr<TEResource>>()
        );
    }
}

TEResource* TEResourceManager::Register(std::string resourceID, std::string location, TEResource::Type type)
{
    using T_ = TEResource::Type;
    const size_t typeIndex = static_cast<size_t>(type);
    {
        //check if already registered
        auto it = m_resources[typeIndex].find(resourceID);
        if(it != m_resources[typeIndex].end()) return it->second.get();
    }
    switch(type)
    {
        case T_::TEXTURE :  m_resources[typeIndex][resourceID] = std::make_unique<TETexture>(location); break;
        case T_::SOUND :    m_resources[typeIndex][resourceID] = std::make_unique<TESound>(location); break;
        case T_::MUSIC :    m_resources[typeIndex][resourceID] = std::make_unique<TEMusic>(location); break;
        default : break;
    }
    return m_resources[typeIndex][resourceID].get();
}
const TETexture& TEResourceManager::GetTexture(std::string resourceID)
{
    TETexture& tex = dynamic_cast<TETexture&> (*m_resources[static_cast<size_t>(TEResource::Type::TEXTURE)][resourceID]);
    if(!tex.Loaded()) tex.Load();
    return tex;
}
const TESound& TEResourceManager::GetSound(std::string resourceID)
{
    TESound& snd = dynamic_cast<TESound&> (*m_resources[static_cast<size_t>(TEResource::Type::SOUND)][resourceID]);
    if(!snd.Loaded()) snd.Load();
    return snd;
}
const TEMusic& TEResourceManager::GetMusic(std::string resourceID)
{
    TEMusic& music = dynamic_cast<TEMusic&> (*m_resources[static_cast<size_t>(TEResource::Type::MUSIC)][resourceID]);
    if(!music.Loaded()) music.Load();
    return music;
}