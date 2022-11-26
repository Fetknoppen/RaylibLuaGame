local test = {}

--Set variables
local jump
local entityID
local speed
local moveDelta
local grounded

function test:start()
    --Called once
    entityID = self.ID
    jump = false;
    grounded = true
    speed = 100
    moveDelta = vector(0,0,0)
    --print(entityID)
end

function test:update(delta)

    --Called once per frame
    local trans = scene.GetComponent(entityID, "Transform")
    local direction = vector(0,0,0)
    if (scene.IsKeyPressed("SPACE") and  grounded)then
        print("JUMP")
    end
    if scene.IsKeyDown("A") then
        direction = direction + vector(-1,0,0)
    end
    if scene.IsKeyDown("D") then
        direction = direction + vector(1,0,0)
    end
    if scene.IsKeyDown("W") then
        direction = direction + vector(0,1,0)
    end
    if scene.IsKeyDown("S") then
        direction = direction + vector(0,-1,0)
    end

    moveDelta = direction * speed * delta
    trans.position = trans.position + moveDelta

    scene.SetComponent(entityID, "Transform", trans)

    scene.SetCamPos(trans.position + vector(0,30,150))

end

function test:onCollision(what)
    --called on collision
    print("Collision "..what)
    local trans = scene.GetComponent(entityID, "Transform")
    trans.position = trans.position - moveDelta
    scene.SetComponent(entityID, "Transform", trans)
end

function test:rayIntersect()
    print("Grounded!")
end

function test:reset()
    print("RESET")
    local trans = scene.GetComponent(entityID, "Transform")
    trans.position = vector(10, 30, 0)
    scene.SetComponent(entityID, "Transform", trans)
end

return test