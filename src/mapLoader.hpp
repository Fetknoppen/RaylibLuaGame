#include <string>
#include <fstream>
#include <iostream>

#include "Scene.hpp"
#include "raylib.h"

class mapLoader
{
public:
    mapLoader(Scene* scene);
    ~mapLoader();
    void load();
    void unLoad();
    void setCurrentMap(std::string name);
private:
    Scene* scene;
    std::vector<int> mapEnteties;

};