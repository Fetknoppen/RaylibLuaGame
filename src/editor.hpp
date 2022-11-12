

#include "Scene.hpp"
#include "raylib.h"

struct cell
{
    Vector2 position;
    const Vector2 size = (Vector2){10.0f, 10.0f};
    bool used = false;
    int type = 0;

    cell(Vector2 position):position(position){}

    void draw()
    {
        switch (this->type)
        {
        case 0:
            //Unused
            DrawCubeWires((Vector3){this->position.x, this->position.y, 0.0f}, 2.0f, 2.0f, 2.0f, MAROON);
            break;
        case 1:
            //Regular block
            DrawCube((Vector3){this->position.x, this->position.y, 0.0f}, 2.0f, 2.0f, 2.0f, GREEN);
            break;
        case 2:
            //Enemy
            DrawCubeWires((Vector3){this->position.x, this->position.y, 0.0f}, 2.0f, 2.0f, 2.0f, RED);
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
        Vector2 min = this->position;
        Vector2 max = Vector2({this->position.x + this->size.x, this->position.y + this->size.y});
        if(mousePos.x >= min.x && mousePos.y >= min.y )
        {
            if(mousePos.x <= max.x && mousePos.y <= max.y)
            {
                ret = true;
            }
        }
        return true;
    }
};

class editor
{
public:
    editor(Scene* scene);
    ~editor();
    void start();
    void update();
    void draw();
private:
    std::vector<cell> grid;
};