#pragma once
#include <iostream>

#include "entt.hpp"
#include "raylib.h"

#include "Systems.hpp"
#include "ResourceHandler.hpp"


class Scene {

	entt::registry m_registry;
	std::unordered_map<std::string, System*> m_systems;
	ResourceHandler rsHandler;

	Camera camera;
public:
	Scene();
	~Scene();

public:
	void init();
	void draw();
	void setInputKeys();
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
	//m_systems.emplace_back(new T(args...));
}
