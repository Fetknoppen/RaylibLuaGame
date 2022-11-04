#include <iostream>
#include <unordered_map>
#include <string>

#include "raylib.h"

class ResourceHandler
{
public:
    ResourceHandler();
    ~ResourceHandler();

    void loadModel(std::string modelName);
    Model* getModel(std::string modelName);
private:
    std::unordered_map<std::string, Model> models;
    bool modelLoaded(std::string modelName);
};