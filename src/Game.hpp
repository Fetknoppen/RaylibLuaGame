
#include "raylib.h"

#include "Scene.hpp"
#include "Systems.hpp"
#include "Button.hpp"
#include "mapLoader.hpp"
#include "Editor.hpp"

enum GAME_STATE{MENU, GAME, EDITOR, QUIT};

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
    mapLoader* map;
    Editor* editor;//This is causing stack mashing? and seg fault?

private:
    bool mapSave;
    std::string mapSaveName;

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