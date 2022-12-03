#include "Game.hpp"
#include "raylib.h" 

Game::Game( lua_State* L)
{
    this->gameState = GAME_STATE::MENU;

    this->L = L;
    this->scene = new Scene();
    this->scene->lua_openscene(L, this->scene);
    this->map = new mapLoader(this->scene);
    this->editor = new Editor(this->scene);
    this->mapSave = false;

    luaL_dofile(L, "../scripts/vector.lua");
	lua_setglobal(L, "vector");
}

Game::~Game()
{
    //TODO: Check here why I get segmentation fault
    //delete this->scene;
    //delete this->map;
    //delete this->editor;
    std::cout<<"Game deconstructor\n";
    CloseWindow(); 
}

void Game::run()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    this->scene->init();

    startLevelSelector();

    luaL_dofile(this->L, "../scripts/init.lua");

    this->buttons.push_back(Button("Start", "button1.png", {20.0f, 20.0f}, 100, 20, WHITE));
    this->buttons.push_back(Button("Editor", "button1.png", {20.0f, 60.0f}, 100, 20, WHITE));
    this->buttons.push_back(Button("Quit", "button1.png", {20.0f, 100.0f}, 100, 20, WHITE));

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        checkMenuSwitch();

        switch (this->gameState)
        {
        case GAME_STATE::MENU:
            drawMenu();
            break;
        case GAME_STATE::LEVEL_SELECTOR:
            drawLevelSelector();
            break;
        case GAME_STATE::GAME:
            drawGame();
            break;
        case GAME_STATE::EDITOR:
            drawEditor();
            break;
        case GAME_STATE::QUIT:
            CloseWindow();
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
    this->scene->AddSystem("BehaviourSystem",new BehaviourSystem(L));
    this->scene->AddSystem("CollisionSystem",new CollisionSystem(L));
    this->scene->AddSystem("RayPhysSystem",new RayPhysSystem(L));
}

void Game::checkMenuSwitch()
{
    // //To menu
    // if(IsKeyDown(KEY_M))
    // {
    //     if(this->gameState == GAME_STATE::GAME)
    //     {
    //         this->map->unLoad();
    //     }
    //     if(this->gameState != GAME_STATE::MENU)
    //     {
    //         this->gameState = GAME_STATE::MENU;
    //         this->startMenu();
    //     }
    // }
    // //To game
    // else if(IsKeyDown(KEY_G))
    // {
    //     if(this->gameState != GAME_STATE::GAME)
    //     {
    //         this->gameState = GAME_STATE::GAME;
    //         this->startGame();
    //     }
    // }
    // //To editor
    // else if(IsKeyDown(KEY_E))
    // {
    //     if(this->gameState == GAME_STATE::GAME)
    //     {
    //         this->map->unLoad();
    //     }
    //     if(this->gameState != GAME_STATE::EDITOR)
    //     {
    //         this->gameState = GAME_STATE::EDITOR;
    //         this->startEditor();
    //     }
    // }
}

void Game::startMenu()
{
    this->scene->resetBehaviours(L);
    this->scene->RemoveSystem("BehaviourSystem");
    this->scene->RemoveSystem("CollisionSystem");
    this->scene->RemoveSystem("RayPhysSystem");
    //this->scene->setCameraPosition((Vector3){0.0f,0.0f, 10.0f});
}

void Game::drawMenu()
{
    //Update
    this->scene->UpdateSystems(GetFrameTime());
    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        this->scene->draw();

        for(auto& b: this->buttons)
        {
            b.draw();
            if(b.clicked()){
               std::string buttonName = b.getName();
               if(buttonName == "Start")
               {
                    this->gameState = GAME_STATE::LEVEL_SELECTOR;
                    this->startLevelSelector();
                    //this->gameState = GAME_STATE::GAME;
                    //this->startGame();
               }
               else if(buttonName == "Editor")
               {
                    this->gameState = GAME_STATE::EDITOR;
                    this->startEditor();
               }
               else if(buttonName == "Quit")
               {
                    this->gameState = GAME_STATE::QUIT;
               }
            }
        }

    EndDrawing();   
}

void Game::startGame()
{
    //luaL_dofile(this->L, "../scripts/test.lua");
    this->scene->AddSystem("BehaviourSystem",new BehaviourSystem(L));
    this->scene->AddSystem("CollisionSystem",new CollisionSystem(L));
    this->scene->AddSystem("RayPhysSystem",new RayPhysSystem(L));
    this->map->load(L);
    //this->scene->setCameraPosition((Vector3){0.0f,0.0f, 10.0f});
}

void Game::drawGame()
{
    //Update
    this->scene->UpdateSystems(GetFrameTime());
    if(IsKeyPressed(KEY_DELETE) || this->scene->win)
    {
        this->scene->win = false;
        this->map->unLoad();
        this->startMenu();
        this->gameState = GAME_STATE::MENU;
    }
   
    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("DELETE: menu", 190, 200, 20, LIGHTGRAY);
        this->scene->draw();
    EndDrawing();   
}

void Game::startEditor()
{
    this->editor->init();
    this->scene->RemoveSystem("BehaviourSystem");
    this->scene->RemoveSystem("CollisionSystem");
    this->scene->RemoveSystem("RayPhysSystem");
    this->scene->resetBehaviours(L);
    //this->scene->setCameraPosition((Vector3){0.0f,0.0f, 300.0f});

}

void Game::drawEditor()
{
    // //Update

    this->scene->UpdateSystems(GetFrameTime());
    if( this->mapSave)
    {
        char c = GetKeyPressed();
        if(c != 0)
        {
            std::cout<<"input char: "<<c<<std::endl;
            this->mapSaveName += c;
        }
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            if(this->mapSaveName.length() > 0)
            {
                std::cout<<"Pop\n";
                this->mapSaveName.pop_back();
            }
        }
        else if(IsKeyPressed(KEY_ENTER))
        {
            std::cout<<"Save this shit!"<<std::endl;
            if(this->mapSaveName.size() > 0 && this->editor->save(this->mapSaveName))
            {
                this->editor->reset();
                this->mapSaveName = "";
                this->mapSave = false;
            }
            else
            {
                std::cout<<"Name already in use!"<<std::endl;
            }
        }
        if(IsKeyPressed(KEY_DELETE))
        {
            this->mapSaveName = "";
            this->mapSave = false;
            this->startMenu();
            this->gameState = GAME_STATE::MENU;
        }
    }
    else
    {
        this->editor->update();

        if(IsKeyPressed(KEY_ENTER))
        {
            this->mapSave = true;
            this->mapSaveName = "";
        }
        if(IsKeyPressed(KEY_DELETE))
        {
            this->mapSaveName = "";
            this->mapSave = false;
            this->startMenu();
            this->gameState = GAME_STATE::MENU;
        }
    }

    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);

        this->scene->draw();
        if( this->mapSave)
        {
            std::string disp = "Name: "+this->mapSaveName;
            DrawText(disp.c_str(), 10, 10, 20, LIGHTGRAY);
            DrawText("Enter: Save\nDELETE: Return to menu", 190, 200, 20, LIGHTGRAY);
        }
        else
        {
            //BeginMode3D(*this->scene->getCam());
            this->editor->draw();
            //EndMode3D();
            DrawText("1: Ground\n2: Enemy\nEnter: Save\nDELETE: Return to menu", 10, 10, 20, LIGHTGRAY);
        }
        
    EndDrawing();   
}

void Game::startLevelSelector() {

    this->scene->RemoveSystem("BehaviourSystem");
    this->scene->RemoveSystem("CollisionSystem");
    this->scene->RemoveSystem("RayPhysSystem");

    for(auto* b: this->mapButtons)
    {
        delete b;
    }
    this->mapButtons.clear();

    std::cout<<"Starting level selector\n";
    int i = 0;
    int x = 0;
    int y = 0;
    float buffer = 20.0f;
    for(auto f: *this->map->getFiles())
    {
        float buttonWidth = 30.0f;
        float buttonHeight = 20.0f;
        Vector2 buttonPos =  {0.0f, 0.0f};
        buttonPos.x = x * (buttonWidth+buffer);
        buttonPos.y = y * (buttonHeight+buffer);
        x++;
        if(buttonPos.x > GetScreenWidth())
        {
            x = 0;
            y++;
            buttonPos.x = x * (buttonWidth+buffer);
            buttonPos.y = y * (buttonHeight+buffer);
        }
        this->mapButtons.push_back(new Button(f, "button1.png", buttonPos, buttonWidth, buttonHeight, GRAY));
        //std::cout<<f<<std::endl;
        i++;
    }

    std::cout<<"Number of buttons: "<<this->mapButtons.size()<<std::endl;
}

void Game::drawLevelSelector() {
    // Draw
    BeginDrawing();
        ClearBackground(RAYWHITE);
        this->scene->draw();
        for(auto& b: this->mapButtons)
        {
           b->draw();
           if(b->clicked())
           {
                this->map->setCurrentMap(b->getName());
                this->gameState = GAME_STATE::GAME;
                startGame();
                return;
           }
        }
    EndDrawing(); 
}
