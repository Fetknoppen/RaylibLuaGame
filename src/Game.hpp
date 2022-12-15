
#include <filesystem>
#include <string>

#include "raylib.h"

#include "Scene.hpp"
#include "Systems.hpp"
#include "Button.hpp"
#include "mapLoader.hpp"
#include "Editor.hpp"

enum GAME_STATE{MENU, LEVEL_SELECTOR, GAME, EDITOR, QUIT};

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
    std::vector<Button*> mapButtons;
    Button* saveButton;
    mapLoader* map;
    Editor* editor;//This is causing stack mashing? and seg fault?

private:
    bool mapSave;
    std::string mapSaveName;
    bool quit;

private:
    void setSystems();
    void startMenu();
    void drawMenu();
    void startLevelSelector();
    void drawLevelSelector();
    void startGame();
    void drawGame();
    void startEditor();
    void drawEditor();
};