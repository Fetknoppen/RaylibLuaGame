local test = {}

--Set variables
local moveRight
local jump
local entityID
local speed

function test:start()
    --Called once
    entityID = self.ID
    jump = false;
    grounded = true
    speed = 100
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
        trans.position.x = trans.position.x - speed * delta
    elseif scene.IsKeyDown("D") then
        print("Moving right!")
        trans.position.x = trans.position.x + speed * delta
    end

    scene.SetComponent(entityID, "Transform", trans)

    scene.SetCamPos(trans.position + vector(0,30,150))


end

function test:reset()
    print("RESET")
    local trans = scene.GetComponent(entityID, "Transform")
    trans.position = vector(10, 30, 0)
    scene.SetComponent(entityID, "Transform", trans)
end

return test