#include "player.h"
#include "ball.h"

Player::Player(ID id, int width, int height)
    : m_id(id)
{
    m_speed = width / 400;
    m_player.w = width / 5;
    m_player.h = height / 48;
    m_player.y = height - m_player.h;
    if (m_id == PLAYER1) {
        m_key_left = SDLK_a;
        m_key_right = SDLK_d;
        m_key_launch = SDLK_SPACE;
        m_color_r = 255;
        m_color_g = 68;
        m_color_b = 84;
        m_player.x = 0;
    } else {
        m_key_left = SDLK_LEFT;
        m_key_right = SDLK_RIGHT;
        m_key_launch = SDLK_RSHIFT;
        m_color_r = 0;
        m_color_g = 193;
        m_color_b = 226;
        m_player.x = width - m_player.w;
    }
    m_start = std::chrono::high_resolution_clock::now();
}

void Player::processEvent(const SDL_Event &event, Ball& ball)
{
    bool ret = false;
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.keysym.sym == m_key_left)
            m_left = true;
        if (event.key.keysym.sym == m_key_right)
            m_right = true;
        if (event.key.keysym.sym == m_key_launch && ball.getState() == HOLDED && ball.getOwner() == m_id)
            ball.launchBall(m_right - m_left);
    }
    if (event.type == SDL_EVENT_KEY_UP) {
        if (event.key.keysym.sym == m_key_left)
            m_left = false;
        if (event.key.keysym.sym == m_key_right)
            m_right = false;
    }
}

void Player::update(int width)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto dif_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start).count();
    if (dif_ms < 5)
        return;
    m_start = now;
    m_player.x += m_right * dif_ms * m_speed - m_left * dif_ms * m_speed;
    if (m_player.x < 0)
        m_player.x = 0;
    if (m_player.x + m_player.w > width)
        m_player.x = width - m_player.w;
}

void Player::draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, m_color_r, m_color_g, m_color_b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &m_player);
}

SDL_FRect Player::getPosition() const
{
    return m_player;
}

bool Player::isMovingLeft() const
{
    return m_left;
}

bool Player::isMovingRight() const
{
    return m_right;
}

ID Player::getID() const
{
    return m_id;
}

int Player::getScore() const
{
    return score;
}

int Player::addScore()
{
    ++score;
}
