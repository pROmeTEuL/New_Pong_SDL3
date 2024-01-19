#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>

enum ID {
    PLAYER1,
    PLAYER2
};

class Player
{
public:
    Player(ID id, int width, int height);
    void processEvent(const SDL_Event &event);
    void update(float dt);
    void draw(SDL_Renderer *renderer);
    bool collides(SDL_FRect rect) const;
private:
    ID m_id;
    SDL_FRect m_player;
    int m_key_left;
    int m_key_right;
    int m_color_r,
        m_color_g,
        m_color_b;
    int m_speed;
    bool m_left = false;
    bool m_right = false;
};

#endif // PLAYER_H
