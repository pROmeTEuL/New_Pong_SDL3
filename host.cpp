#include "host.h"

Host::Host(const std::shared_ptr<Window> &window) : GameBase(window)
{
    m_player1 = std::make_unique<Player>(PLAYER1, window->getWidth(), window->getHeight());
    m_player2 = std::make_unique<Player>(PLAYER2, window->getWidth(), window->getHeight());
    m_ball = std::make_unique<Ball>(window->getHeight(), window->getHeight(), PLAYER1);
}

Host::~Host()
{
    SDLNet_DestroyServer(m_server);
}

void Host::connection()
{
    while () {

    }
    run();
}

void Host::processObjectEvent(SDL_Event &event)
{
    SDLNet_AcceptClient(m_server, &m_socket);
    m_player1->processEvent(event, *m_ball);
}

void Host::update(int width, int height)
{
    m_player1->update(width);
    m_ball->update(*m_player1, width, height);
}

void Host::drawObjects(SDL_Renderer *renderer)
{
    if (m_ball->getOwner() == PLAYER1) {
        // guest
        m_player1->draw(renderer); // host
    } else {
        m_player1->draw(renderer); // host
        // guest
    }
    m_ball->draw(renderer);
}
