#include "CoreObject/TESystem.hpp"

std::unordered_map<TESystemType, std::function<std::weak_ptr<TESystem>(TESystemID)>> TESystem::registeredSystems;
std::unordered_map<TESystemType, std::function<void(std::weak_ptr<TESystem>)>> TESystem::systemsDestroyer;