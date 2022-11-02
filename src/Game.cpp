#include "Game.hpp"
#include <raylib.h>

Game::Game(Scene* scene)
{
	this->scene = scene;

	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "3D Mario");
	SetTargetFPS(60);

	

	this->gameState = State::GAME;

	SetUpEnteties();
	SetUpSystems();

	SetCameraMode(this->scene->GetComponent<Camera>(this->player), CAMERA_FREE);     // Set a free camera mode
}

Game::~Game()
{
	RemoveSystems();
}

void Game::Run()
{

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		this->scene->UpdateSystems(GetFrameTime());
		

		BeginDrawing();


			ClearBackground(RAYWHITE);

			switch (gameState)
			{
			case State::MAIN_MENU:
				DrawMenu();
				break;
			case State::GAME:
				DrawGame();
				break;
			case State::EDITOR:
				DrawEditor();
				break;
			}

			

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
}

void Game::SetUpEnteties()
{
	//Player
	this->player = this->scene->CreateEntity();
	Transform& transform = this->scene->GetComponent<Transform>(this->player);
	transform.scale = Vector3({0.1f, 0.0, 0.0f});
	this->scene->SetComponent<PlayerComponent>(this->player);
	this->scene->SetComponent<LivingComponent>(this->player);
	LivingComponent& livComp = this->scene->GetComponent<LivingComponent>(this->player);
	livComp.health = 100;
	livComp.speed = 10;
	this->scene->SetComponent<PhysichsComponent>(this->player, Vector3({0.f, 0.0f, 0.0f}), 9.82f, 0.2f, 0.2f);
	Model playerModel = LoadModel("../assets/obj/UBot-OBJ.obj");
	Texture2D playerTexture = LoadTexture("../assets/textures/UBot-Diff.png");
	SetMaterialTexture(&playerModel.materials[0], MATERIAL_MAP_DIFFUSE, playerTexture);
	this->scene->SetComponent<MeshComponent>(this->player, playerModel, playerTexture);
	this->scene->SetComponent<CollisionComponent>(this->player, GetMeshBoundingBox(playerModel.meshes[0]));
	Camera camera = { 0 };
	camera.position = Vector3({0.0f, 0.0f, -70.0f}); // Camera position
	camera.target = Vector3({0.0f, 0.0f, 0.0f});     // Camera looking at point
	camera.up = Vector3({0.0f, 1.0f, 0.0f});          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type
	this->scene->SetComponent<Camera>(this->player, camera);
	
	this->gameEnteties.push_back(player);

	for (int i = 0; i < 10; i++) {
		this->obstacles.push_back(this->scene->CreateEntity());
		int ent = this->obstacles[i];
		Transform& trans = this->scene->GetComponent<Transform>(ent);
		trans.scale = Vector3({0.1f, 0.0, 0.0f});
		trans.rotation = QuaternionFromAxisAngle(Vector3({0, 1, 0}), DEG2RAD * (rand() % 361));
		trans.translation = Vector3({(float)(rand() % 1000 - 500), 0.0f, (float)(rand() % 1000 - 500)});
		this->scene->SetComponent<MeshComponent>(ent, playerModel, playerTexture);
	}
}

void Game::SetUpSystems()
{
	this->scene->CreateSystem<PlayerMovementSystem>();
	this->scene->CreateSystem<PlayerCameraSystem>();
	this->scene->CreateSystem<ModelMoveSystem>();
	this->scene->CreateSystem<PhysicsSystem>();
}

void Game::RemoveSystems()
{
	this->scene->RemoveEntity(this->player);
}

void Game::DrawMenu()
{
	DrawText("Menu", 190, 200, 20, LIGHTGRAY);
	
}

void Game::DrawGame()
{
	Camera& cam = this->scene->GetComponent<Camera>(this->player);
	UpdateCamera(&cam);

	BeginMode3D(cam);

	DrawGrid(100, 100.0f);

	for (int i = 0; i < (int)gameEnteties.size(); i++) {
		Transform& transform = this->scene->GetComponent<Transform>(gameEnteties[i]);
		if (this->scene->HasComponents<MeshComponent>(gameEnteties[i])) {
			Model& model = this->scene->GetComponent<MeshComponent>(gameEnteties[i]).model;
			DrawModel(model, transform.translation, transform.scale.x, WHITE);
		}
	}

	for (int i = 0; i < (int)obstacles.size(); i++) {
		Transform& transform = this->scene->GetComponent<Transform>(obstacles[i]);
		if (this->scene->HasComponents<MeshComponent>(obstacles[i])) {
			Model& model = this->scene->GetComponent<MeshComponent>(obstacles[i]).model;
			DrawModel(model, transform.translation, transform.scale.x, WHITE);
		}
	}
	


	Transform& transform = this->scene->GetComponent<Transform>(this->player);
	PlayerComponent& playerComp = this->scene->GetComponent<PlayerComponent>(this->player);
	Ray ray;
	ray.direction = playerComp.forwardVec;
	ray.position = transform.translation;
	DrawRay(ray, RED);

	EndMode3D();

	std::string vector = "(" + std::to_string(playerComp.forwardVec.x) +", " + std::to_string(playerComp.forwardVec.y) + ", " + std::to_string(playerComp.forwardVec.z)+")";
	Vector3 neutral = Vector3({1.0f, 0.0f, 0.0f});
	std::string angle = std::to_string(Vector3Angle(neutral, playerComp.forwardVec));
	DrawText(vector.c_str(), 10, 20, 20, RED);
	DrawText(angle.c_str(), 10, 40, 20, RED);
}

void Game::DrawEditor()
{
	DrawText("Editor", 190, 200, 20, LIGHTGRAY);
}
