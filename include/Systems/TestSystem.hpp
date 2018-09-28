#ifndef TERO_SYS_TEST_HPP
#define TERO_SYS_TEST_HPP

#include "CoreMinimal.hpp"
#include <iostream>

struct STest : public TESystem {
    STest(Json::LargestInt ID): TESystem(ID) {}
    static const TESystemType TypeID;
    virtual TESystemType Type() const noexcept override { return STest::TypeID; }
    virtual Json::Value Serialize() const
    {
        return Json::Value(Json::objectValue);
    }
    virtual void Load(Json::Value&& data)
    {
        std::cout << "Loading... " << TypeID << "\n";
    }
    virtual void Initialize()
    {
        std::cout << "Loaded: " << TypeID << "\n";
    }

    private: 
    STest(): TESystem(0) {}
    friend class UserDefinedTypes;
};

#endif