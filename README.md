# New_Pong_SDL3

New_Pong_SDL3 is a remake of the New_Pong, but in SDL3 and with another feature.
The install is really simple:
  - First, clone the git repository using:
`git clone https://github.com/pROmeTEuL/New_Pong_SDL3.git`
  - Then, create a build folder and run cmake from that folder onto the project folder
```
mkdir ../build
cd ../build
cmake ../New_Pong
```
  - After that run the executable file and enjoy the game
### Note: If you are using windows, be sure to have MinGW (or an equivalent) installed, otherwise you cannot use the commands previously specified
Game features:
  - Multiplayer (just like squash game, but it's pong)
  - A menu to choose between multiplayer and singleplayer, made using ImGui
  - An option to play this game online using a tcp connection
  - A chat to communicate with the opponent 
## Note: The address to join an online game must be an IPv4 address instead of IPv6 or domain name (example: 127.0.0.1 instead of localhost or 0:0:0:0:0:0:0:1)

