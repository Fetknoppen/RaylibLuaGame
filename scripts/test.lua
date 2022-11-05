entity = scene.CreateEntity()
scene.SetComponent(entity, "Mesh", "UBot-OBJ.obj")
trans = scene.GetComponent(entity, "Transform")
scene.RemoveComponent(entity, "Transform")
trans.position = trans.position + vector(3, 0, 0)
scene.SetComponent(entity, "Transform", trans)
trans = scene.GetComponent(entity, "Transform")
print(trans.position)
return trans

