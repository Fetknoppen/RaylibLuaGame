local test = {}

--Set variables
local moveRight
local jump
local entityID
local speed
local moveDelta

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
        --print("Moving left!")
        trans.position.x = trans.position.x - speed * delta
    elseif scene.IsKeyDown("D") then
        --print("Moving right!")
        trans.position.x = trans.position.x + speed * delta
    elseif scene.IsKeyDown("W") then
        --print("Moving up!")
        trans.position.y = trans.position.y + speed * delta
    elseif scene.IsKeyDown("S") then
        --print("Moving down!")
        trans.position.y = trans.position.y - speed * delta
    end

    scene.SetComponent(entityID, "Transform", trans)

    scene.SetCamPos(trans.position + vector(0,30,150))

end

function test:onCollision(type)
    --called on collision
    print("Collision "..type)
end

function test:reset()
    print("RESET")
    local trans = scene.GetComponent(entityID, "Transform")
    trans.position = vector(10, 30, 0)
    scene.SetComponent(entityID, "Transform", trans)
end

return test