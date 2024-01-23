#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "gamebase.h"
#include "player.h"
#include "ball.h"
#include <memory>

class Multiplayer : public GameBase
{
public:
    Multiplayer(const std::shared_ptr<Window> &window);
    ~Multiplayer();
private:
    Player *m_player1;
    Player *m_player2;
    Ball *m_ball;
    // GameBase interface
protected:
    void processObjectEvent(SDL_Event &event) override;
    void update(int width, int height) override;
    void drawObjects(SDL_Renderer *renderer) override;
};

#endif // MULTIPLAYER_H
