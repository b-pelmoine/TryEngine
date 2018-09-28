#include "CoreObject/TESystem.hpp"

std::unordered_map<TESystemType, std::function<std::weak_ptr<TESystem>(TESystemID)>> TESystem::registeredSystems;