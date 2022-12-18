scene.LoadModel("advancedCharacter.obj")
scene.LoadModel("basicCharacter.obj")

entity = scene.CreateEntity()
scene.SetComponent(entity, "Mesh", "advancedCharacter.obj")

--Removing and adding mesh just for demonstration
scene.RemoveComponent(entity, "Mesh")
scene.SetComponent(entity, "Mesh", "advancedCharacter.obj")


scene.SetComponent(entity, "Behaviour", "player.lua")
local trans = scene.GetComponent(entity, "Transform")

--scene.SetComponent(entity, "Transform", trans)
--trans.scale = vector(0.1,0.1,0.1)
trans.position = vector(10, 30, 0)
scene.SetComponent(entity, "Transform", trans)

--Position    sizeX    SizeY
scene.SetComponent(entity, "Collider", trans.position, 10, 10)
print("Set collider!\n\n")


