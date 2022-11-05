
#include "raylib.h"

#include "Scene.hpp"
#include "Systems.hpp"

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

private:
    void checkMenuSwitch();
    void setSystems();
    void drawMenu();
    void drawGame();
    void drawEditor();
};