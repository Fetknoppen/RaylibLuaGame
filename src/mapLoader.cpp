#include "mapLoader.hpp"
#include "raylib.h"


mapLoader::mapLoader(Scene* scene)
    :scene(scene)
{
    this->currentMap = "default";
}

mapLoader::~mapLoader() {
    //std::cout<<"Map deconstructor start\n";
    //std::cout<<"Map deconstructor end\n";
}

void mapLoader::load(lua_State* L) {
    std::ifstream cur;
    std::string path = "../Levels/"+this->currentMap;
    cur.open(path);
    //std::cout<<"Trying to load "<<this->currentMap<<std::endl;
    if(cur.is_open())
    {
        //std::cout<<"Loading "<<this->currentMap<<std::endl;
        std::string delimiter_space = " ";
        std::string delimiter_comma = ",";
        std::string token;
        std::string line;
        Vector3 position;
        int type;
        size_t pos = 0;
        while(getline(cur, line))
        {
            pos = line.find(delimiter_space);
            token = line.substr(0, pos);
            type = stoi(token);
            line.erase(0, pos+delimiter_space.size());
            pos = line.find(delimiter_comma);
            token = line.substr(0, pos);
            position.x = std::stof(token);
            line.erase(0, pos+delimiter_comma.size());
            pos = line.find(delimiter_comma);
            token = line.substr(0, pos);
            position.y = std::stof(token);
            line.erase(0, pos+delimiter_comma.size());
            token = position.z = std::stof(line);
            
            //std::cout<<"Type: "<<type<<" x: "<<position.x<<" y: "<<position.y<<" z: "<<position.z<<std::endl;
            this->mapEnteties.push_back(this->scene->CreateEntity());
            TransformComponent& trans = this->scene->GetComponent<TransformComponent>(this->mapEnteties.back());
            trans.position = position;
            MeshComponent meshComp;
            BoundingBox box;
            switch (type)
            {
            case 1:
                meshComp.name = "cube";
                box.min = {position.x - CELL_SIZE.x/2.0f, position.y - CELL_SIZE.y/2.0f, position.z - CELL_SIZE.x/2.0f};
                box.max = {position.x + CELL_SIZE.x/2.0f, position.y + CELL_SIZE.y/2.0f, position.z + CELL_SIZE.x/2.0f};
                break;
            case 2:
                scene->addBehaviour(L, this->mapEnteties.back(), "enemy.lua");
                meshComp.name = "basicCharacter.obj";
                box.min = {position.x - CELL_SIZE.x/4.0f, position.y - CELL_SIZE.y/4.0f, position.z - CELL_SIZE.x/4.0f};
                box.max = {position.x + CELL_SIZE.x/4.0f, position.y + CELL_SIZE.y/4.0f, position.z + CELL_SIZE.x/4.0f};
                break;
            }
		    scene->SetComponent<MeshComponent>(this->mapEnteties.back(), meshComp);

            
            CollisionComp col;
            col.box = box;
		    scene->SetComponent<CollisionComp>(this->mapEnteties.back(), col);
        }      
    }
    else
    {
        std::cout<<"Could not open "<<this->currentMap<<std::endl;
    }
}

void mapLoader::unLoad() {
    for(auto e: this->mapEnteties)
    {
        this->scene->RemoveEntity(e);
    }
}

void mapLoader::setCurrentMap(std::string name) {
    this->currentMap = name;
}

std::vector<std::string>* mapLoader::getFiles() {
    this->updateFiles();
    return &this->files;
}

void mapLoader::updateFiles() {
    this->files.clear();
    std::string path = "../Levels/";
    std::string temp;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        temp = entry.path().generic_string();
        temp.erase(0, path.length());
        this->files.push_back(temp);
    }    
}
