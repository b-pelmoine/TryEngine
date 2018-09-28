#ifndef TERO_COMP_TEST_HPP
#define TERO_COMP_TEST_HPP

#include "CoreMinimal.hpp"
#include <iostream>

struct CTest : public TEComponent {
    CTest(std::weak_ptr<TEEntity> e): TEComponent(e) {}
    CTest() {}
    static const TEComponentType TypeID;
    virtual TEComponentType Type() const noexcept override { return CTest::TypeID; }
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
};

#endif