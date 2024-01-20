#ifndef BALL_H
#define BALL_H

#include <SDL3/SDL.h>

enum State {
    FLYING,
    HOLDED
};

class Ball
{
public:
    Ball();
private:
    State m_state = HOLDED;
    SDL_FRect m_ball;
    m_speed;
    enum Direction {
        LEFT,
        RIGHT
    } m_direction;
    bool m_up = false;
};

#endif // BALL_H
