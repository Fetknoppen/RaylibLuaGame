#include "Game.hpp"

Game::Game( lua_State* L)
{
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
        // Update
        this->scene->UpdateSystems(GetFrameTime());

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

            this->scene->draw();

        EndDrawing();
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