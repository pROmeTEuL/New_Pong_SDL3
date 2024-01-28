#include "multiplayer.h"

#include <cassert>

#include "menu.h"
#include "onlinedata.h"

Multiplayer::Multiplayer(const std::shared_ptr<Window> &window, Mode mode, SDLNet_StreamSocket *socket)
    : GameBase(window)
    , m_mode(mode)
    , m_socket(socket)
{
    m_player1 = std::make_unique<Player>(Player::ID::PLAYER1, window->getWidth(), window->getHeight());
    m_player2 = std::make_unique<Player>(Player::ID::PLAYER2, window->getWidth(), window->getHeight());
    m_ball = std::make_unique<Ball>(window->getWidth(), window->getHeight(), m_player1->getID());
    Menu::instance().setDrawScore([this]{
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Score", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 68, 84, 255));
        ImGui::Text("Player 1 score: %d", m_player1->getScore());
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 193, 226, 255));
        ImGui::Text("Player 2 score: %d", m_player2->getScore());
        ImGui::PopStyleColor();
        ImGui::End();
    });
    switch(mode) {
    case Mode::HOST:
        m_data = std::make_unique<HostData>();
        break;
    case Mode::GUEST:
        m_data = std::make_unique<GuestData>();
        break;
    default:
        break;
    }
}

Multiplayer::~Multiplayer()
{
    SDLNet_DestroyStreamSocket(m_socket);
}

void Multiplayer::processObjectEvent(SDL_Event &event)
{
    switch(m_mode) {
    case Mode::LOCAL:
        m_player1->processEvent(event, *m_ball);
        m_player2->processEvent(event, *m_ball);
        break;
    case Mode::HOST:
        m_player1->processEvent(event, *m_ball);
        // player2 is handled by guest
        break;
    case Mode::GUEST:
        // player1 is handled by host
        m_player2->processEvent(event, *m_ball);
        break;
    }
}

void Multiplayer::update(int width, int height)
{
    switch(m_mode) {
    case Mode::LOCAL:
        m_player1->update(width);
        m_player2->update(width);
        m_ball->update(*m_player1, width, height);
        m_ball->update(*m_player2, width, height);
        break;
    case Mode::HOST:
        m_player1->update(width);
        // player2 is handled by guest
        m_ball->update(*m_player1, width, height);
        m_ball->update(*m_player2, width, height);
        break;
    case Mode::GUEST:
        m_player2->update(width);
        // player1 and ball are handled by host
        break;
    }
}

void Multiplayer::drawObjects(SDL_Renderer *renderer)
{
    if(m_ball->getOwner() == Player::ID::PLAYER1) {
        m_player2->draw(renderer);
        m_player1->draw(renderer);
    } else {
        m_player1->draw(renderer);
        m_player2->draw(renderer);
    }
    m_ball->draw(renderer);
}

bool operator!=(SDL_FRect rect1, SDL_FRect rect2) {
    return rect1.x != rect2.x ||
           rect1.y != rect2.y ||
           rect1.w != rect2.w ||
           rect1.h != rect2.h;
}

void Multiplayer::processEvent()
{
    static_assert(sizeof(m_recvBuffer) == 256, "Invalid size");
    if (m_socket) {
        int size = SDLNet_ReadFromStreamSocket(m_socket, &m_recvBuffer[m_bufferPos], sizeof(m_recvBuffer) - m_bufferPos);
        if (sizeof(m_recvBuffer) - m_bufferPos > 256) {
            std::cerr << "m_bufferPos = " << m_bufferPos << std::endl;
            assert(false);
        }
        if (size < 0) {
            m_playing = false;
            Menu::instance().setState(Menu::State::MAIN_MENU);
            return;
        }
        while (size > 0 || m_bufferPos > sizeof(Header)) {
            m_bufferPos += size;
            size = 0;
            if (m_bufferPos < sizeof(Header))
                break;
            auto buf = reinterpret_cast<Header*>(m_recvBuffer);
            assert(buf->SIG == *reinterpret_cast<const uint32_t*>("RTV"));
            if (buf->size > m_bufferPos)
                break;
            switch(buf->type) {
            case Header::Type::GUEST_DATA: {
                // I am host and received data from guest
                auto guestBuf = reinterpret_cast<GuestData*>(buf);
                m_player2->setPos(guestBuf->player2);
                if (m_ball->getOwner() == Player::ID::PLAYER2 && m_ball->getState() == Ball::State::HOLDED && guestBuf->ballState == Ball::State::FLYING)
                    m_ball->launchBall(guestBuf->playerDirection);
                m_player2->setDirection(guestBuf->playerDirection);
                Menu::instance().setState(guestBuf->menuState);
            }
            break;
            case Header::Type::HOST_DATA: {
                // I am guest and received data from host
                auto hostBuf = reinterpret_cast<HostData*>(buf);
                m_player1->setPos(hostBuf->player1);
                m_ball->setPos(hostBuf->ball);
                m_ball->setState(hostBuf->ballState);
                m_ball->setOwner(hostBuf->ballOwner);
                m_player1->setScore(hostBuf->score1);
                m_player2->setScore(hostBuf->score2);

                Menu::instance().setState(hostBuf->menuState);
            }
            break;
            case Header::Type::MSG_DATA: {
                auto msgBuf = reinterpret_cast<MsgData*>(buf);
            }
            break;
            }
            memmove(&m_recvBuffer[0], &m_recvBuffer[buf->size], m_bufferPos - buf->size);
            m_bufferPos -= buf->size;
        }
    }
    GameBase::processEvent();
    bool sendData = false;
    switch(m_mode) {
    case Mode::HOST: {
        auto hostData = static_cast<HostData*>(m_data.get());
        if (hostData->ball != m_ball->getPos()) {
            sendData = true;
            hostData->ball = m_ball->getPos();
        }
        if (hostData->player1 != m_player1->getPos()) {
            sendData = true;
            hostData->player1 = m_player1->getPos();
        }
        if (hostData->score1 != m_player1->getScore()) {
            sendData = true;
            hostData->score1 = m_player1->getScore();
        }
        if (hostData->score2 != m_player2->getScore()) {
            sendData = true;
            hostData->score2 = m_player2->getScore();
        }
        if(hostData->ballState != m_ball->getState()) {
            sendData = true;
            hostData->ballState = m_ball->getState();
        }
        if(hostData->menuState != Menu::instance().getState()) {
            sendData = true;
            hostData->menuState = Menu::instance().getState();
        }
        if(hostData->ballOwner != m_ball->getOwner()) {
            sendData = true;
            hostData->ballOwner = m_ball->getOwner();
        }
    }
    break;
    case Mode::GUEST: {
        auto guestData = static_cast<GuestData*>(m_data.get());
        if (guestData->player2 != m_player2->getPos()) {
            sendData = true;
            guestData->player2 = m_player2->getPos();
        }
        if(guestData->ballState != m_ball->getState()) {
            sendData = true;
            guestData->ballState = m_ball->getState();
        }
        int dir = m_player2->isMovingRight() - m_player2->isMovingLeft();
        if (guestData->playerDirection != dir) {
            sendData = true;
            guestData->playerDirection = dir;
        }
        if(guestData->menuState != Menu::instance().getState()) {
            sendData = true;
            guestData->menuState = Menu::instance().getState();
        }
    }
    break;
    default:
        break;
    }
    if (sendData) {
        SDLNet_WriteToStreamSocket(m_socket, m_data.get(), m_data->size);
    }
}
