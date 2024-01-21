#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>

#include "window.h"
#include "player.h"
#include "ball.h"


class Singleplayer
{
public:
    Singleplayer(Window &window);
    ~Singleplayer();
    void processEvent(SDL_Event &event);
    void run();
private:
    void update();
    void draw();
    void getScore();
private:
    Window m_window;
    Player *m_player;
    Ball *m_ball;
    bool m_playing = true;
    bool m_paused = false;
};

#endif // SINGLEPLAYER_H
