#include "Scene.hpp"
#include "raylib.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	std::cout<<"Scene deconstructor start\n";
	for(auto& s: m_systems)
	{
		delete s.second;
	}
	std::cout<<"Scene deconstructor end\n";
}

void Scene::init()
{
	setCameraPosition({ 0.0f, 0.0f, 10.0f });
	this->win = false;
}

void Scene::draw()
{
	BeginMode3D(this->camera);

	auto view = this->m_registry.view<TransformComponent, MeshComponent>();
	view.each([&](const TransformComponent& transform, const MeshComponent& meshComp)
	{
		if(meshComp.name == "cube")
		{
			DrawCube(transform.position, CELL_SIZE.x, CELL_SIZE.y, CELL_SIZE.x, BLUE);
		}
		else
		{
			Model* modelPtr = this->rsHandler.getModel(meshComp.name);
			if (modelPtr != nullptr)
			{
				//DrawModel(*modelPtr, transform.position, transform.scale.x, WHITE);
				DrawModelEx(*modelPtr, transform.position, {0,1,0}, transform.rotation.y, transform.scale, WHITE);
				//std::cout<<"Drawing model: "<< meshComp.name <<"\n";
			}
			else
			{
				std::cout<<"Error: Model is not loaded yet.\n";
			}
		}
	});

	auto view2 = this->m_registry.view<TransformComponent, CollisionComp, MeshComponent>();
	view2.each([&](const TransformComponent& transform, const CollisionComp& col, const MeshComponent& meshComp)
	{
		if(meshComp.name == "cube")
		{
			DrawBoundingBox(col.box, GREEN);
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
	if (this->IsEntity(entity)) 
	{
		m_registry.destroy((entt::entity)entity);
	}

	else
	{
		std::cout<<"Error: Cant remove invalid entity.\n";
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
		{"SetCamPos", lua_setCameraPos},
		{"winGame", lua_winGame},
		{"GetScreenWidth", lua_getScreenWidth},
		{"IsKeyPressed", lua_isKeyPressed},
		{"IsKeyDown", lua_isKeyDown},
		{"IsKeyUp", lua_isKeyUp},
		{"CreateEntity", lua_CreateEntity},
		{"SetComponent", lua_SetComponent},
		{"GetEntityCount", lua_GetEntityCount},
		{"IsEntity", lua_IsEntity},
		{"RemoveComponent", lua_RemoveComponent},
		{"HasComponent", lua_HasComponent},
		{"GetComponent", lua_GetComponent},
		{"RemoveEntity", lua_RemoveEntity},
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
	else if(type == "Collider"){
		Vector3 position = lua_tovector(L, 3);
		float sizeX = lua_tonumber(L, 4);
		float sizeY = lua_tonumber(L, 5);
		BoundingBox box;
		box.min = position;
		box.min.x -= sizeX/2.0f;
		box.min.y -= sizeY/2.0f;
		box.max = position;
		box.max.x += sizeX/2.0f;
		box.max.y += sizeY/2.0f;
		CollisionComp col;
		col.box = box;
		scene->SetComponent<CollisionComp>(entity, col);
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
	else if(type == "Collider"){
		hascomponent = scene->HasComponents<CollisionComp>(entity);
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
	else if(type == "Collider"){
		scene->RemoveComponent<CollisionComp>(entity);
	}
	return 0;
}

int Scene::lua_setCameraPos(lua_State* L) {
    Scene* scene = lua_GetSceneUpValue(L);
	Vector3 position = lua_tovector(L, 1);
	scene->setCameraPosition(position);
	return 0;
}

int Scene::lua_winGame(lua_State* L)
{
	Scene* scene = lua_GetSceneUpValue(L);
	scene->win = true;
	return 1;
}

void Scene::setCameraPosition(Vector3 position) {
	this->camera = { 0 };
    this->camera.position = position;
    this->camera.target = {position.x, +position.y, position.z - 1.0f};
    this->camera.up = { 0.0f, 1.0f, 0.0f };
    this->camera.fovy = 60.0f;
    this->camera.projection = CAMERA_PERSPECTIVE;
}

int Scene::lua_getScreenWidth(lua_State* L) {
	lua_pushinteger(L, GetScreenWidth());
	return 1;
}

int Scene::lua_isKeyPressed(lua_State* L) {
	Scene* scene = lua_GetSceneUpValue(L);
    std::string key = lua_tostring(L, 1);
	lua_pushboolean(L, IsKeyPressed(scene->inputKeys[key]));
	return 1;
}

int Scene::lua_isKeyDown(lua_State* L) {
    Scene* scene = lua_GetSceneUpValue(L);
    std::string key = lua_tostring(L, 1);
	lua_pushboolean(L, IsKeyDown(scene->inputKeys[key]));
	return 1;
}

int Scene::lua_isKeyUp(lua_State* L) {
    Scene* scene = lua_GetSceneUpValue(L);
    std::string key = lua_tostring(L, 1);
	lua_pushboolean(L, IsKeyUp(scene->inputKeys[key]));
	return 1;
}

void Scene::addBehaviour(lua_State* L, int entity, std::string file)
{
	if(this->HasComponents<Behaviour>(entity)){
		this->RemoveComponent<Behaviour>(entity);
	}
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
	this->SetComponent<Behaviour>(entity, file.c_str(), ref);
}

void Scene::resetBehaviours(lua_State* L) {

	auto view = this->m_registry.view<Behaviour>();
	view.each([&](Behaviour& script){
		
		lua_rawgeti(L, LUA_REGISTRYINDEX, script.luaRef);
		lua_getfield(L, -1, "reset");
		lua_pushvalue(L, -2);
		if(lua_pcall(L, 1, 0, 0) != LUA_OK){
			std::cout<<"Failed to call\n";
		}
		lua_pop(L, 1);
	});
}

Camera* Scene::getCam() {
	return &this->camera;
}
