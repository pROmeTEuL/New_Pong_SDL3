#include "player.h"

Player::Player(ID id, int width, int height)
    : m_id(id)
{
    m_speed = width / 68;
    m_player.w = width / 5;
    m_player.h = height / 48;
    m_player.y = height - m_player.h;
    if (m_id == PLAYER1) {
        m_key_left = SDLK_LEFT;
        m_key_right = SDLK_RIGHT;
        m_color_r = 255;
        m_color_g = 68;
        m_color_b = 84;
        m_player.x = 0;
    } else {
        m_key_left = SDLK_a;
        m_key_right = SDLK_d;
        m_color_r = 0;
        m_color_g = 193;
        m_color_b = 226;
        m_player.x = width - m_player.w;
    }
}

void Player::processEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.keysym.sym == m_key_left)
            m_left = true;
        if (event.key.keysym.sym == m_key_right)
            m_right = true;
    }
    if (event.type == SDL_EVENT_KEY_UP) {
        if (event.key.keysym.sym == m_key_left)
            m_left = false;
        if (event.key.keysym.sym == m_key_right)
            m_right = false;
    }
}

void Player::update(float dt)
{
    m_player.x += m_right * dt * m_speed - m_left * dt * m_speed;
}

void Player::draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, m_color_r, m_color_g, m_color_b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &m_player);
}

bool Player::collides(SDL_FRect rect) const
{

}
