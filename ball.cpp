#include "ball.h"

Ball::Ball(int width, int height, ID owner)
{
    m_ball.x = 20;
    m_ball.y = 30;
    m_ball.w = width / 64;
    m_ball.h = height  /48;
    m_speed = width / 90;
    m_owner = owner;
}

void Ball::update(Player &player, int width, int height)
{
    if (player.getID() != m_owner)
        return;
    if (m_state == HOLDED) {
        m_ball.y = player.getPosition().y - m_ball.h;
        m_ball.x = player.getPosition().x + player.getPosition().w / 2 - m_ball.w / 2;
        return;
    }
    m_ball.x += m_speed * m_direction_horizontal;
    m_ball.y += m_speed * m_direction_vertical;
    if (m_ball.x <= 0 || m_ball.x + m_ball.w >= width)
        m_direction_horizontal = -m_direction_horizontal;
    if (m_ball.y <= 0)
        m_direction_vertical = -m_direction_vertical;
    if (collides_with(player)) {
        player.addScore();
        m_direction_vertical = -m_direction_vertical;
        if (player.isMovingLeft())
            effectLeft();
        if (player.isMovingRight())
            effectRight();
        if (m_owner == PLAYER1)
            m_owner = PLAYER2;
        else
            m_owner = PLAYER1;
    }
    if (m_ball.y + m_ball.h >= height)
        m_state = HOLDED;
}

void Ball::draw(SDL_Renderer *renderer)
{
    if (m_owner == PLAYER1)
        SDL_SetRenderDrawColor(renderer, 255, 68, 84, SDL_ALPHA_OPAQUE);
    else if (m_owner == PLAYER2)
        SDL_SetRenderDrawColor(renderer, 0, 193, 226, SDL_ALPHA_OPAQUE);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &m_ball);
}

void Ball::launchBall(int direction)
{
    m_state = FLYING;
    m_direction_vertical = -1;
    m_direction_horizontal = direction;
    if (m_owner == PLAYER1)
        m_owner = PLAYER2;
    else
        m_owner = PLAYER1;
}

void Ball::effectLeft()
{
    --m_direction_horizontal;
}

void Ball::effectRight()
{
    ++m_direction_horizontal;
}

ID Ball::getOwner() const
{
    return m_owner;
}

State Ball::getState() const
{
    return m_state;
}

bool Ball::collides_with(const Player &player)
{
    if (player.getID() != m_owner)
        return false;
    if (player.getPosition().x > m_ball.x + m_ball.w)
        return false;
    if (player.getPosition().x + player.getPosition().w < m_ball.x)
        return false;
    if (player.getPosition().y > m_ball.y + m_ball.h)
        return false;
    if (player.getPosition().y + player.getPosition().h < m_ball.y)
        return false;
    return true;
}
