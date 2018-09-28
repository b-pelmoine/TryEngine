#include "Modules/DiscordHandler.hpp"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <iostream>

std::string DiscordHandler::ID() { return "DISCORD-RPC"; }

DiscordHandler::DiscordHandler() {}

DiscordHandler::~DiscordHandler()
{
    Discord_Shutdown();
}

void DiscordHandler::Load(Json::Value&& data)
{
    APP_ID = const_cast<char*>(data["DISCORD_APP_ID"].asCString());
}

void DiscordHandler::Initialize()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = DiscordHandlers::handleDiscordReady;
    handlers.errored = DiscordHandlers::handleDiscordError;
    handlers.disconnected = DiscordHandlers::handleDiscordDisconnected;
    handlers.joinGame = DiscordHandlers::handleDiscordJoinGame;
    handlers.spectateGame = DiscordHandlers::handleDiscordSpectateGame;
    handlers.joinRequest = DiscordHandlers::handleDiscordJoinRequest;

    // Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
    Discord_Initialize(APP_ID, &handlers, 1, NULL);
    std::cout << "DISCORD_INITIALISATION" << std::endl;
    UpdatePresence();
}

void DiscordHandler::Update()
{
    Discord_RunCallbacks();
}
   
void DiscordHandler::UpdatePresence()
{
    char buffer[256];
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.startTimestamp = time(0);
    discordPresence.state = "Coding solo";
    sprintf(buffer, "DiscordRPC API <3");
    discordPresence.details = buffer;
    discordPresence.largeImageText = "Hello watchudoin ?";
    discordPresence.largeImageKey = "tero_large";
    discordPresence.smallImageKey = "tero";
    Discord_UpdatePresence(&discordPresence);

    std::cout << "DISCORD_UPDATE" << std::endl;
}

#pragma region DiscordHandlers

static int prompt(char* line, size_t size)
{
    int res;
    char* nl;
    printf("\n> ");
    fflush(stdout);
    res = fgets(line, (int)size, stdin) ? 1 : 0;
    line[size - 1] = 0;
    nl = strchr(line, '\n');
    if (nl) {
        *nl = 0;
    }
    return res;
}

static void DiscordHandlers::handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("\nDiscord: connected to user %s#%s - %s\n",
           connectedUser->username,
           connectedUser->discriminator,
           connectedUser->userId);
}

static void DiscordHandlers::handleDiscordDisconnected(int errcode, const char* message)
{
    printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void DiscordHandlers::handleDiscordError(int errcode, const char* message)
{
    printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void DiscordHandlers::handleDiscordJoinGame(const char* secret)
{
    printf("\nDiscord: join (%s)\n", secret);
}

static void DiscordHandlers::handleDiscordSpectateGame(const char* secret)
{
    printf("\nDiscord: spectate (%s)\n", secret);
}

static void DiscordHandlers::handleDiscordJoinRequest(const DiscordUser* request)
{
    int response = -1;
    char yn[4];
    printf("\nDiscord: join request from %s#%s - %s\n",
           request->username,
           request->discriminator,
           request->userId);
    do {
        printf("Accept? (y/n)");
        if (!prompt(yn, sizeof(yn))) {
            break;
        }

        if (!yn[0]) {
            continue;
        }

        if (yn[0] == 'y') {
            response = DISCORD_REPLY_YES;
            break;
        }

        if (yn[0] == 'n') {
            response = DISCORD_REPLY_NO;
            break;
        }
    } while (1);
    if (response != -1) {
        Discord_Respond(request->userId, response);
    }
}

#pragma endregion DiscordHandlers
