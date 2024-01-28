#ifndef BALL_H
#define BALL_H

#include <SDL3/SDL.h>
#include <cstdint>

#include "player.h"

class Ball
{
public:
    enum class State: uint8_t {
        FLYING,
        HOLDED
    };
public:
    Ball(int width, int height, Player::ID owner);
    void update(Player &player, int width, int height);
    void draw(SDL_Renderer *renderer);
    void launchBall(int direction);
    int getDirection() const;
    Player::ID getOwner() const;
    State getState() const;
    void setPos(SDL_FRect rect);
    SDL_FRect getPos() const;
    void setState(State newState);

    void setOwner(Player::ID newOwner);

private:
    bool collides_with(const Player &player);
    void effectLeft();
    void effectRight();
private:
    State m_state = State::HOLDED;
    SDL_FRect m_ball;
    int m_effect = 1; // NEVER 0
    int m_speed;
    int m_direction_horizontal = 1;
    int m_direction_vertical = 0;
    Player::ID m_owner;
    bool m_up = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

#endif // BALL_H
