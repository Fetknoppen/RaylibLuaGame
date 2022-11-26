#include "Editor.hpp"
#include "raylib.h"


Editor::Editor(Scene* scene)
{
  
}

Editor::~Editor() 
{
    std::cout<<"Editor deconstructor start\n";
    std::cout<<"Editor deconstructor end\n";
}

void Editor::update() {

    if(IsKeyPressed(KEY_ONE))
    {
        std::cout<<"Block selected\n";
        this->currentType = 1;
    }
    else if(IsKeyPressed(KEY_TWO))
    {
        std::cout<<"Enemy selected\n";
        this->currentType = 2;
    }

    for(auto& c: this->grid)
    {
        
        if(c.canClick())
        {
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !c.used)
            {
                c.used = true;
                c.type = this->currentType;
                std::cout<<"Block changed\n";
            }
            else if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && c.used)
            {
                c.used = false;
                c.type = 0;
                std::cout<<"Block changed\n";
            }
        }
    }
}

void Editor::draw() {
    for(auto& c: this->grid)
    {
        c.draw();
    }
}

void Editor::setUpGrid() {
    std::cout<<"X: "<<this->gridSizeX<<" Y: "<<this->gridSizeY<<std::endl;
    for(int x = 0; x < this->gridSizeX; x++)
    {
        for(int y = 0; y < this->gridSizeY; y++)
        {
            cell newCell((Vector2){0.0f,0.0f});
            newCell.position.x = (newCell.size.x)*x;
            newCell.position.y = (newCell.size.y)*y;
            this->grid.push_back(newCell);
        }
    }
}

bool Editor::save(std::string name) {

    std::string fileName = "../Levels/"+name+".txt";
    std::ofstream file(fileName);
    if(!file)
    {
        return false;
    }
    for(auto& c: this->grid)
    {
        if(c.used)
        {
            c.position.y = (c.position.y - GetScreenHeight())*(-1.0f) - CELL_SIZE.y;

            file<<c.type<<" "<<c.position.x<<","<<c.position.y<<","<<"0\n";
        }
    }

    return true;
}

void Editor::reset() {
    init();
}

void Editor::init() {
    this->grid.clear();
    this->currentType = 1;
    this->gridSizeX = (int)(GetScreenWidth()/CELL_SIZE.x);
    this->gridSizeY = (int)(GetScreenHeight()/CELL_SIZE.y);
    setUpGrid();   
}
