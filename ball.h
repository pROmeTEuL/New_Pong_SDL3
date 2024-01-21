#ifndef BALL_H
#define BALL_H

#include <SDL3/SDL.h>
#include "player.h"

enum State {
    FLYING,
    HOLDED
};

class Ball
{
public:
    Ball(int width, int height, ID owner);
    void update(Player &player, int width, int height);
    void draw(SDL_Renderer *renderer);
    void launchBall(int direction);
    ID getOwner() const;
    State getState() const;
private:
    bool collides_with(const Player &player);
    void effectLeft();
    void effectRight();
    State m_state = HOLDED;
    SDL_FRect m_ball;
    int m_effect = 1; // NEVER 0
    int m_speed;
    int m_direction_horizontal = 1;
    int m_direction_vertical = 0;
    ID m_owner;
    bool m_up = false;
};

#endif // BALL_H
