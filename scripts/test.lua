scene.LoadModel("UBot-OBJ.obj")

entity = scene.CreateEntity()
scene.SetComponent(entity, "Mesh", "UBot-OBJ.obj")
scene.SetComponent(entity, "Behaviour", "testBehaviour.lua")

