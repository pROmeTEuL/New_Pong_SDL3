#include "ball.h"

Ball::Ball(int width, int height, Player::ID owner)
{
    m_ball.x = 20;
    m_ball.y = 30;
    m_ball.w = width / 64;
    m_ball.h = height  /48;
    m_speed = width / 90;
    m_owner = owner;
    m_start = std::chrono::high_resolution_clock::now();
}

void Ball::update(Player &player, int width, int height)
{
    if (player.getID() != m_owner)
        return;
    auto now = std::chrono::high_resolution_clock::now();
    auto dif_ms = std::chrono::duration_cast<std::chrono::microseconds>(now - m_start).count();
    if (dif_ms < 16667)
        return;
    m_start = now;
    if (m_state == State::HOLDED) {
        m_ball.y = player.getPos().y - m_ball.h;
        m_ball.x = player.getPos().x + player.getPos().w / 2 - m_ball.w / 2;
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
        if (m_owner == Player::ID::PLAYER1)
            m_owner = Player::ID::PLAYER2;
        else if (m_owner == Player::ID::PLAYER2)
            m_owner = Player::ID::PLAYER1;
    }
    if (m_ball.y + m_ball.h >= height)
        m_state = State::HOLDED;
}

void Ball::draw(SDL_Renderer *renderer)
{
    if (m_owner == Player::ID::PLAYER1)
        SDL_SetRenderDrawColor(renderer, 255, 68, 84, SDL_ALPHA_OPAQUE);
    else if (m_owner == Player::ID::PLAYER2)
        SDL_SetRenderDrawColor(renderer, 0, 193, 226, SDL_ALPHA_OPAQUE);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &m_ball);
}

void Ball::launchBall(int direction)
{
    m_state = State::FLYING;
    m_direction_vertical = -1;
    m_direction_horizontal = direction;
    if (m_owner == Player::ID::PLAYER1)
        m_owner = Player::ID::PLAYER2;
    else if (m_owner == Player::ID::PLAYER2)
        m_owner = Player::ID::PLAYER1;
}

int Ball::getDirection() const
{
    return m_direction_horizontal;
}

void Ball::effectLeft()
{
    --m_direction_horizontal;
}

void Ball::effectRight()
{
    ++m_direction_horizontal;
}

void Ball::setOwner(Player::ID newOwner)
{
    m_owner = newOwner;
}

void Ball::setState(State newState)
{
    m_state = newState;
}

Player::ID Ball::getOwner() const
{
    return m_owner;
}

Ball::State Ball::getState() const
{
    return m_state;
}

void Ball::setPos(SDL_FRect rect)
{
    m_ball = rect;
}

SDL_FRect Ball::getPos() const
{
    return m_ball;
}

bool Ball::collides_with(const Player &player)
{
    if (player.getID() != m_owner)
        return false;
    if (player.getPos().x > m_ball.x + m_ball.w)
        return false;
    if (player.getPos().x + player.getPos().w < m_ball.x)
        return false;
    if (player.getPos().y > m_ball.y + m_ball.h)
        return false;
    if (player.getPos().y + player.getPos().h < m_ball.y)
        return false;
    return true;
}
