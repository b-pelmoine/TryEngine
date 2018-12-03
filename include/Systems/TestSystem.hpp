#ifndef TERO_SYS_TEST_HPP
#define TERO_SYS_TEST_HPP

#include "CoreMinimal.hpp"
#include <iostream>

class CTest;

struct STest : public TESystem {
    STest(Json::LargestInt ID): TESystem(ID) {}
    ~STest();
    static const TESystemType TypeID;
    virtual TESystemType Type() const noexcept override { return STest::TypeID; }
    Json::Value Serialize() const override;
    void Load(Json::Value&& data) override;
    void Initialize() override;
    void OnDestroy() override;

    private: 
    STest(): TESystem(0) {}
    std::vector<std::weak_ptr<CTest>> m_drawables;
    friend class UserDefinedTypes;
};

#endif