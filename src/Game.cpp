#include "Game.hpp"
#include "raylib.h"

Game::Game( lua_State* L)
{
    this->gameState = GAME_STATE::MENU;

    this->L = L;
    this->scene = new Scene();
    this->scene->lua_openscene(L, this->scene);

    luaL_dofile(L, "../scripts/vector.lua");
	lua_setglobal(L, "vector");
}

Game::~Game()
{
    
}

void Game::run()
{
   
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    this->scene->init();
    this->setSystems();
    luaL_dofile(this->L, "../scripts/test.lua");

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        checkMenuSwitch();

        switch (this->gameState)
        {
        case GAME_STATE::MENU:
            drawMenu();
            break;
        case GAME_STATE::GAME:
            drawGame();
            break;
        case GAME_STATE::EDITOR:
            drawEditor();
            break;
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}

void Game::setSystems()
{
    this->scene->AddSystem(new BehaviourSystem(L));
}

void Game::checkMenuSwitch()
{
    if(IsKeyDown(KEY_M))
    {
        this->gameState = GAME_STATE::MENU;
    }
    else if(IsKeyDown(KEY_G))
    {
        this->gameState = GAME_STATE::GAME;
    }
    else if(IsKeyDown(KEY_E))
    {
        this->gameState = GAME_STATE::EDITOR;
    }
}

void Game::drawMenu()
{
    //Update
    this->scene->UpdateSystems(GetFrameTime());
    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Current: Menu\nMenu: M\nGame: G\nEditor: E", 190, 200, 20, LIGHTGRAY);
        this->scene->draw();
    EndDrawing();   
}

void Game::drawGame()
{
    //Update
    this->scene->UpdateSystems(GetFrameTime());
    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Current: Game\nMenu: M\nGame: G\nEditor: E", 190, 200, 20, LIGHTGRAY);
        this->scene->draw();
    EndDrawing();   
}

void Game::drawEditor()
{
    //Update
    this->scene->UpdateSystems(GetFrameTime());
    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Current: Editor\nMenu: M\nGame: G\nEditor: E", 190, 200, 20, LIGHTGRAY);
        this->scene->draw();
    EndDrawing();   
}