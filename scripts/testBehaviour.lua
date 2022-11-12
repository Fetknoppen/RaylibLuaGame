local test = {}

--Set variables
local moveRight
local jump
local entityID

function test:start()
    --Called once
    entityID = self.ID
    jump = false;
    grounded = true
   
    --print(entityID)
end

function test:update(delta)

    --Called once per frame
    local trans = scene.GetComponent(entityID, "Transform")

    if (scene.IsKeyPressed("SPACE") and  grounded)then
        print("JUMP")
    end
    if scene.IsKeyDown("A") then
        print("Moving left!")
        trans.position.x = trans.position.x - 1 * delta
    elseif scene.IsKeyDown("D") then
        print("Moving right!")
        trans.position.x = trans.position.x + 1 * delta
    end

    scene.SetComponent(entityID, "Transform", trans)

    scene.SetCamPos(trans.position + vector(0,0,10))


end

return test