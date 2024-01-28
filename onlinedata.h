#ifndef ONLINEDATA_H
#define ONLINEDATA_H

#include <SDL3/SDL.h>
#include <cstdint>

#include "ball.h"
#include "player.h"
#include "menu.h"

#pragma pack(push, 1)

struct Header
{
    enum class Type: uint8_t {
        HOST_DATA,
        GUEST_DATA,
        MSG_DATA
    };
    Header(Type type, uint8_t size)
        : type(type)
        , size(size)
    {}
    virtual ~Header() = default;
    uint32_t SIG = *reinterpret_cast<const uint32_t*>("RTV");
    Type type;
    uint8_t size;
};

struct HostData : public Header {
    HostData()
        : Header(Type::HOST_DATA, sizeof(HostData))
    {}
    Ball::State ballState;
    Player::ID ballOwner;
    Menu::State menuState;
    SDL_FRect ball;
    SDL_FRect player1;
    int score1, score2;
};

struct GuestData : public Header
{
    GuestData()
        : Header(Type::GUEST_DATA, sizeof(GuestData))
    {}
    Ball::State ballState;
    int playerDirection;
    Menu::State menuState;
    SDL_FRect player2;

};

struct MsgData : public Header
{
    MsgData()
        : Header(Type::MSG_DATA, sizeof(MsgData))
    {}
    char msg[255 - sizeof(Header)];
};

#pragma pack(pop)

#endif // ONLINEDATA_H
