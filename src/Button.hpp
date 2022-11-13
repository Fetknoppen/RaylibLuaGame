
#include <string>
#include <iostream>

#include "raylib.h"

class Button
{
public:
    Button(std::string label, std::string fileName, Vector2 position = Vector2({0.0f, 0.0f}), float width = 20.0f, float height = 10.0f, Color color = WHITE);
    ~Button();
    void draw();
    bool canClick();
    bool clicked();
    std::string getName();
private:
    Texture2D background;
    std::string label;
    Color color;
    Vector2 position;
    float width;
    float height;
    int fontSize;
};