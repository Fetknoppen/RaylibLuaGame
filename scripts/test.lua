scene.LoadModel("UBot-OBJ.obj")

entity = scene.CreateEntity()
scene.SetComponent(entity, "Mesh", "UBot-OBJ.obj")
--print("Behaviour for: ")
--print(entity)
scene.SetComponent(entity, "Behaviour", "testBehaviour.lua")
--scene.GetComponent(entity, "Behaviour").id = entity;
-- trans = scene.GetComponent(entity, "Transform")
-- scene.RemoveComponent(entity, "Transform")
-- trans.position = trans.position + vector(3, 0, 0)
-- scene.SetComponent(entity, "Transform", trans)
-- trans = scene.GetComponent(entity, "Transform")
-- print(trans.position)

