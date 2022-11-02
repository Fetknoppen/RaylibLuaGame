#include "Scene.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
	for (int i = 0; i < (int)m_systems.size(); i++) {
		delete (m_systems[i]);
	}
}

int Scene::GetEntityCount()
{
	return (int)m_registry.alive();
}

int Scene::CreateEntity()
{
	int ret = (int)m_registry.create();
	this->SetComponent<Transform>(ret);
	return  ret;
}

bool Scene::IsEntity(int entity)
{
	return m_registry.valid((entt::entity)entity);
}

void Scene::RemoveEntity(int entity)
{
	if (this->IsEntity(entity)) {
		m_registry.destroy((entt::entity)entity);
	}
	else {
		std::cout << "Error: Invalid entity" << std::endl;
	}
}

void Scene::UpdateSystems(float delta)
{
	for (auto it = m_systems.begin(); it != m_systems.end(); it++) {
		if ((*it)->OnUpdate(m_registry, delta)) {
			delete (*it);
			it = m_systems.erase(it);
		}
	}
}

void Scene::lua_openscene(lua_State* L, Scene* scene)
{
	lua_newtable(L);
	luaL_Reg methods[] = {
		// { "MethodNameInLua", FunctionNameInC }
		{"CreateEntity", lua_CreateEntity},
		{"SetComponent", lua_SetComponent},
		{"GetEntityCount", lua_GetEntityCount},
		{"IsEntity", lua_IsEntity},
		{"RemoveComponent", lua_RemoveComponent},
		{"HasComponent", lua_HasComponent},
		{"GetComponent", lua_GetComponent},
		{"RemoveComponent", lua_RemoveComponent},
		//This array has to end with {NULL, NULL}, 
		//so that luaL_setfuncs know to stop reading
		{NULL, NULL}
	};

	lua_pushlightuserdata(L, scene);

	luaL_setfuncs(L, methods, 1); // 1 : one upvalue (lightuserdata)

	lua_setglobal(L, "scene");
}

Scene* Scene::lua_GetSceneUpValue(lua_State* L)
{
	Scene* scene = nullptr;
	if (lua_isuserdata(L, lua_upvalueindex(1))) {
		scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	}
	return scene;
	return nullptr;
}

int Scene::lua_CreateEntity(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);
	int entity = scene->CreateEntity();
	lua_pushinteger(L, entity);
	return 1;
}

int Scene::lua_SetComponent(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);

	int entity = (int)lua_tointeger(L, 1);
	std::string type = lua_tostring(L, 2);

	if (type == "vector3") {
		Vector3 vec = lua_tovector3(L, 3);
		scene->SetComponent<Vector3>(entity, vec);
	}
	if (type == "vector4") {
		Vector4 vec = lua_tovector4(L, 3);
		scene->SetComponent<Vector4>(entity, vec);
	}
	else if (type == "transform") {
		Transform transform = lua_totransform(L, 3);
		scene->SetComponent<Transform>(entity, transform);
	}

	return 0;
}

int Scene::lua_GetEntityCount(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);

	int count = scene->GetEntityCount();

	lua_pushinteger(L, count);

	return 1;
}

int Scene::lua_IsEntity(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);
	int entity = (int)lua_tointeger(L, 1);
	bool alive = scene->IsEntity(entity);
	lua_pushboolean(L, alive);
	return 1;
}

int Scene::lua_RemoveEntity(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);
	int entity = (int)lua_tointeger(L, 1);
	scene->RemoveEntity(entity);
	return 0;
}

int Scene::lua_HasComponent(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);
	int entity = (int)lua_tointeger(L, 1);
	std::string type = lua_tostring(L, 2);
	bool hascomponent = false;

	
	if (type == "vector3") {
		hascomponent = scene->HasComponents<Vector3>(entity);
	}
	else if (type == "vector4") {
		hascomponent = scene->HasComponents<Vector4>(entity);
	}
	else if (type == "transform") {
		hascomponent = scene->HasComponents<Transform>(entity);
	}

	lua_pushboolean(L, hascomponent);
	return 1;
}

int Scene::lua_GetComponent(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);
	int entity = (int)lua_tointeger(L, 1);
	std::string type = lua_tostring(L, 2);

	
	if (type == "vector3" && scene->HasComponents<Vector3>(entity)) {
		Vector3& vec = scene->GetComponent<Vector3>(entity);
		lua_pushvector3(L, vec);
		return 1;
	}
	if (type == "vector4" && scene->HasComponents<Vector4>(entity)) {
		Vector4& vec = scene->GetComponent<Vector4>(entity);
		lua_pushvector4(L, vec);
		return 1;
	}
	else if (type == "transform" && scene->HasComponents<Transform>(entity)) {
		Transform& transform = scene->GetComponent<Transform>(entity);
		lua_pushtransform(L, transform);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

int Scene::lua_RemoveComponent(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);

	int entity = (int)lua_tointeger(L, 1);
	std::string type = lua_tostring(L, 2);

	
	if (type == "vector3") {
		scene->RemoveComponent<Vector3>(entity);
	}
	else if (type == "transform") {
		scene->RemoveComponent<Transform>(entity);
	}
	return 0;
}
