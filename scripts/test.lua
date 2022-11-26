scene.LoadModel("UBot-OBJ.obj")

entity = scene.CreateEntity()
scene.SetComponent(entity, "Mesh", "UBot-OBJ.obj")
scene.SetComponent(entity, "Behaviour", "testBehaviour.lua")
local trans = scene.GetComponent(entity, "Transform")

scene.SetComponent(entity, "Transform", trans)
trans.scale = vector(0.15,0.15,0.15)
trans.position = vector(10, 30, 0)
scene.SetComponent(entity, "Transform", trans)

--Position    sizeX    SizeY
scene.SetComponent(entity, "Collider", trans.position, 25, 25)
print("Set collider!\n\n")


