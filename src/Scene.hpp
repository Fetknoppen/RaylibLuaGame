#pragma once
#include <iostream>
#include <map>

#include "entt.hpp"
#include "lua.hpp"
#include "raylib.h"

#include "Systems.hpp"
#include "ResourceHandler.hpp"


class Scene {

	entt::registry m_registry;
	
	std::unordered_map<std::string, System*> m_systems;
	ResourceHandler rsHandler;
	Camera camera;
	inline static std::unordered_map<std::string, int> inputKeys{
        { "0", 48 },
        { "1", 49 },
        { "2", 50 },
        { "3", 51 },
        { "4", 52 },
        { "5", 53 },
        { "6", 54 },
        { "7", 55 },
        { "8", 56 },
        { "9", 57 },
        { "A", 65 },
        { "B", 66 },
        { "C", 67 },
        { "D", 68 },
        { "E", 69 },
        { "F", 70 },
        { "G", 71 },
        { "H", 72 },
        { "I", 73 },
        { "J", 74 },
        { "K", 75 },
        { "L", 76 },
        { "M", 77 },
        { "N", 78 },
        { "O", 79 },
        { "P", 80 },
        { "Q", 81 },
        { "R", 82 },
        { "S", 83 },
        { "T", 84 },
        { "U", 85 },
        { "V", 86 },
        { "W", 87 },
        { "X", 88 },
        { "Y", 89 },
        { "Z", 90 },
        { "SPACE", 32 },
        { "ESCAPE", 256 },
        { "ENTER", 257 },
        { "TAB", 258 },
        { "BACKSPACE", 259 },
        { "RIGHT", 262 },
        { "LEFT", 263 },
        { "DOWN", 264 },
        { "UP", 265 },
    };

public:
	Scene();
	~Scene();

public:
	Camera* getCam();
	void init();
	void draw();
	void resetBehaviours(lua_State* L);
	void AddSystem(std::string name, System* system);
	void RemoveSystem(std::string name);
	void setCameraPosition(Vector3 position);

	int GetEntityCount();

	int CreateEntity();
	bool IsEntity(int entity);
	void RemoveEntity(int entity);

	template <typename...Args>
	bool HasComponents(int entity);

	template <typename T>
	T& GetComponent(int entity);

	template <typename T>
	void SetComponent(int entity, const T& components);

	template <typename T, typename...Args>
	void SetComponent(int entity, Args...args);

	template <typename T>
	void RemoveComponent(int entity);

	template <typename T, typename...Args>
	void CreateSystem(Args...args);

	void UpdateSystems(float delta);

	//Lua
public:
	static void lua_openscene(lua_State* L, Scene* scene);

private:
	static Scene* lua_GetSceneUpValue(lua_State* L);

	static int lua_LoadModel(lua_State* L);
	static int lua_setCameraPos(lua_State* L);
	static int lua_isKeyPressed(lua_State* L);
	static int lua_isKeyDown(lua_State* L);
	static int lua_isKeyUp(lua_State* L);
	static int lua_CreateSystem(lua_State* L);
	static int lua_CreateEntity(lua_State* L);
	static int lua_SetComponent(lua_State* L);
	static int lua_GetEntityCount(lua_State* L);
	static int lua_IsEntity(lua_State* L);
	static int lua_RemoveEntity(lua_State* L);
	static int lua_HasComponent(lua_State* L);
	static int lua_GetComponent(lua_State* L);
	static int lua_RemoveComponent(lua_State* L);


};




template<typename ...Args>
inline bool Scene::HasComponents(int entity)
{
	return m_registry.all_of<Args...>((entt::entity)entity);
}

template<typename T>
inline T& Scene::GetComponent(int entity)
{
	return m_registry.get<T>((entt::entity)entity);
}

template<typename T>
inline void Scene::SetComponent(int entity, const T& components)
{
	m_registry.emplace_or_replace<T>((entt::entity)entity, components);
}

template<typename T, typename ...Args>
inline void Scene::SetComponent(int entity, Args ...args)
{
	m_registry.emplace_or_replace<T>((entt::entity)entity, args...);
}

template<typename T>
inline void Scene::RemoveComponent(int entity)
{
	m_registry.remove<T>((entt::entity)entity);
}

template<typename T, typename ...Args>
inline void Scene::CreateSystem(Args ...args)
{
	m_systems.insert(new T(args...));
}
