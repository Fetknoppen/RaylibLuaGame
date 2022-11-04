
#include "raylib.h"

#include "Scene.hpp"
#include "Systems.hpp"

class Game
{
public:
    Game(lua_State* L);
    ~Game();
    void run();
private:
    Scene* scene;
    lua_State* L;

private:
    void setSystems();
};