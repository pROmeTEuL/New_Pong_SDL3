#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H

#include "gamebase.h"
#include "player.h"
#include "ball.h"
#include <memory>

class Singleplayer : public GameBase
{
public:
    Singleplayer(const std::shared_ptr<Window> &window);
    ~Singleplayer();
private:
        std::unique_ptr<Player> m_player;
        std::unique_ptr<Ball> m_ball;
// GameBase interface
protected:
    void processObjectEvent(SDL_Event &event);
    void update(int width, int height);
    void drawObjects(SDL_Renderer *renderer);
    void drawScore();
};

#endif // SINGLEPLAYER_H
