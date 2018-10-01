#ifndef TERO_SERIALIZABLE_HPP
#define TERO_SERIALIZABLE_HPP

#include <json/json.h>

class TESerializable
{
    public:
    virtual void Load(Json::Value&& data) = 0;
    virtual Json::Value Serialize() const { return Json::Value(Json::objectValue); };
    virtual void Initialize() {};
};

#endif