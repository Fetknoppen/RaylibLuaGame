#include "Editor.hpp"
#include "raylib.h"


Editor::Editor(Scene* scene, int sizeX, int sizeY)
    :gridSizeX(sizeX), gridSizeY(sizeY)
{
    setUpGrid();
    this->currentType = 1;
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
