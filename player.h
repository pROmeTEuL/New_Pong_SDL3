#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <chrono>

enum ID {
    PLAYER1,
    PLAYER2
};

class Ball;

class Player
{
public:
    Player(ID id, int width, int height);
    void processEvent(const SDL_Event &event, Ball &ball);
    void update(int width);
    void draw(SDL_Renderer *renderer);
    SDL_FRect getPosition() const;
    bool collides(SDL_FRect rect) const;
    ID getID() const;
private:
    ID m_id;
    SDL_FRect m_player;
    int m_key_left;
    int m_key_right;
    int m_key_launch;
    int m_color_r,
        m_color_g,
        m_color_b;
    int m_speed;
    bool m_left = false;
    bool m_right = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

#endif // PLAYER_H
