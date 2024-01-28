#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include <memory>
#include <SDL3_net/SDL_net.h>

#include "gamebase.h"
#include "player.h"
#include "ball.h"

class Multiplayer : public GameBase
{
public:
    enum class Mode {
        LOCAL,
        HOST,
        GUEST
    };

public:
    Multiplayer(const std::shared_ptr<Window> &window, Mode mode, SDLNet_StreamSocket *socket = nullptr);
    ~Multiplayer();
private:
    std::unique_ptr<Player> m_player1;
    std::unique_ptr<Player> m_player2;
    std::unique_ptr<Ball> m_ball;
    std::unique_ptr<struct Header> m_data;
    SDLNet_StreamSocket *m_socket;
    char m_recvBuffer[256];
    int m_bufferPos = 0;
    Mode m_mode;
    // GameBase interface
protected:
    void processObjectEvent(SDL_Event &event) override;
    void update(int width, int height) override;
    void drawObjects(SDL_Renderer *renderer) override;
    void processEvent() override;
};

#endif // MULTIPLAYER_H
