#include "ResourceHandler.hpp"

ResourceHandler::ResourceHandler(){};
ResourceHandler::~ResourceHandler()
{
    for(auto& p: this->models)
    {
        UnloadModel(p.second);
    }
};

void ResourceHandler::loadModel(std::string modelName)
{
    if(modelLoaded(modelName))
    {
        std::cout<<"ResourceHandler: model " << modelName << " already loaded"<<std::endl;
        return;
    }
    std::string path = "../assets/obj/";
    path += modelName;
    Model model = LoadModel(path.c_str());
    this->models.insert({modelName, model});

    if(this->models.count(modelName) > 0)
    {
        std::cout<<"ResourceHandler: Model "<<modelName<<" loaded correctly\n";
    }
};
Model* ResourceHandler::getModel(std::string modelName)
{
    if(!modelLoaded(modelName))
    {
        std::cout<<"ResourceHandler: model " << modelName << " not loaded"<<std::endl;
        return nullptr;
    }
    return &this->models.at(modelName);
};
bool ResourceHandler::modelLoaded(std::string modelName)
{
    return this->models.count(modelName) > 0;
};