#include "mapLoader.hpp"


mapLoader::mapLoader(Scene* scene)
    :scene(scene)
{
    
}

mapLoader::~mapLoader() {
    
}

void mapLoader::load() {
    std::ifstream cur;
    cur.open("../Levels/Current.txt");
    if(cur.is_open())
    {
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
            MeshComponent meshComp("UBot-OBJ.obj");
		    scene->SetComponent<MeshComponent>(this->mapEnteties.back(), meshComp);
        }      
    }
}

void mapLoader::unLoad() {
    for(auto e: this->mapEnteties)
    {
        this->scene->RemoveEntity(e);
    }
}

void mapLoader::setCurrentMap(std::string name) {
    
}
