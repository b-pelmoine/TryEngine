#ifndef TERO_DISCORDHANDLER_HPP
#define TERO_DISCORDHANDLER_HPP

#include <System/TEModule.hpp>
#include <discord_register.h>
#include <discord_rpc.h>
#include <memory>

class DiscordHandler : public TEModule
{
    char* APP_ID;

    public:
    void Load(Json::Value&& data) override;
    void Initialize() override;
    void Update() override;
    void UpdatePresence();

    DiscordHandler();
    ~DiscordHandler();

    static std::string ID();
    static DiscordHandler* Create() { return new DiscordHandler; }
};

#endif