#include "Button.hpp"

Button::Button(std::string label, std::string fileName, Vector2 position, float width, float height,  Color color)
    :label(label), position(position), width(width), height(height), color(color)
{
    std::string path = "../assets/sprites/"+fileName;
    Image image = LoadImage(path.c_str());
    ImageResize(&image, this->width, this->height);
    this->background = LoadTextureFromImage(image);

    this->fontSize = this->width/(int)this->label.size();
};

Button::~Button(){};

void Button::draw()
{
    DrawTexture(this->background, this->position.x, this->position.y, this->color);
    DrawText(this->label.c_str(), this->position.x, this->position.y, this->fontSize, BLACK);
}

bool Button::canClick()
{
    bool ret = false;
    Vector2 mousePos = GetMousePosition();
    Vector2 min = this->position;
    Vector2 max = Vector2({this->position.x + this->width, this->position.y + this->height});

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

bool Button::clicked()
{
    bool ret = false;
    if(canClick() && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        ret = true;
    }
    return ret;
}
std::string Button::getName() {
    return this->label;    
}
