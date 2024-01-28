#ifndef HOST_H
#define HOST_H

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>
#include <functional>
#include <memory>

#include "gamebase.h"
#include "window.h"
#include "player.h"
#include "ball.h"

class Host : private GameBase
{
public:
    Host(const std::shared_ptr<Window> &window);
    ~Host();
    void connection();
private:
//    std::function<void()> m_waiting_for_connection;

    std::unique_ptr<Player>m_player1; // host
    std::unique_ptr<Player> m_player2; // guest
    std::unique_ptr<Ball> m_ball;
    SDLNet_Server *m_server = nullptr;
    SDLNet_StreamSocket *m_socket = nullptr;
    // GameBase interface
protected:
    void processObjectEvent(SDL_Event &event);
    void update(int width, int height);
    void drawObjects(SDL_Renderer *renderer);
};

#endif // HOST_H
