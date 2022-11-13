#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

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
    void updateFiles();
    std::vector<std::string>* getFiles();
private:
    Scene* scene;
    std::vector<std::string> files;
    std::vector<int> mapEnteties;
    std::string currentMap;

};