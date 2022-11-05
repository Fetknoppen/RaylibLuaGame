#include "Scene.hpp"
#include "lua.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for(auto& s: m_systems)
	{
		delete s.second;
	}
}

void Scene::init()
{
	this->camera = { 0 };
    this->camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    this->camera.target = (Vector3){ 0.0f, 1.8f, 0.0f };
    this->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    this->camera.fovy = 60.0f;
    this->camera.projection = CAMERA_PERSPECTIVE;

	SetCameraMode(camera, CAMERA_FREE);
}

void Scene::draw()
{
	BeginMode3D(this->camera);

	auto view = this->m_registry.view<TransformComponent, MeshComponent>();
	view.each([&](const TransformComponent& transform, const MeshComponent& meshComp)
	{
		Model* modelPtr = this->rsHandler.getModel(meshComp.name);
		if (modelPtr != nullptr)
		{
			DrawModel(*modelPtr, transform.position, 0.02f, RED);
		}
		else
		{
			std::cout<<"Error: Model is not loaded yet.\n";
		}
	});

	EndMode3D();
}

void Scene::AddSystem(std::string name, System *system)
{
	this->m_systems.insert({name, system});
}

void Scene::RemoveSystem(std::string name)
{
	delete this->m_systems[name];
	this->m_systems.erase(name);
}

int Scene::GetEntityCount()
{
	return (int)m_registry.alive();
}

int Scene::CreateEntity()
{
	int ret = (int)m_registry.create();
	this->SetComponent<TransformComponent>(ret);
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
	}
}

void Scene::UpdateSystems(float delta)
{
	for(auto& s: this->m_systems){
		if(s.second->OnUpdate(m_registry, delta))
		{
			delete s.second;
			m_systems.erase(s.first);
		}
	}
}

void Scene::lua_openscene(lua_State* L, Scene* scene)
{
	lua_newtable(L);
	luaL_Reg methods[] = {	// this->luaComponents.push_back("Transform");
	// this->luaComponents.push_back("Mesh");
		{"LoadModel", lua_LoadModel},
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
}

int Scene::lua_LoadModel(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);
	std::string modelName = lua_tostring(L, 1);
	scene->rsHandler.loadModel(modelName);
	return 0;
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

	
	if (type == "Transform") {
		TransformComponent transform = lua_totransform(L, 3);
		scene->SetComponent<TransformComponent>(entity, transform);
	}
	else if(type == "Mesh"){
		MeshComponent meshComp(lua_tostring(L, 3));
		scene->SetComponent<MeshComponent>(entity, meshComp);
	}
	else if(type == "Behaviour"){
		if(scene->HasComponents<Behaviour>(entity)){
			scene->RemoveComponent<Behaviour>(entity);
		}
		std::string file = lua_tostring(L, 3);
		file = "../scripts/"+file;
		luaL_dofile(L, file.c_str());
		lua_pushvalue(L, -1);
		int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_pushinteger(L, entity);
		lua_setfield(L, -2, "ID");

		lua_pushstring(L, file.c_str());
		lua_setfield(L, -2, "path");

		lua_getfield(L, -1, "start");
		lua_pushvalue(L, -2);
		lua_pcall(L, 1, 0, 0);

		scene->SetComponent<Behaviour>(entity, file.c_str(), ref);
		
		return 1;
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

	
	if (type == "Transform") {
		hascomponent = scene->HasComponents<TransformComponent>(entity);
	}
	else if (type == "Mesh") {
		hascomponent = scene->HasComponents<Mesh>(entity);
	}
	else if(type == "Behaviour"){
		hascomponent = scene->HasComponents<Behaviour>(entity);
	}

	lua_pushboolean(L, hascomponent);
	return 1;
}

int Scene::lua_GetComponent(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);
	int entity = (int)lua_tointeger(L, 1);
	std::string type = lua_tostring(L, 2);

	
	if (type == "Transform" && scene->HasComponents<TransformComponent>(entity)) {
		TransformComponent& transform = scene->GetComponent<TransformComponent>(entity);
		lua_pushtransform(L, transform);
		return 1;
	}
	else if (type == "Mesh" && scene->HasComponents<MeshComponent>(entity)) {
		MeshComponent& mesh = scene->GetComponent<MeshComponent>(entity);
		lua_pushstring(L, mesh.name.c_str());
		return 1;
	}
	else if (type == "Behaviour" && scene->HasComponents<Behaviour>(entity)) {
		Behaviour& behavior = scene->GetComponent<Behaviour>(entity);
		lua_rawgeti(L, LUA_REGISTRYINDEX, behavior.luaRef);
		return 1;
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

int Scene::lua_RemoveComponent(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);

	int entity = (int)lua_tointeger(L, 1);
	std::string type = lua_tostring(L, 2);

	
	
	if (type == "Transform") {
		scene->RemoveComponent<TransformComponent>(entity);
	}
	else if (type == "Mesh") {
		scene->RemoveComponent<MeshComponent>(entity);
	}
	else if(type == "Behaviour"){
		scene->RemoveComponent<Behaviour>(entity);
	}
	return 0;
}
