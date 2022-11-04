#include "Game.hpp"

Game::Game( lua_State* L)
{
    this->scene = new Scene();
    this->scene->lua_openscene(L, this->scene);

    luaL_loadfile(L, "../scripts/vector.lua");
	lua_pcall(L, 0, 1, 0);
	lua_setglobal(L, "vector");

    luaL_dofile(L, "../scripts/test.lua");
}

Game::~Game()
{
    
}

void Game::run()
{
   
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    std::cout<<"Inited window\n";

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    this->scene->init();

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
    
}