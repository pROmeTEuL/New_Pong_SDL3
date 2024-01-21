#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H

#include "gamebase.h"
#include "player.h"
#include "ball.h"

class Singleplayer : public GameBase
{
public:
    Singleplayer(Window &window);
    ~Singleplayer();
private:
        Player *m_player;
        Ball *m_ball;
// GameBase interface
protected:
    void processObjectEvent(SDL_Event &event);
    void update(int width, int height);
    void drawObjects(SDL_Renderer *renderer);
    void drawScore();
};

#endif // SINGLEPLAYER_H
