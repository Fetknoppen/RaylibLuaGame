

#include "Scene.hpp"
#include "raylib.h"

struct cell
{
    Vector2 position;
    const Vector2 size = (Vector2){30.0f, 30.0f};
    bool used = false;
    int type = 0;

    cell(Vector2 position):position(position){}

    void draw()
    {
        Vector3 drawPos = (Vector3){this->position.x, this->position.y, 0.0f};
        switch (this->type)
        {
        case 0:
            //Unused
            DrawCubeWires(drawPos, this->size.x, this->size.y, 2.0f, YELLOW);
            break;
        case 1:
            //Regular block
            DrawCube(drawPos, this->size.x, this->size.y, 2.0f, GREEN);
            break;
        case 2:
            //Enemy
            DrawCube(drawPos, this->size.x, this->size.y, 2.0f, RED);
            break;
        }
    }
    void setType(int type)
    {
        this->type = type;
    }
    bool canClick()
    {
        bool ret = false;
        Vector2 mousePos = GetMousePosition();
        Vector2 min = Vector2({this->position.x - this->size.x/2, this->position.y - this->size.y/2});
        Vector2 max = Vector2({this->position.x + this->size.x/2, this->position.y + this->size.y/2});
        if(mousePos.x >= min.x && mousePos.y >= min.y )
        {
            if(mousePos.x <= max.x && mousePos.y <= max.y)
            {
                ret = true;
               
            }
        }

        // std::cout<<"MousePos: ("<<mousePos.x << ", " << mousePos.y << ")\n"; 
        // std::cout<<"min: ("<<min.x << ", " << min.y << ")\n"; 
        // std::cout<<"max: ("<<max.x << ", " << max.y << ")\n"; 
        return ret;
    }

};

class Editor
{
public:
    Editor(Scene* scene, int sizeX = 100, int sizeY = 10);
    ~Editor();
    void update();
    void draw();
private:
    Scene* scene;
    int gridSizeX;
    int gridSizeY;
    int currentType;
    std::vector<cell> grid;

private:
    void setUpGrid();
    
};