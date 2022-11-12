#include <string>

#include "Scene.hpp"

class mapLoader
{
public:
    mapLoader();
    ~mapLoader();
    void load();
    void setCurrentMap(std::string name);
private:
    Scene* scene;


};