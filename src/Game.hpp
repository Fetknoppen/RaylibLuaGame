
#include "raylib.h"

#include "Scene.hpp"
#include "Systems.hpp"
#include "Button.hpp"

enum GAME_STATE{MENU, GAME, EDITOR};

class Game
{
public:
    Game(lua_State* L);
    ~Game();
    void run();
private:
    Scene* scene;
    lua_State* L;
    GAME_STATE gameState;
    std::vector<Button> buttons;

private:
    void checkMenuSwitch();
    void setSystems();
    void startMenu();
    void drawMenu();
    void startGame();
    void drawGame();
    void startEditor();
    void drawEditor();
};