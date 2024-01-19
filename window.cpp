#include "window.h"

Window::Window(std::string_view title, int width, int height)
    : m_width(width)
    , m_height(height)
{
    if (SDL_CreateWindowAndRenderer(width, height, wflags, &m_window, &m_renderer) < 0) {
        SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
        close();
    }
    SDL_SetWindowTitle(m_window, title.data());
    const float scale = SDL_GetWindowDisplayScale(m_window);
    SDL_SetRenderScale(m_renderer, scale, scale);
}

Window::~Window()
{
//    SDL_CloseGamepad(joyId);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    close();
}

void Window::close()
{
    SDL_Quit();
}

void Window::clear(int r, int g, int b)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);
}

void Window::setColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void Window::draw(SDL_Texture *texture, SDL_FRect *fRect)
{
    SDL_RenderTexture(m_renderer, texture, NULL, fRect);
}

void Window::display()
{
    SDL_RenderPresent(m_renderer);
}

bool Window::pollEvent(SDL_Event &event)
{
    return SDL_PollEvent(&event);
}

SDL_Window *Window::getWindow() const
{
    return m_window;
}

SDL_Renderer *Window::getRenderer() const
{
    return m_renderer;
}

int Window::getWidth() const
{
    return m_width;
}

int Window::getHeight() const
{
    return m_height;
}
