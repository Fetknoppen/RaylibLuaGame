#pragma once
#include <vector>
#include <string>

#include "Scene.hpp"
#include "raylib.h"

enum class State { MAIN_MENU, GAME, EDITOR};

class Game {
public:
	Game(Scene* scene);
	~Game();
	void Run();
private:
	Scene* scene;
	State gameState;
	int player;
	std::vector<int> enemies;
	std::vector<int> obstacles;
	
	std::vector<int> gameEnteties;
	std::vector<int> MenuEnteties;
	std::vector<int> EditorEnteties;

private:
	void SetUpEnteties();
	void SetUpSystems();
	void RemoveSystems();

	void DrawMenu();
	void DrawGame();
	void DrawEditor();
};