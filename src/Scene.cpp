#include "Scene.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
	for(auto& s: m_systems)
	{
		delete s;
	}
}

void Scene::init()
{
	this->loadModels();


	this->camera = { 0 };
    this->camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    this->camera.target = (Vector3){ 0.0f, 1.8f, 0.0f };
    this->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    this->camera.fovy = 60.0f;
    this->camera.projection = CAMERA_PERSPECTIVE;

	SetCameraMode(camera, CAMERA_FIRST_PERSON);

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
			std::cout<<"Some models are not loaded!\nLoad them manually in 'loadModels' function in Scene.cpp\n";
		}
	});

	EndMode3D();
}

void Scene::loadModels()
{
	this->rsHandler.loadModel("UBot-OBJ.obj");
}

void Scene::AddSystem(System *system)
{
	this->m_systems.push_back(system);
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
	luaL_Reg methods[] = {	// this->luaComponents.push_back("Transform");
	// this->luaComponents.push_back("Mesh");
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

	lua_newtable(L);
	
	for (int i = 0; i < (int)luaComponents.size(); i++)
	{
		lua_pushnumber(L, i);
		lua_setfield(L, -2, luaComponents[i].c_str());
	}

	lua_setglobal(L, "ComponentType");
}

Scene* Scene::lua_GetSceneUpValue(lua_State* L)
{
	Scene* scene = nullptr;
	if (lua_isuserdata(L, lua_upvalueindex(1))) {
		scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	}
	return scene;
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
	return 0;
}
